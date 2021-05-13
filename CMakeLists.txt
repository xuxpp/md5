
add_executable(app main.c md5.c)
add_executable(test_bin test.c md5.c)
add_executable(benchmark benchmark.c md5.c)

enable_testing()
add_test(md5_test test_bin ../dummy.pdf)