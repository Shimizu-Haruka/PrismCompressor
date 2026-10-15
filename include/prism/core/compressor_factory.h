#pragma once
#include"prism/core/codec.h"
#include"prism/core/types.h"

#include<memory>

namespace prism::core{
    class CompressorFactory{
    public:
        static std::unique_ptr<ICodec> create(AlgorithmType type);
    };
}