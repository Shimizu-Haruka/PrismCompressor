#pragma once

#include"prism/core/types.h"
#include<string>

namespace prism::core{
class CompressionEngine{
    CompressResult compressBuffer(//压缩内存里的数据
        const ByteBuffer& input,
        const CompressionOptions& options = {});

    ByteBuffer decompressBuffer(//解压文件里的内存
        const ByteBuffer&  prismData,
        const CompressionOptions& options = {});

    CompressResult compressFile(//压缩文件
        const std::string inputFile,
        const std::string outputFile,
        const CompressionOptions options = {});

    ByteBuffer decompressFile(//解压文件
        const std::string inputFile,
        const std::string outputFile,
        const CompressionOptions options = {});

        /*ByteBuffer 给gui用
          File       给cli用*/
};
}