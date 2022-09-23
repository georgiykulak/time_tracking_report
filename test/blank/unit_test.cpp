#include "../../src/time_tracking_report.hpp"

#include <iostream>
#include <cassert>

int main()
{
    tracking::TimeTrackingReport ttr_test("input.csv");
    
    if (ttr_test)
        assert( ttr_test.readFromInputFile() == 0 );

    std::cout << "Start writing to output file\n";
    assert( ttr_test.writeToOutputFile() == 0 );

    return 0;
}