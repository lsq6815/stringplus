# Unity is not available due to network restrictions.
# Using a minimal custom test framework instead.

# Simple test utilities
set(TEST_UTILS ${CMAKE_CURRENT_SOURCE_DIR}/test_utils.h)

# Test for stringplus
add_executable(test_stringplus test_stringplus.c)
target_link_libraries(test_stringplus stringplus)
add_test(NAME test_stringplus COMMAND test_stringplus)

# Test for ccary
add_executable(test_ccary test_ccary.c)
target_link_libraries(test_ccary stringplus)
add_test(NAME test_ccary COMMAND test_ccary)

# Test for object
add_executable(test_object test_object.c)
target_link_libraries(test_object stringplus)
add_test(NAME test_object COMMAND test_object)
