#pragma once

#include <iomanip>
#include <iostream>
#include <mutex>

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

    enum class BuildType
    {
        DEBUG,
        RELEASE
    };

    class Logger
    {
    public:
        static Logger& getInstance();

        template<typename T>
        Logger& operator<<( const T& message )
        {
            if ( quiet && ( currentLevel != LogLevel::WARN && currentLevel != LogLevel::ERROR ) )
            {
                return *this;
            }

            if ( currentLevel == LogLevel::DEBUG && buildType == BuildType::RELEASE )
            {
                return *this;
            }

            std::lock_guard<std::mutex> lock( mutex_ );
            std::cout << message << std::flush;
            return *this;
        }

        typedef std::ostream& ( *ManipFn )( std::ostream& );
        Logger& operator<<( ManipFn manip );

        static Logger& log( LogLevel level = LogLevel::INFO );

        static void setDestination( LogDestination destination );

        static void setBuildType( BuildType type ) { getInstance().buildType = type; }
        static void setQuiet( bool quiet ) { getInstance().quiet = quiet; }

    private:
        Logger() : currentLevel( LogLevel::INFO ) {}
        ~Logger() = default;

        Logger( const Logger& )            = delete;
        Logger& operator=( const Logger& ) = delete;

        std::mutex mutex_;
        LogLevel   currentLevel;

        std::string getTimestamp();
        std::string getColorPrefix();
        void        setLogLevel( LogLevel level );

        BuildType buildType = BuildType::RELEASE;
        bool      quiet     = false;
    };

}  // namespace Tomos
