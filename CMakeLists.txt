# 1. 需要最低版本 3.16 的 CMake，项目 PrismCompressor，用 C++ 语言
cmake_minimum_required(VERSION 3.16)
project(PrismCompressor LANGUAGES CXX)

# 2.要用 C++17 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 3. 零件库
add_library(prism_core
    src/bitstream/bit_writer.cpp
    src/bitstream/bit_reader.cpp
    src/util/frequency_table.cpp
    src/algorithms/huffman_tree.cpp
    src/algorithms/huffman.cpp
    src/algorithms/rle.cpp
    src/algorithms/lz77.cpp
    src/format/prism_format.cpp
    src/core/compressor_factory.cpp
    src/core/compression_engine.cpp
    src/core/algorithm_selector.cpp
    src/io/file_io.cpp
)

# 4. 去哪找头文件
target_include_directories(prism_core PUBLIC include)

# 5. 命令行程序
add_executable(prism_cli
    app/cli/main.cpp
)
target_link_libraries(prism_cli PRIVATE prism_core)

# 6. 测试程序
# 每个测试文件各自带 main()，独立成目标；test_main.cpp 只是 #include test_roundtrip.cpp
# （其中已定义 main），单独构建会重复定义 main，故不加入；两个 0 字节的空测试文件同样不加入。
enable_testing()
set(PRISM_TEST_TARGETS
    test_rle
    test_lz77
    test_huffman_tree
    test_prism_format
    test_file_io
    test_roundtrip
)
foreach(test_target ${PRISM_TEST_TARGETS})
    add_executable(${test_target} tests/${test_target}.cpp)
    target_link_libraries(${test_target} PRIVATE prism_core)
    add_test(NAME ${test_target} COMMAND ${test_target})
endforeach()