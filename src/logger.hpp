#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <fstream>
#include <ctime>

namespace tracking
{

    class Logger
    {
    public:
        ~Logger()
        {
            m_logStream.close();
        }

        Logger() = default;

        Logger( Logger const & logger )
        {
            m_logFileName = logger.m_logFileName;
            m_logStream.open( m_logFileName );
        }

        Logger( std::string const & logFileName )
        {
            m_logFileName = logFileName;
            m_logStream.open( m_logFileName );
        }

        void reset( std::string const & logFileName )
        {
            if ( m_logStream.is_open() )
                m_logStream.close();

            m_logStream.open( m_logFileName );
        }

        void log( std::string const & str )
        {
            m_logStream << "*** TTR LOG " << _currentDateTime() << " ***: " << str << "\n";
        }

    private:
        std::string m_logFileName = "/var/log/time_tracking_report.log";
        std::ofstream m_logStream;

        std::string _currentDateTime() const
        {
            std::time_t now = time( 0 );
            tm tstruct;
            char buf[ 80 ];
            tstruct = *localtime( &now );
            
            strftime( buf, sizeof( buf ), "%Y-%m-%d.%X", &tstruct );

            return buf;
        }
    };

} // namespace tracking

#endif /* LOGGER_HPP */