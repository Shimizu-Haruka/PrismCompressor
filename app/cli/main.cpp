#include "prism/core/compression_engine.h"
#include <iostream>
#include <string>

void printUsage()
{
    std::cout << "Usage:\n"
              << "  prism_cli compress <input.txt> <output.prism> [algo]\n"
              << "  prism_cli decompress <input.prism> <output.txt>\n"
              << "\n"
              << "Algorithms: rle, huffman, lz77, adaptive (default)\n";
}

prism::AlgorithmType parseAlgorithm(const std::string& algoStr)
{
    if (algoStr == "rle")     return prism::AlgorithmType::RLE;
    if (algoStr == "huffman") return prism::AlgorithmType::Huffman;
    if (algoStr == "lz77")    return prism::AlgorithmType::LZ77;
    return prism::AlgorithmType::Adaptive;
}

int main(int argc, char* argv[])
{

    if (argc < 4)
    {
        printUsage();
        return 1;
    }

    std::string command = argv[1];
    std::string inputPath = argv[2];
    std::string outputPath = argv[3];

    prism::core::CompressionEngine engine;
    prism::CompressionOptions options;

    if (command == "compress")
    {
        if (argc >= 5)
        {
            options.algorithm = parseAlgorithm(argv[4]);
        }

        std::cout << "Compressing " << inputPath << " ..." << std::endl;
        
        auto result = engine.compressFile(inputPath, outputPath, options);
        
        if (result.data.empty())
        {
            std::cerr << "Error: Compression failed. File may not exist." << std::endl;
            return 1;
        }

        std::cout << "Success!\n";
        std::cout << "Original size:   " << result.stats.originalSize << " bytes\n";
        std::cout << "Compressed size: " << result.stats.compressedSize << " bytes\n";
        std::cout << "Compression Ratio: " << result.stats.compressionRatio * 100 << "%\n";
    }
    else if (command == "decompress")
    {
        std::cout << "Decompressing " << inputPath << " ..." << std::endl;

        auto restored = engine.decompressFile(inputPath, outputPath, options);

        if (restored.empty())
        {
            std::cerr << "Error: Decompression failed. Invalid or corrupted file." << std::endl;
            return 1;
        }

        std::cout << "Success!\n";
        std::cout << "Restored size: " << restored.size() << " bytes\n";
    }
    else
    {
        std::cerr << "Unknown command: " << command << "\n";
        printUsage();
        return 1;
    }

    return 0;
}