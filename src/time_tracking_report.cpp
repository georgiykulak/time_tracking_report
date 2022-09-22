#include "time_tracking_report.hpp"

namespace tracking
{

    TimeTrackingReport::~TimeTrackingReport()
    {
        m_reportsCsv.close();
        m_summaryCsv.close();
    }

    TimeTrackingReport::TimeTrackingReport( std::string const & iPath, std::string const & oPath )
        : m_reportsCsvName{ iPath }
        , m_summaryCsvName{ oPath.empty() ? "output.csv" : oPath }
    {
        m_reportsCsv.open( m_reportsCsvName );
        
        if ( !m_reportsCsv )
            return;
        
        m_summaryCsv.open( m_summaryCsvName );
    }

    TimeTrackingReport::TimeTrackingReport( TimeTrackingReport const & ttr )
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
        // TODO

        return m_reportsSize;
    }

    std::size_t TimeTrackingReport::readFromInputFileAsync()
    {
        // TODO

        return m_reportsSize;
    }

    std::size_t TimeTrackingReport::writeToOutputFile()
    {
        // TODO

        return m_summarySize;
    }

} // namespace tracking
