#include"prism/algorithms/huffman_tree.h"

#include<iostream>
#include<cassert>

std::size_t weightPathLength(
    const prism::algorithms::HuffmanTree::NodePtr& node,
    std::size_t depth = 0)
{
    if(node == nullptr)
    {
        return 0;
    }
    if(node->is_leaf)
    {
        return node->frequency * depth;
    }

    return 
        weightPathLength(node->left, depth + 1) + 
        weightPathLength(node->right, depth + 1);
}

void testHuffmanTreeSimple()
{
    prism::ByteBuffer testData = {'a' , 'b' , 'a' , 'b' , 'a' , 'c'};
    prism::util::FrequencyTable freqTable ;
    freqTable.build(testData);

    assert(freqTable.get('a') == 3);

    prism::algorithms::HuffmanTree huffmanTree;
    prism::algorithms::HuffmanTree::NodePtr root = huffmanTree.build(freqTable);

    std::cout << weightPathLength(root , 0) << std::endl;

    assert(root != nullptr);
    assert(root->frequency == 6);
    assert(root->left->frequency == 3);

    std::cout << "test huffman tree simple pass" << std::endl;
}

void testGenerateCodes()
{
    prism::util::FrequencyTable freqTable;
    prism::ByteBuffer testData = {'a', 'b', 'a', 'b', 'c', 'a', 'a', 'd', 'c'};
    freqTable.build(testData);

    prism::algorithms::HuffmanTree huffmanTree;
    auto root = huffmanTree.build(freqTable);

    std::array<prism::algorithms::EncodedSymbol , 256> codes;
    codes = huffmanTree.generateCodes(root);

    assert(root->frequency == 9);
    assert(codes['a'].code == 0b000);
    assert(codes['d'].length == 3);

    std::cout << "test generate code pass" << std::endl;
}

int main()
{
    testHuffmanTreeSimple();
    testGenerateCodes();
    return 0;
}