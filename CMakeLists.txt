cmake_minimum_required( VERSION 3.16 )

project( time_tracking_report )

set( CMAKE_CXX_STANDARD 20 )

add_executable( test_blank    test/blank/unit_test.cpp src/time_tracking_report.cpp )
add_executable( test_read     test/read/unit_test.cpp src/time_tracking_report.cpp )
add_executable( test_read_SOH test/read_SOH/unit_test.cpp src/time_tracking_report.cpp )
