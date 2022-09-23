#include "time_tracking_report.hpp"

#include <sstream>
#include <ctime>
#include <chrono>
#include <iostream>

namespace tracking
{

    TimeTrackingReport::~TimeTrackingReport()
    {
        m_reportsCsv.close();
        m_summaryCsv.close();
    }

    TimeTrackingReport::TimeTrackingReport(
        std::string const & iPath,
        std::string const & oPath,
        char denominator
    )
        : m_reportsCsvName{ iPath }
        , m_summaryCsvName{ oPath.empty() ? "output.csv" : oPath }
        , m_denominator{ denominator }
    {
        m_reportsCsv.open( m_reportsCsvName );
        
        if ( !m_reportsCsv )
            return;
        
        m_summaryCsv.open( m_summaryCsvName );
    }

    TimeTrackingReport::TimeTrackingReport( TimeTrackingReport const & ttr )
        : m_denominator{ ttr.m_denominator }
    {
        TimeTrackingReport(ttr.m_reportsCsvName, ttr.m_summaryCsvName);

        m_reports = ttr.m_reports;
        m_reportsSize = ttr.m_reportsSize;
        m_summarySize = ttr.m_summarySize;
    }

    TimeTrackingReport& TimeTrackingReport::operator=( TimeTrackingReport const & ttr )
    {
        TimeTrackingReport(ttr.m_reportsCsvName, ttr.m_summaryCsvName);

        m_reports = ttr.m_reports;
        m_reportsSize = ttr.m_reportsSize;
        m_summarySize = ttr.m_summarySize;

        return *this;
    }

    TimeTrackingReport::operator bool() const noexcept
    {
        return m_reportsCsv && m_summaryCsv;
    }

    std::size_t TimeTrackingReport::readFromInputFile()
    {
        if ( !*this )
            return 0;

        BasicReportRow tmpRow;
        std::string tmpString;
        std::string tmpPart;
        
        m_reportsSize = 0;

        while ( std::getline( m_reportsCsv, tmpString ) )
        {
            std::cout << "Raw line " << m_reportsSize << ": " << tmpString << "\n";
            parseRow( tmpRow, tmpString );
            m_reports.push_back( std::move( tmpRow ) );
        }

        return m_reportsSize;
    }

    std::size_t TimeTrackingReport::readFromInputFileAsync()
    {
        if (!*this)
            return 0;

        // TODO: implement

        return m_reportsSize;
    }

    bool operator<( DateStamp const & lds, DateStamp const & rds )
    {
        if ( lds.m_year != rds.m_year )
            return lds.m_year < rds.m_year;

        if ( lds.m_month != rds.m_month )
            return lds.m_month < rds.m_month;
        
        return lds.m_day < rds.m_day;
    }

    std::size_t TimeTrackingReport::writeToOutputFile()
    {
        if ( m_reports.empty() )
            return 0;

        m_summarySize = 0;
        std::string denominator = ";";
        std::map< std::string, std::multimap< DateStamp, long long > > peopleMonthsMap;

        for ( auto const & report : m_reports )
        {
            auto const & name = report.m_name;

            if ( peopleMonthsMap.find( name ) == peopleMonthsMap.end() )
            {
                std::multimap< DateStamp, long long > tmpMap;
                tmpMap.insert({ report.m_date, report.m_hours });
                peopleMonthsMap.insert({ name, tmpMap });
            }
            else
            {
                peopleMonthsMap[ name ].insert({ report.m_date, report.m_hours });
            }

            std::cout << "peopleMap => {report.m_name:" << report.m_name
                << ", {report.m_date:" << report.m_date.toString()
                << ", report.m_hours:" << report.m_hours << "}}\n";
        }

        std::cout << "peopleMonthsMap.size() = " << peopleMonthsMap.size() << std::endl;

        _writeMapToFile( peopleMonthsMap );

        return m_summarySize;
    }

    void TimeTrackingReport::parseRow( BasicReportRow & ds, std::string & str )
    {
        std::string tmpPart;
        constexpr char const denominator = ';';
        std::stringstream sstr(str);
        
        std::getline(sstr, ds.m_name, denominator);
        std::getline(sstr, ds.m_email, denominator);
        std::getline(sstr, ds.m_department, denominator);
        std::getline(sstr, ds.m_position, denominator);
        std::getline(sstr, ds.m_project, denominator);
        std::getline(sstr, ds.m_task, denominator);

        std::getline(sstr, tmpPart, denominator);
        std::tm tm = {};
        strptime(tmpPart.c_str(), "%Y-%m-%d", &tm);
        ds.m_date.m_year = tm.tm_year + 1900;
        ds.m_date.m_month = tm.tm_mon + 1;
        ds.m_date.m_day = tm.tm_mday;

        std::getline(sstr, tmpPart, denominator);
        ds.m_hours = std::stoll(tmpPart);

        ++m_reportsSize;

        std::cout << "Parsed line:\n";
        std::cout << ds.m_name
                << "|" << ds.m_email
                << "|" << ds.m_department
                << "|" << ds.m_position
                << "|" << ds.m_project
                << "|" << ds.m_task
                << "|" << ds.m_date.toString()
                << "|" << std::to_string(ds.m_hours) << "\n";
    }

    void TimeTrackingReport::_writeMapToFile(
        std::map< std::string, std::multimap< DateStamp, long long > > const & peopleMonthsMap
    )
    {
        std::map< std::string, std::map< DateStamp, long long > > outputMap;

        for ( auto const & person : peopleMonthsMap )
        {
            std::map< DateStamp, long long > tmpMap;

            for ( auto const & monthHours : person.second )
            {
                auto hoursSet = person.second.equal_range( monthHours.first );
                long long hoursSum = 0;

                for ( auto it = hoursSet.first; it != hoursSet.second; ++it )
                    hoursSum += it->second;

                tmpMap.insert({ monthHours.first, hoursSum });
            }

            outputMap.insert({ person.first, tmpMap });
        }

        for ( auto const & report : outputMap )
        {
            for ( auto const & date : report.second )
            {
                m_summaryCsv << report.first;
                m_summaryCsv << m_denominator;
                m_summaryCsv << date.first.toString();
                m_summaryCsv << m_denominator;
                m_summaryCsv << date.second;
                m_summaryCsv << "\n";
                
                ++m_summarySize;
            }
        }
    }

} // namespace tracking
