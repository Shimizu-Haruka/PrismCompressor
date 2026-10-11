#include"prism/algorithms/huffman.h"
#include"prism/bitstream/bit_writer.h"

namespace prism::algorithms
{
    ByteBuffer HuffmanCompressor::compress(const ByteBuffer& input)const
    {
        if(input.empty())
        {
            return input;
        }
        bitstream::BitWriter writer;
        writer.writeBits(static_cast<uint64_t>(input.size()) , 32);//写入原字节大小

        util::FrequencyTable freq;
        freq.build(input);//统计频率

        auto root = HuffmanTree::build(freq);//建树
        
        auto codes= HuffmanTree::generateCodes(root);//生成编码表

        HuffmanTree::serialize(root , writer);//写入树结构

        for(auto& i : input)
        {
            writer.writeBits(codes[i].code , codes[i].length);
        }
        writer.finish();//写入

        return writer.getData();
    }

    std::string HuffmanCompressor::name()const{
        return "Huffman";
    }

    ByteBuffer HuffmanCompressor::decompress(const ByteBuffer& input)const
    {
        if(input.empty())
        {
            return input;
        }

        bitstream::BitReader reader(input , input.size() * 8);
        
        std::uint32_t originalSize = 0;
        if(!reader.readBits(32 , originalSize))//无法读取原大小
        {
            return {};
        }

        HuffmanTree::NodePtr root = HuffmanTree::deserialize(reader);//读取树结构
        if(root == nullptr)
        {
            return {};
        }

        ByteBuffer result;

        if(root->is_leaf)
        {
            result.reserve(originalSize);
            for(std::size_t i = 0; i < originalSize; ++i)
                result.push_back(root->character);
            return result;
        }

        auto current = root;
        result.reserve(originalSize);//预留空间
        while(result.size() < originalSize)
        {
            int bit = 0;
            if(!reader.readBit(bit))
            {
                break;
            }

            if(bit == 0)
            {
                current = current->left;
            }
            else
            {
                current = current->right;
            }
            if(current == nullptr)
            {
                break;
            }
            if(current->is_leaf)
            {
                result.push_back(current->character);//将叶子节点值写入
                current = root;//回到根节点
            }
        }

        return result;

    }

    std::string HuffmanCompressor::name()const{
        return "huffman";
    }
}