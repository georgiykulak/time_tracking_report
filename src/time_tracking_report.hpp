#ifndef TIME_TRACKING_REPORT
#define TIME_TRACKING_REPORT

#include <fstream>
#include <string>
#include <vector>

namespace tracking
{

    class TimeTrackingReport
    {
    public:
        struct BasicReportRow;

        ~TimeTrackingReport();

        TimeTrackingReport() = delete;

        TimeTrackingReport( std::string const & iPath, std::string const & oPath = "" );
        
        TimeTrackingReport( TimeTrackingReport const & ttr );

        TimeTrackingReport& operator=( TimeTrackingReport const & ttr );

        operator bool() const noexcept;

        std::size_t readFromInputFile();

        std::size_t readFromInputFileAsync();

        std::size_t writeToOutputFile();

        virtual void parseRow( BasicReportRow & ds, std::string & str );

    private:
        std::string m_reportsCsvName;
        std::string m_summaryCsvName;
        std::ifstream m_reportsCsv;
        std::ofstream m_summaryCsv;
        std::vector< BasicReportRow > m_reports;
        std::size_t m_reportsSize = 0;
        std::size_t m_summarySize = 0;
    };

    struct DateStamp
    {
        unsigned m_year;
        unsigned m_month;
        unsigned m_day;

        std::string toString() const
        {
            return std::to_string(m_year) + "-"
                 + std::to_string(m_month) + "-"
                 + std::to_string(m_day);
        }
    };

    struct TimeTrackingReport::BasicReportRow
    {
        std::string m_name;
        std::string m_email;
        std::string m_department;
        std::string m_position;
        std::string m_project;
        std::string m_task;
        DateStamp m_date;
        long long m_hours;
    };

} // namespace tracking

#endif /* TIME_TRACKING_REPORT */
