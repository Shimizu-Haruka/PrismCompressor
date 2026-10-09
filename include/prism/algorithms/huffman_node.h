#pragma once
#include"prism/core/types.h"
#include<cstddef>
#include<cstdint>
#include<memory>

namespace prism::algorithms{
    struct HuffmanNode{
        std::size_t frequency = 0;//字符出现频率
        Byte character;//节点对应字符
        bool is_leaf = 0;//节点是否为叶子结点

        std::shared_ptr<HuffmanNode> left = nullptr;//左儿子
        std::shared_ptr<HuffmanNode> right = nullptr;//右儿子
    };

    struct EncodedSymbol//编码字符
    {
        std::uint32_t code;//编码字符
        std::uint8_t length;//编码长度
    };
}