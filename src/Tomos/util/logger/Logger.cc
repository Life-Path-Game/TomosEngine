//
// Created by dstuden on 1/19/25.
//

#include "Logger.hh"

#include <source_location>

namespace Tomos
{

    Logger& Logger::getInstance()
    {
        static Logger instance;
        return instance;
    }

    Logger& Logger::operator<<( ManipFn manip )
    {
        std::lock_guard<std::mutex> lock( mutex_ );
        manip( std::cout );
        return *this;
    }

    Logger& Logger::log( LogLevel level, const std::source_location location )
    {
        Logger& instance = getInstance();

        if ( instance.quiet && ( instance.currentLevel != LogLevel::WARN && instance.currentLevel != LogLevel::ERROR ) )
        {
            return instance;
        }

        std::lock_guard<std::mutex> lock( instance.mutex_ );
        instance.setLogLevel( level );
        std::cout << std::endl
                  << instance.getTimestamp() << instance.getColorPrefix() << " " << location.function_name() << ": ";
        return instance;
    }

    void Logger::setDestination( LogDestination destination )
    {
        // Not implemented
    }

    void Logger::setLogLevel( LogLevel level ) { currentLevel = level; }

    std::string Logger::getColorPrefix()
    {
        switch ( currentLevel )
        {
            case LogLevel::DEBUG:
                return "[DEBUG] ";
            case LogLevel::INFO:
                return "[INFO] ";
            case LogLevel::WARN:
                return "[WARN] ";
            case LogLevel::ERROR:
                return "[ERROR] ";
            default:
                return "";
        }
    }

    std::string Logger::getTimestamp()
    {
        auto now        = std::chrono::system_clock::now();
        auto time_t_now = std::chrono::system_clock::to_time_t( now );
        auto ms         = std::chrono::duration_cast<std::chrono::milliseconds>( now.time_since_epoch() ) % 1000;

        std::tm local_tm = *std::localtime( &time_t_now );

        std::ostringstream oss;
        oss << "[" << std::put_time( &local_tm, "%Y-%m-%d %H:%M:%S" ) << "." << std::setfill( '0' ) << std::setw( 3 )
            << ms.count() << "] ";
        return oss.str();
    }


}  // namespace Tomos
