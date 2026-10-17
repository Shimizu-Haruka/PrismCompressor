#include"prism/algorithms/huffman_tree.h"
#include"prism/algorithms/huffman.h"

#include<iostream>
#include<cassert>
#include<vector>

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

void testRoot(prism::algorithms::HuffmanTree::NodePtr root)
{
    if(root == nullptr)
    {
        std::cout << "root is null" << std::endl;
        return;
    }
    std::cout << root->character << std::endl;
    if(root->left != nullptr)
        std::cout << "l" << root->left->character;
    if(root->right != nullptr)
        std::cout << "r"<< root->right->character;
    std::cout << std::endl;
    testRoot(root->left);
    testRoot(root->right);
    return;
}

void testSerializeDeserialize()
{
    prism::util::FrequencyTable freq;
    prism::ByteBuffer testData = {'a', 'b', 'a', 'b', 'c', 'a', 'a', 'd', 'c'};
    freq.build(testData);

    prism::bitstream::BitWriter writer;
    auto root = prism::algorithms::HuffmanTree::build(freq);
    prism::algorithms::HuffmanTree::serialize(root , writer);
    writer.finish();

    // std::cout << "root:" << std::endl;
    // testRoot(root);

    // for (int i = 0; i < 256; ++i) {
    // if (freq.get(i) > 0)
    //     std::cout << (char)i << " -> " << freq.get(i) << std::endl;
    // }
    //freq pass


    prism::bitstream::BitReader reader(writer.getData() , writer.totalBits());
    auto newRoot = prism::algorithms::HuffmanTree::deserialize(reader);

    // std::cout << "new root:" << std::endl;
    // testRoot(newRoot);

    auto codes1 = prism::algorithms::HuffmanTree::generateCodes(root);
    auto codes2 = prism::algorithms::HuffmanTree::generateCodes(newRoot);

    for (int i = 0; i < 256; ++i)
    {
        // std::cout << i << std::endl;
        // std::cout << codes1[i].code << ' ' << codes2[i].code << std::endl;
        assert(codes1[i].length == codes2[i].length);
        if (codes1[i].length > 0)
        {
            assert(codes1[i].code == codes2[i].code);
        }
    }//验证编码表是否一致

    std::cout << "test serialize deserialize pass" << std::endl;
    return;

}

void testHuffmanRoundTrip()
{
    prism::algorithms::HuffmanCompressor huff;
    std::vector<prism::ByteBuffer> testData = {
        {'a'},
        {},
        {'a' , 'b'},
        {'a' , 'b' , 'a' , 'b' , 'a' , 'c'},
        {'a' , 'b' , 'c' , 'd' , 'e' , 'e' , 'e'}
    };
    
    for(auto i : testData)
    {
        // std::cout << "test data: ";
        auto comp = huff.compress(i);
        auto decomp = huff.decompress(comp);

        assert(i == decomp);
    }

    std::cout << "test huffman round trip pass" << std::endl;
    return;
}

int main()
{
    testHuffmanTreeSimple();
    testGenerateCodes();
    testSerializeDeserialize();
    testHuffmanRoundTrip();
    return 0;
}