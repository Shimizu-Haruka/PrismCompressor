#pragma once

#include"types.h"
#include<string>

namespace prism::core
{
    class ICodec
    {
    public:
        virtual ~ICodec() = default;//析构函数

        virtual ByteBuffer compress(const ByteBuffer& input) = 0;//压缩
        virtual ByteBuffer decompress(const ByteBuffer& input) = 0;//解压
        virtual std::string name() const  = 0;//编码器名称
    };
}