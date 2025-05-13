#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <variant>
#include <type_traits>
#include <unordered_map>
#include <vector>
#include "Tomos/lib/json.hpp"
#include "Tomos/util/logger/Logger.hh"

using json = nlohmann::json;

namespace Tomos
{
    class ConfigManager;

    // Type alias for all supported JSON/C++ types
    using ConfigValue = std::variant<
        int, // JSON number (integer)
        unsigned int, // JSON number (unsigned)
        float, // JSON number (float)
        double, // JSON number (double)
        bool, // JSON boolean
        long, // JSON number (long)
        unsigned long, // JSON number (unsigned long)
        std::string, // JSON string
        std::vector<int>, // JSON array of integers
        std::vector<float>, // JSON array of floats
        std::vector<double>, // JSON array of doubles
        std::vector<bool>, // JSON array of booleans
        std::vector<std::string>, // JSON array of strings
        json // For nested objects or untyped values
    >;

    // Base class for configuration schemas
    class ConfigSchema
    {
    public:
        virtual ~ConfigSchema() = default;

        // Register all fields with the ConfigManager
        virtual void registerFields( ConfigManager& p_manager ) = 0;

        // Load values from JSON
        virtual void load( const json& p_data ) = 0;
    };

    // Concrete schema implementation using CRTP
    template<typename T>
    class ConfigSchemaImpl : public ConfigSchema
    {
    public:
        void registerFields( ConfigManager& p_manager ) override
        {
            static_cast<T*>( this )->defineFields( p_manager );
        }

        void load( const json& p_data ) override
        {
            static_cast<T*>( this )->loadFields( p_data );
        }
    };

    class ConfigManager
    {
    public:
        // Register a schema and its fields
        template<typename Schema>
        void setSchema()
        {
            bool isBase = std::is_base_of_v<ConfigSchema, Schema>;
            LOG_ASSERT_MSG( isBase,
                            "Schema must inherit from ConfigSchema" );

            m_currentSchema = std::make_unique<Schema>();
            m_currentSchema->registerFields( *this );
        }

        // Load configuration from file
        bool loadConfig( const std::string& p_configPath )
        {
            LOG_ASSERT_MSG( m_currentSchema,
                            "No schema set. Call setSchema() first." );

            try
            {
                std::ifstream configFile( p_configPath );
                if ( !configFile.is_open() )
                {
                    LOG_ERROR() << "Warning: Could not open config file: " << p_configPath
                            << ". Using defaults.";
                    return false;
                }

                json fileData       = json::parse( configFile );
                m_currentConfigPath = p_configPath;

                // Let the schema load the data it recognizes
                m_currentSchema->load( fileData );

                // Store the complete JSON for extended access
                m_configData = fileData;

                return true;
            }
            catch ( const json::parse_error& e )
            {
                LOG_ERROR() << "JSON parse error: " << e.what();
                return false;
            } catch ( const std::exception& e )
            {
                LOG_ERROR() << "Error loading config: " << e.what();
                return false;
            }
        }

        // Register a field with the manager
        template<typename T>
        void registerField( const std::string& p_key, T* p_fieldPtr, const T& p_defaultValue )
        {
            m_fieldRegistry[p_key] = p_fieldPtr;
            *p_fieldPtr            = p_defaultValue;

            // Store the default value in case we need to reset
            m_defaultValues[p_key] = p_defaultValue;
        }

        // Get a value (type-safe)
        template<typename T>
        T get( const std::string& p_key ) const
        {
            try
            {
                if ( m_configData.contains( p_key ) )
                {
                    return m_configData[p_key].get<T>();
                }

                // Check if we have a default value
                auto it = m_defaultValues.find( p_key );
                if ( it != m_defaultValues.end() )
                {
                    return std::get<T>( it->second );
                }

                throw std::runtime_error( "Key not found: " + p_key );
            }
            catch ( const json::exception& e )
            {
                LOG_ERROR() << "Type mismatch for key '" + p_key + "': " + e.what();
                throw;
            }
            catch ( const std::exception& e )
            {
                LOG_ERROR() << "Error getting value for key '" + p_key + "': " + e.what();
                throw;
            }
        }

        // Get raw JSON data
        const json& getRawData() const { return m_configData; }

    private:
        std::unique_ptr<ConfigSchema>                m_currentSchema;
        std::unordered_map<std::string, void*>       m_fieldRegistry;
        std::unordered_map<std::string, ConfigValue> m_defaultValues;
        json                                         m_configData;
        std::string                                  m_currentConfigPath;
    };


    class BaseConfig : public ConfigSchemaImpl<BaseConfig>
    {
    public:
        int           m_unassignedLayerId   = -1;
        unsigned long m_maxInstancesPerDraw = 1024;

        void defineFields( ConfigManager& p_manager )
        {
            p_manager.registerField( "unassignedLayerId", &m_unassignedLayerId, m_unassignedLayerId );
            p_manager.registerField( "maxInstancesPerDraw", &m_maxInstancesPerDraw, m_maxInstancesPerDraw );
        }

        void loadFields( const json& p_data )
        {
            if ( p_data.contains( "unnasignedLayerId" ) ) m_unassignedLayerId = p_data["unnasignedLayerId"];
            if ( p_data.contains( "maxInstancesPerDraw" ) ) m_maxInstancesPerDraw = p_data["maxInstancesPerDraw"];
        }
    };
} // Tomos
