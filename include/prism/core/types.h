#pragma once

#include<cstdint>
#include<vector>

namespace prism
{
    using Byte = std::uint8_t;
    using ByteBuffer = std::vector<Byte>;
    
enum class AlgorithmType{//枚举算法
    RLE,
    Huffman,
    LZ77,
    Adaptive//自动选择
};

struct CompressionOptions{//压缩选项
    AlgorithmType algorithm = AlgorithmType::Adaptive;//默认自动
    bool multithread = false;//默认无多线程
    int threadCount = 1;//线程数
};

struct CompressionStats{//统计数据
    std::size_t originalSize = 0;//原始大小
    std::size_t compressedSize = 0;//压缩后大小
    double compressionRatio = 0.0;//压缩率
    double compressTimeMS = 0.0;//压缩时间
    double decompressTimeMS = 0.0;//解压时间
};

struct CompressResult{//压缩结果
    ByteBuffer data;//数据
    CompressionStats stats;//统计信息
};
}