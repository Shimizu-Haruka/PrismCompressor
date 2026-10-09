#include"prism/algorithms/rle.h"

#include<cassert>
#include<iostream>

void testRleEmpty(){
    prism::ByteBuffer testData{};
    prism::algorithms::RLECompressor rle;
    auto compressed = rle.compress(testData);

    assert(compressed.size() == 0);
    prism::ByteBuffer result = rle.decompress(compressed);

    assert(result.size() == 0);
    assert(result == testData);

    std::cout << "test rle empty pass" << std::endl;
}

void testRleComplex()
{
    prism::ByteBuffer testData{'a' , 'a' , 'a' , 'a' , 'b' , 'b' , 'b'};
    prism::algorithms::RLECompressor rle;

    for(int i = 0; i < 1000; i++){
        testData.push_back('X');
    }
    prism::ByteBuffer compressed = rle.compress(testData);
    prism::ByteBuffer result = rle.decompress(compressed);

    assert(result == testData);

    std::cout << "test rle complex pass" << std ::endl;
    return;
}

int main()
{
    testRleEmpty();
    testRleComplex();
    return 0;
}