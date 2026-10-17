#include"prism/io/file_io.h"
#include"prism/core/compression_engine.h"
#include"prism/core/types.h"

#include<cassert>
#include<iostream>

void testWriteFile()
{
    prism::ByteBuffer test1 = {'a' , 'b' , 'c' , 'd'};
    std::string path = "test_write_file.bin";

    assert(prism::io::FileIO::writeFile(path , test1));

    auto read = prism::io::FileIO::readFile(path);
    assert(read == test1);

    std::cout << "test write file passed" << std::endl;
}

void testReadNoneText()
{
    auto data = prism::io::FileIO::readFile("definitely_not_exist_12345.bin");
    assert(data.empty());

    std::cout << "testReadNonexistent passed!" << std::endl;
}

void testFileCompressRoundTrip()
{
    prism::core::CompressionEngine engine;

    // 创建测试文件
    prism::ByteBuffer original;
    const char* text = "Hello World! This is a test for file compression. ";
    for (int i = 0; i < 50; ++i)
    {
        for (const char* p = text; *p; ++p)
        {
            original.push_back(static_cast<prism::Byte>(*p));
        }
    }

    prism::io::FileIO::writeFile("test_input.txt", original);

    prism::CompressionOptions options;
    options.algorithm = prism::AlgorithmType::LZ77;

    auto result = engine.compressFile("test_input.txt", "test_output.prism", options);
    assert(!result.data.empty());
    auto restored = engine.decompressFile("test_output.prism", "test_restored.txt", options);
    assert(restored == original);

    std::cout << "testFileCompressRoundTrip passed!" << std::endl;
    std::cout << "  original = " << original.size() << std::endl;
    std::cout << "  compressed = " << result.data.size() << std::endl;
}

int main()
{
    testWriteFile();
    testReadNoneText();
    testFileCompressRoundTrip();
    return 0;
}