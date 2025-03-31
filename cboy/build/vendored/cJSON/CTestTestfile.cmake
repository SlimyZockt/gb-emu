# CMake generated Testfile for 
# Source directory: /home/myt/Dokumente/projects/gb-emu/cboy/vendored/cJSON
# Build directory: /home/myt/Dokumente/projects/gb-emu/cboy/build/vendored/cJSON
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(cJSON_test "/home/myt/Dokumente/projects/gb-emu/cboy/build/vendored/cJSON/cJSON_test")
set_tests_properties(cJSON_test PROPERTIES  _BACKTRACE_TRIPLES "/home/myt/Dokumente/projects/gb-emu/cboy/vendored/cJSON/CMakeLists.txt;248;add_test;/home/myt/Dokumente/projects/gb-emu/cboy/vendored/cJSON/CMakeLists.txt;0;")
subdirs("tests")
subdirs("fuzzing")
