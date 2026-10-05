#include"../../include/prism/bitstream/bit_reader.h"

namespace prism::bitstream{
    BitReader::BitReader(const ByteBuffer& data, std::size_t validBits)
    : data_(data) , validBits_(validBits) , bitPos_(0)
    {
    }

    bool BitReader::readBit(int& bit)
    {
        if(bitPos_ >= validBits_)
        {
            return 0;
        }
        std::size_t byteIndex = bitPos_ / 8;
        std::size_t bitIndex = bitPos_ % 8;
        bit = (data_[byteIndex] >> (7 - bitIndex)) & 1;
        ++bitPos_;
        return 1;
    }

    bool BitReader::readBits(int count, std::uint32_t& value)
    {
        if(bitPos_ + static_cast<std::size_t>(count) >  validBits_)
        {
            return 0;
        }
        value = 0;
        for(int i = 0; i < count; ++i)
        {
            int bit = 0;
            readBit(bit);
            value = (value << 1) | bit;
        }
        return 1;
    }

    bool BitReader::eof() const
    {
        return bitPos_ >= validBits_;
    }

    std::size_t BitReader::remainingBits() const{
        return validBits_ - bitPos_;
    }

    void BitReader::reset()
    {
        bitPos_ = 0;
    }
}