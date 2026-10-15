#include"prism/core/compressor_factory.h"
#include"prism/algorithms/huffman.h"
#include"prism/algorithms/lz77.h"
#include"prism/algorithms/rle.h"

namespace prism::core{
    std::unique_ptr<ICodec> CompressorFactory(AlgorithmType type)
    {
        switch(type)
        {
            case AlgorithmType::RLE:
                return std::make_unique<algorithms::RLECompressor>();

            case AlgorithmType::Huffman:
                return std::make_unique<algorithms::HuffmanCompressor>();

            case AlgorithmType::LZ77:
                return std::make_unique<algorithms::lz77Compressor>();

            default:
                return nullptr;
        }
    }
}