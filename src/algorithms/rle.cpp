#include"prism/algorithms/rle.h"
#include<iostream>

namespace prism::algorithms{
ByteBuffer RLECompressor::compress(const ByteBuffer& input)const{
    if(input.empty())
    {
        return {};
    }
    ByteBuffer result;
    Byte currentByte = input[0];
    Byte count = 1;
    for(size_t i = 1; i < input.size(); i++){
        if(i == currentByte)
        {
            if(count < maxRunLength)
            {
                count++;
            }
            else{
                result.push_back(count);
                result.push_back(currentByte);
                currentByte = input[i];
                count = 1;
            }
        }
        else
        {
            result.push_back(count);
            result.push_back(currentByte);
            count = 1;
            currentByte = input[i];
        }
    }
    result.push_back(count);
    result.push_back(currentByte);
    return result;
}

ByteBuffer RLECompressor::decompress(const ByteBuffer& input)const{
    if(input.empty())
    {
        return {};
    }
    if(input.size() % 2 != 0)
    {
        std::cout << "Error:file is destoryed." << std::endl;
        return{};
    }
    ByteBuffer result;
    Byte count = 0;
    Byte currentByte = 0;
    for(size_t i = 0; i < input.size(); i+=2)
    {
        currentByte = input[i + 1];
        count = input[i];
        for(size_t j = 0; j <count; j++)
        {
            result.push_back(currentByte);
        }
    }
    return result;
}

std::string RLECompressor::name()const
{
    return "RLE";
}

}