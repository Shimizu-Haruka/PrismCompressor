#include"../../include/prism/bitstream/bit_writer.h"

namespace prism::bitstream
{
    void BitWriter::writeBit(int bit)
    {
        currentByte_ = (currentByte_ << 1) | (bit & 1);
        ++bitCount_;
        ++totalBits_;
        if(bitCount_ == 8)
        {
            data_.push_back(currentByte_);
            currentByte_ = 0;
            bitCount_ = 0;

        }
    }

    void BitWriter::writeBits(std::uint32_t value, int count)
    {
        for(int i = count - 1; i >= 0; --i)
        {
            int bit = (value >> i) & 1;
            writeBit(bit);
        }
    }

    const ByteBuffer& BitWriter::getData() const{
        return data_;
    }

    std::size_t BitWriter::bitCount() const{
        return static_cast<std::size_t>(bitCount_);//显式转换 忽略警告
    }

    void BitWriter::clear()
    {
        data_.clear();
        currentByte_ = 0;
        bitCount_ = 0;
        totalBits_ = 0;
    }

    void BitWriter::finish()
    {
        if(bitCount_ > 0)
        {
            currentByte_ <<= (8 - bitCount_);
            data_.push_back(currentByte_);
            
            currentByte_ = 0;
            bitCount_ = 0;//清除缓存
        }
    }

    std::size_t BitWriter::totalBits()const
    {
        return totalBits_;
    }
}