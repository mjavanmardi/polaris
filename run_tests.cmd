cd build_vs2015\bin\Debug
Core_Tests.exe --gtest_output="xml:core_results.xml"
router_tests.exe --gtest_output="xml:router_results.xml"
cd ..\Release
Core_Tests.exe --gtest_output="xml:core_results.xml"
router_tests.exe --gtest_output="xml:router_results.xml"
cd ..\..\..
