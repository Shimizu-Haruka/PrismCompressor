#include"prism/core/compression_engine.h"
#include"prism/core/compressor_factory.h"
#include"prism/format/prism_format.h"
#include"prism/core/algorithm_selector.h"
#include"prism/io/file_io.h"
// #include"prism/io/file_io.h"

namespace prism::core{

CompressResult CompressionEngine::compressBuffer(
    const ByteBuffer& input,
    const CompressionOptions& options
)
{
    CompressResult result;
    result.stats.originalSize = input.size();

    if(input.empty())
    {
        return result;
    }

    //create algorithms
    AlgorithmType actualAlgorithm = options.algorithm;

    if (options.algorithm == AlgorithmType::Adaptive)
    {
        actualAlgorithm = AlgorithmSelector::select(input);
    }

    auto codec = CompressorFactory::create(actualAlgorithm);


    if (!codec)
    {
        return result;
    }
    //压缩得到payload
    ByteBuffer payload = codec->compress(input);

    format::PrismHeader header;//构建头
    header.version = 1;
    header.algorithm = static_cast<std::uint8_t>(actualAlgorithm);
    header.originalSize = static_cast<std::uint32_t>(input.size());
    header.compressedSize = static_cast<std::uint32_t>(payload.size());
    header.checkSum = format::PrismFile::computeChecksum(payload);

    //打包
    result.data = format::PrismFile::pack(header,payload);
    result.stats.compressedSize = result.data.size();
    result.stats.compressionRatio =
        static_cast<double>(result.stats.compressedSize) /
        static_cast<double>(result.stats.originalSize);
    return result;
}

ByteBuffer CompressionEngine::decompressBuffer(
    const ByteBuffer&  prismData,
    const CompressionOptions& options)
    {
        if(prismData.empty())
        {
            return {};
        }

        format::PrismHeader header;
        ByteBuffer payload;

        if (!format::PrismFile::unpack(prismData, header, payload))
        {
            return {};
        }

        if(header.version != 1)
        {
            return {};
        }

        if(format::PrismFile::computeChecksum(payload) != header.checkSum)
        {
            return {};
        }

        auto algorithm = static_cast<AlgorithmType>(header.algorithm);
        auto codec = CompressorFactory::create(algorithm);
        if(!codec)
        {
            return {};
        }

        return codec->decompress(payload);
    }

CompressResult CompressionEngine::compressFile(
    const std::string inputFile,
    const std::string outputFile,
    const CompressionOptions options
)
{

    CompressResult result;

    ByteBuffer input = io::FileIO::readFile(inputFile);
    if(input.empty())
    {
        return {};
    }

    result = compressBuffer(input , options);

    if(!result.data.empty())
    {
        io::FileIO::writeFile(outputFile , result.data);
    }

    return result;
}

ByteBuffer CompressionEngine::decompressFile(
        const std::string inputFile,
        const std::string outputFile,
        const CompressionOptions options)
{
    ByteBuffer prismData = io::FileIO::readFile(inputFile);
    if(prismData.empty())
    {
        return {};
    }

    ByteBuffer original = decompressBuffer(prismData , options);

    if(!original.empty())
    {
        io::FileIO::writeFile(outputFile , original);
    }

    return original;
}

}