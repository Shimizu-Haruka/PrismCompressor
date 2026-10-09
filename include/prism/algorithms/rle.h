#pragma once

#include"prism/core/codec.h"

namespace prism::algorithms{
    class RLECompressor : public core::ICodec{
        public:
        ByteBuffer compress(const ByteBuffer& input)const override;//压缩
        ByteBuffer decompress(const ByteBuffer& input)const override;//解压
        std::string name() const override;//压缩算法名称
        private:
        static constexpr Byte maxRunLength = 255;
    };
}