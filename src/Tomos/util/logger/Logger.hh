#pragma once

#include <iomanip>
#include <iostream>
#include <mutex>
#include <source_location>

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
        Logger& operator<<( const T& message )
        {
            if ( quiet && ( currentLevel != LogLevel::WARN && currentLevel != LogLevel::ERROR ) )
            {
                return *this;
            }

            std::lock_guard<std::mutex> lock( mutex_ );
            std::cout << message << std::flush;
            return *this;
        }

        typedef std::ostream& ( *ManipFn )( std::ostream& );
        Logger& operator<<( ManipFn manip );

        static Logger& log( LogLevel                   level    = LogLevel::INFO,
                            const std::source_location location = std::source_location::current() );

        static void setDestination( LogDestination destination );

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

        bool quiet = false;
    };

}  // namespace Tomos

// ======= MACROS FOR LOGGING =======

// Always log INFO, WARN, and ERROR
#define LOG_INFO() Tomos::Logger::log( Tomos::LogLevel::INFO )
#define LOG_WARN() Tomos::Logger::log( Tomos::LogLevel::WARN )
#define LOG_ERROR() Tomos::Logger::log( Tomos::LogLevel::ERROR )

// Only for debug builds
#ifndef NDEBUG
#define LOG_DEBUG() Tomos::Logger::log( Tomos::LogLevel::DEBUG )
#else  // real
#define LOG_DEBUG() \
    if ( false ) Tomos::Logger::log( Tomos::LogLevel::DEBUG )
#endif
