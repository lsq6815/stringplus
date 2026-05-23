# Unity test framework (object library, avoids recompilation)
# 禁止对第三方代码运行 clang-tidy 和 Werror
add_library(unity_obj OBJECT unity/unity.c)
target_include_directories(unity_obj PUBLIC unity)
target_compile_options(unity_obj PRIVATE -w)
set_target_properties(unity_obj PROPERTIES C_CLANG_TIDY "")

# Test for stringplus
add_executable(test_stringplus test_stringplus.c)
target_link_libraries(test_stringplus stringplus unity_obj)
add_test(NAME test_stringplus COMMAND test_stringplus)

# Test for ccary
add_executable(test_ccary test_ccary.c)
set_target_properties(test_ccary PROPERTIES C_CLANG_TIDY "")
target_link_libraries(test_ccary stringplus unity_obj)
add_test(NAME test_ccary COMMAND test_ccary)

# Test for object
add_executable(test_object test_object.c)
target_link_libraries(test_object stringplus unity_obj)
add_test(NAME test_object COMMAND test_object)
