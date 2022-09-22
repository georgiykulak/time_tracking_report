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
        ~TimeTrackingReport();

        TimeTrackingReport() = delete;

        TimeTrackingReport( std::string iPath, std::string oPath = "" );
        
        TimeTrackingReport( TimeTrackingReport const & ttr );

        TimeTrackingReport& operator=( TimeTrackingReport const & ttr );

        operator bool() const noexcept;

        std::size_t readFromInputFile();

        std::size_t readFromInputFileAsync();

        std::size_t writeToOutputFile();

        struct BasicReportRow;

    private:
        std::ifstream m_reportsCsv;
        std::ofstream m_summaryCsv;
        std::vector< BasicReportRow > m_reports;
        std::size_t m_reportsSize;
        std::size_t m_summarySize;
    };

    struct DateStamp
    {
        std::size_t m_year;
        unsigned m_month;
        unsigned m_day;
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
        double m_hours;
    };

} // namespace tracking

#endif /* TIME_TRACKING_REPORT */
