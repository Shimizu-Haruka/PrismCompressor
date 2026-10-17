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
#add_executable(prism_tests
#    tests/test_main.cpp
 #   tests/test_bit_writer.cpp
    # ... 你的所有测试 .cpp 文件
)
target_link_libraries(prism_tests PRIVATE prism_core)