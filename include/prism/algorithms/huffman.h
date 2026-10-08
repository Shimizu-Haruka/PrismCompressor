#pragma once
#include"prism/core/codec.h"
#include"prism/algorithms/huffman_tree.h"

namespace prism::algorithms
{
class HuffmanCompressor : public core::ICodec
{
public:
    ByteBuffer compress(const ByteBuffer& input)const override;//压缩
    ByteBuffer decompress(const ByteBuffer& input)const override;//解压
    std::string name()const override;//编码器名称
};
}