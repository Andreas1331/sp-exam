# CMake generated Testfile for 
# Source directory: C:/Users/andre/Documents/GitHub/sp_exam/project
# Build directory: C:/Users/andre/Documents/GitHub/sp_exam/project/cmake-build-debug
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[unit_tests]=] "C:/Users/andre/Documents/GitHub/sp_exam/project/cmake-build-debug/unittests.exe")
set_tests_properties([=[unit_tests]=] PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/andre/Documents/GitHub/sp_exam/project/CMakeLists.txt;17;add_test;C:/Users/andre/Documents/GitHub/sp_exam/project/CMakeLists.txt;0;")
add_test([=[plot_tests]=] "python" "C:/Users/andre/Documents/GitHub/sp_exam/project/py/plotter.py")
set_tests_properties([=[plot_tests]=] PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/andre/Documents/GitHub/sp_exam/project/CMakeLists.txt;18;add_test;C:/Users/andre/Documents/GitHub/sp_exam/project/CMakeLists.txt;0;")
