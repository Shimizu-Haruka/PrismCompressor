#include"prism/core/algorithm_selector.h"
#include"prism/core/compressor_factory.h"

namespace prism::core{
AlgorithmType AlgorithmSelector::select(const ByteBuffer& input)
{
    if(input.size() < kMinSizeForTest)
    {
        return AlgorithmType::Huffman;
    }

    AlgorithmType candidate[] = {
        AlgorithmType::RLE,
        AlgorithmType::Huffman,
        AlgorithmType::LZ77
    };

    AlgorithmType best = AlgorithmType::Huffman;
    std::size_t bestSize = input.size();

    for(AlgorithmType algo : candidate)
    {
        auto codec = CompressorFactory::create(algo);
        if(!codec)
        {
            continue;
        }

        ByteBuffer compressed = codec->compress(input);
        if(compressed.empty())
        {
            continue;
        }

        if(bestSize < compressed.size())
        {
            best = algo;
            bestSize = compressed.size();
        }
    }

    return best;
}
}