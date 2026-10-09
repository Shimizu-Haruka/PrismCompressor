#pragma once
#include"prism/algorithms/huffman_node.h"
#include"prism/core/types.h"
#include"prism/util/frequency_table.h"
#include"prism/bitstream/bit_writer.h"
#include"prism/bitstream/bit_reader.h"

namespace prism::algorithms{
    class HuffmanTree{
    public:
        using NodePtr = std::shared_ptr<HuffmanNode>;//节点指针
        static NodePtr build(const util::FrequencyTable& freq);//建树

        static std::array<EncodedSymbol , 256>
        generateCodes(const NodePtr& root);//生成编码表，对外入口

    private:
    struct CompareNode//NodePtr比较函数，用于优先队列
    {
        bool operator()(const NodePtr& a, const NodePtr& b) const;//重载运算符
    }; 

    static void traverse(//遍历树，生成编码表
        const NodePtr& root,
        uint32_t code,
        uint8_t length,
        std::array<EncodedSymbol , 256>& codes);
    
    public:
    static void serialize(//序列化树
        const NodePtr& node,
        bitstream::BitWriter& writer);
    
    static NodePtr deserialize(//反序列化树
        bitstream::BitReader& reader
    );
    };
}

