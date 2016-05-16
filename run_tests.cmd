cd build_vs2015\bin\Debug
Core_Tests.exe --gtest_output="xml:..\tests\core_results_rel.xml"
router_tests.exe --gtest_output="xml:..\tests\router_results_rel.xml"
cd ..\Release
Core_Tests.exe --gtest_output="xml:..\tests\core_results_dbg.xml"
router_tests.exe --gtest_output="xml:..\tests\router_results_dbg.xml"
cd ..\..\..
