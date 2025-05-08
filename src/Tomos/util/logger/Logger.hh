#pragma once

#include <iomanip>
#include <iostream>
#include <mutex>
#include <source_location>
#include <cassert>

namespace Tomos
{
    enum class LogLevel
    {
        INFO,
        DEBUG,
        WARN,
        ERROR
    };

    enum class LogDestination
    {
        CONSOLE,
        FILE
    };

    class Logger
    {
    public:
        static Logger& getInstance();

        template<typename T>
        Logger& operator<<( const T& p_message )
        {
            std::lock_guard<std::mutex> lock( m_mutex );
            std::cout << p_message << std::flush;
            return *this;
        }

        typedef std::ostream& ( *ManipFn )( std::ostream& );
        Logger&                  operator<<( ManipFn p_manip );

        static Logger& log( LogLevel             p_level    = LogLevel::INFO,
                            std::source_location p_location = std::source_location::current() );

    private:
        Logger() :
            m_currentLevel( LogLevel::INFO )
        {
        }

        ~Logger() = default;

        Logger( const Logger& )            = delete;
        Logger& operator=( const Logger& ) = delete;

        std::mutex m_mutex{};
        LogLevel   m_currentLevel;

        std::string getTimestamp();
        std::string getPrefix();
        void        setLogLevel( LogLevel p_level );
    };
} // namespace Tomos

// ======= MACROS FOR LOGGING =======

// Always log INFO, WARN, and ERROR, ASSERT
#define LOG_INFO() Tomos::Logger::log( Tomos::LogLevel::INFO )
#define LOG_WARN() Tomos::Logger::log( Tomos::LogLevel::WARN )
#define LOG_ERROR() Tomos::Logger::log( Tomos::LogLevel::ERROR )
#define LOG_ASSERT_MSG( p_expression, p_message )  \
    if ( !( p_expression ) )        \
    {                               \
        LOG_ERROR() << p_message;   \
        assert( p_expression );     \
    }
#define LOG_ASSERT( p_expression )  \
    if ( !( p_expression ) )        \
    {                               \
        LOG_ERROR() << "Assertion failed"; \
        assert( p_expression );     \
    }

// Only for debug builds
#ifndef NDEBUG
#define LOG_DEBUG() Tomos::Logger::log( Tomos::LogLevel::DEBUG )
#else  // real
#define LOG_DEBUG() \
    if ( false ) Tomos::Logger::log( Tomos::LogLevel::DEBUG )
#endif
