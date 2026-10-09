#include"prism/algorithms/huffman_tree.h"
#include<vector>
#include<queue>

namespace prism::algorithms{
    bool HuffmanTree::CompareNode::operator()(const NodePtr& a, const NodePtr& b) const
    {
        return a->frequency > b->frequency;//小根堆
    }

    HuffmanTree::NodePtr HuffmanTree::build(const util::FrequencyTable& freq)
    {
        std::priority_queue<NodePtr , std::vector<NodePtr> , CompareNode> pq;

        for(int i = 0; i < 256; ++i)//建立节点并压入优先队列
        {
            Byte b = static_cast<Byte>(i);
            std::size_t count = freq.get(b);

            if(count == 0)
            {
                continue;
            }
            
            auto node = std::make_shared<HuffmanNode>();
            node->frequency = count;
            node->character = b;
            node->is_leaf = true;
            pq.push(node);

        }

        if(pq.size() == 1)//只有一个字符的特殊情况
        {
            return pq.top();
        }


        while(pq.size() > 1)
        {
            auto left = pq.top();
            pq.pop();

            auto right = pq.top();
            pq.pop();

            auto parent = std::make_shared<HuffmanNode>();
            parent->left = left;
            parent->right = right;
            parent->frequency = left->frequency + right->frequency;//父节点频率

            pq.push(parent);//压入优先队列
        }
        return pq.top();//返回根节点
    }

    std::array<prism::algorithms::EncodedSymbol , 256>
    HuffmanTree::generateCodes(const prism::algorithms::HuffmanTree::NodePtr& root)
    {
        std::array<EncodedSymbol , 256> codes{};

        if(root == nullptr)
        {
            return codes;
        }

        traverse(root , 0 , 0 , codes);
        return codes;
    }

    void HuffmanTree::traverse(const NodePtr& root,
    uint32_t code,
    uint8_t length,
    std::array<EncodedSymbol , 256>& codes)
    {
        if(root == nullptr)
        {
            return;
        }

        if(root->is_leaf)
        {
            codes[root->character].code = code;
            codes[root->character].length = (length == 0) ? 1 : length;
            return;
        }

        traverse(root->left , code << 1 , length + 1 , codes);
        traverse(root->right , code << 1 | 1 , length + 1 , codes);
        return;
    }

    void HuffmanTree::serialize(const NodePtr& node,bitstream::BitWriter& writer)
    {
        if(node == nullptr)
        {
            return;
        }
        if(node->is_leaf)
        {
            writer.writeBit(1);
            writer.writeBits(node->character , 8);
            return;
        }

        writer.writeBit(0);
        serialize(node->left , writer);
        serialize(node->right , writer);
        return;
    }

    HuffmanTree::NodePtr HuffmanTree::deserialize(bitstream::BitReader& reader)
    {
        int bit = 0;
        if(!reader.readBit(bit))
        {
            return nullptr;
        }

        if(bit == 1)
        {
            std::uint32_t value = 0;
            if(!reader.readBits(8 , value))
            {
                return nullptr;
            }
            auto node = std::make_shared<HuffmanNode>();
            node->character = static_cast<Byte>(value);
            node->is_leaf = true;
            return node;
        }
        else
        {
            auto node = std::make_shared<HuffmanNode>();
            node->left = deserialize(reader);
            node->right = deserialize(reader);
            node->is_leaf = false;
            return node;
        }
    }
}