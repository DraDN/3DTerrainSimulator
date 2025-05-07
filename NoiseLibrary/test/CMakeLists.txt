# add_subdirectory(dependencies/FastNoise2)

add_executable(fast_noise_test_ex main.cpp)

find_package(Threads REQUIRED)

# target_include_directories(fast_noise_test_ex PRIVATE ../dependencies)

target_link_libraries(fast_noise_test_ex PRIVATE FastNoise2 Threads::Threads)

add_test(fast_noise_test fast_noise_test_ex)
