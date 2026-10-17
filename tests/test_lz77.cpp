#include"prism/algorithms/lz77.h"
#include<cassert>
#include<iostream>

void testLz77Empty()
{
    prism::ByteBuffer testData = {};
    prism::algorithms::lz77Compressor lz;
    assert(lz.compress(testData).empty());
    assert(lz.decompress(testData).empty());
    std::cout << "test lz77 empty pass" << std::endl;
}

void testLz77Simple()
{
    prism::ByteBuffer testData = {'a'};
    prism::algorithms::lz77Compressor lz;
    auto compressed = lz.compress(testData);
    auto decompressed = lz.decompress(compressed);
    assert(testData == decompressed);
    std::cout << "test lz77 simple pass" << std:: endl;
}

void testLz77AbcAbcAbc()
{
    prism::ByteBuffer testData = {'a' , 'b' , 'c' , 'a' , 'b' , 'c' , 'a' , 'b' , 'c'};
    prism::algorithms::lz77Compressor lz;
    auto compressed = lz.compress(testData);
    auto decompressed = lz.decompress(compressed);
    assert(testData == decompressed);
    std::cout << "test lz77 abcabcabc pass" << std::endl << compressed.size() << std::endl;
}

void testLz77LongText()
{
    const char* testText = "The quick brown fox jumps over the lazy dog. ";
    prism::ByteBuffer testData;
    prism::algorithms::lz77Compressor lz;
    for(int i = 0; i < 20; i++)
    {
        for(const char* p = testText; *p; ++p)
        {
            testData.push_back(static_cast<prism::Byte>(*p));
        }
        auto compressed = lz.compress(testData);
        auto decompressed = lz.decompress(compressed);
    
    assert(decompressed == testData);

    std::cout << "test lz77 long text passed! " << testData.size()
              << " compressed=" << compressed.size() << std::endl;
    }
}

int main()
{
    testLz77Empty();
    testLz77Simple();
    testLz77AbcAbcAbc();
    testLz77LongText();
}