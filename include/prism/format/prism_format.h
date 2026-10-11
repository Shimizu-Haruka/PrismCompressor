#pragma once

#include"prism/core/types.h"
#include<cstdint>

namespace prism::format{

    struct PrismHeader{
        std::uint32_t magic = 0x5052534D;// 4Byte PRSM
        std::uint8_t version = 1;         //1Byte
        std::uint8_t algorithm = 0;       //1Byte
        std::uint16_t reserved;            //2Byte
        std::uint32_t originalSize;       //4Byte
        std::uint32_t compressedSize;     //4Byte
        std::uint32_t checkSum;           //4Byte
    };

    class PrismFile{
    public:
        static constexpr std::size_t kHeaderSize = 20;//头字节
        static ByteBuffer pack(const PrismHeader& header , const ByteBuffer& payload);//打包
        static bool unpack(const ByteBuffer& fileData, PrismHeader& header ,ByteBuffer& payload);//解包
        static std::uint32_t computeChecksum(const ByteBuffer& data);//算检验和
    };
}