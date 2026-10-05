#pragma once

#include"../core/types.h"

#include<cstddef>
#include<cstdint>

namespace prism::bitstream{
class BitReader{
private:
    const ByteBuffer& data_;//需要读取的字节数据
    std::size_t validBits_ = 0;//总有效bit
    std::size_t bitPos_ = 0;//当前读到的bit位置
public:
    BitReader(const ByteBuffer& data, std::size_t validBits);//保存传入数据并初始化

    //true:成功 false:失败
    bool readBit(int& bit);//读取一个bit
    bool readBits(int count,std::uint32_t& value);//读取count个bit, 存入value中
    bool eof() const;//文件是否读完 true:finish false:not finish
    std::size_t remainingBits() const;//还剩多少bits没有读完
    
    void reset();//重置 回到开头
};
}