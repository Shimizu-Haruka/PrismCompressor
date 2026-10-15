#pragma once
#include"prism/core/types.h"

namespace prism::core{
class AlgorithmSelector{
public:
    static AlgorithmType select(const ByteBuffer& input);//返回选中的算法
private:
    // 数据太短时直接返回 Huffman
    static constexpr std::size_t kMinSizeForTest = 64;
};
}