#pragma once
#include"prism/core/types.h"

#include<array>
#include<cstddef>

namespace prism::util{
    class FrequencyTable{
    public:
        void build(const ByteBuffer& input);//从输入数据里统计频率
        void clear();//清空频率统计
        std::size_t get(Byte value) const;//获取某个Byte出现的频率
        bool empty() const;//判断列表是否为空

    private:
        std::array<std::size_t , 256> counts_{};//Byte出现的次数列表
    };
}