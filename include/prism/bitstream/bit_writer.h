#pragma once
#include"../core/types.h"
#include<cstddef>
#include<cstdint>

namespace prism::bitstream
{
    class BitWriter
    {
    public:
        void writeBit(int bit);//写入一个位
        void writeBits(uint32_t value ,int count);//写入count个value位
        void finish();//处理剩余的位
        void clear();//清空缓冲区
        const ByteBuffer& getData() const;//获取缓冲区数据
        std::size_t bitCount() const;//获取缓冲区中有效位的数量
    private:
        Byte currentByte_ = 0; //当前字节
        int bitCount_ = 0;//当前字节中的有效位数（满8归零）
        ByteBuffer data_; //当前已打包好的byte
    };
}