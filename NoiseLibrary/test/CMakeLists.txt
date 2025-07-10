# add_subdirectory(dependencies/FastNoise2)

add_executable(fast_noise_test_ex main.cpp)

find_package(Threads REQUIRED)

target_include_directories(fast_noise_test_ex PRIVATE ../include)

target_link_libraries(fast_noise_test_ex PRIVATE FastNoise2 Threads::Threads)
# target_link_libraries(fast_noise_test_ex PRIVATE FastNoise2)

add_test(fast_noise_test fast_noise_test_ex)
