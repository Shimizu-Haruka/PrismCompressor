#include"prism/algorithms/lz77.h"
#include"prism/bitstream/bit_writer.h"
#include"prism/bitstream/bit_reader.h"
namespace prism::algorithms{
    std::uint32_t lz77Compressor::hash3(const ByteBuffer& input , std::size_t pos)
    {
        std::uint32_t hash = input[pos];
        hash = (hash << 8) ^ input[pos + 1];
        hash = (hash << 8) ^ input[pos + 2];
        return hash % kHashSize;
    }

    void lz77Compressor::insertHash(const ByteBuffer& input,/*输入*/
            std::size_t pos,/*位置*/
            std::vector<std::uint32_t>& head,/*哈希表*/
            std::vector<std::uint32_t>& prev/*前驱*/)
    {
        if(pos + 2 >= input.size())
        {
            return;
        }
        std::uint32_t h = hash3(input, pos);
        prev[pos] = head[h];
        head[h] = static_cast<std::uint32_t>(pos);
        return;
    }

    std::size_t lz77Compressor::findMatch(
        const ByteBuffer& input,
            std::size_t pos,
            std::vector<std::uint32_t>& head,
            std::vector<std::uint32_t>& prev,
            std::size_t& matchOffset
    )
    {
        matchOffset = 0;
        std::size_t bestLength = 0;

        if(pos + 2 >= input.size())
        {
            return 0;
        }

        std::uint32_t h = hash3(input, pos);
        std::uint32_t candidate = head[h];

        std::size_t windowStart = (pos > kWindowSize) ? (pos - kWindowSize) : 0;//滑动窗口的起始位置

        while(candidate != kInvalidPos && candidate >= windowStart)
        {
            std::size_t length = 0;

            while(pos + length < input.size()
            && input[candidate + length] == input[pos + length]
            && length < kMaxMatchLength)
            {
                ++length;
            }

            if(length > bestLength)
            {
                bestLength = length;
                matchOffset = pos - candidate;
            }
            candidate = prev[candidate];
        }
        return bestLength;
    }

    ByteBuffer lz77Compressor::compress(const ByteBuffer& input)const{
        if(input.empty())
        {
            return {};
        }
        std::vector<std::uint32_t> head(kHashSize , kInvalidPos);//初始化
        std::vector<std::uint32_t> prev(input.size() , kInvalidPos);

        bitstream::BitWriter writer;
        std::size_t pos = 0;
        writer.writeBits(static_cast<std::uint32_t>(input.size()) , 32);

        while(pos < input.size())
        {
            std::size_t offset = 0;
            std::size_t length = findMatch(input , pos , head , prev , offset);

            if (length >= kMinMatchLength && pos + length >= input.size())
            {
                length = input.size() - pos - 1; 
                if (length < kMinMatchLength)
                {
                    length = 0;
                }
            }
            if(length >= kMinMatchLength)
            {
                writer.writeBit(1);
                writer.writeBits(static_cast<std::uint32_t>(offset) , 12);
                writer.writeBits(static_cast<std::uint32_t>(length) , 8);
                writer.writeBits(input[pos + length] , 8);

                for(std::size_t i = 0; i <= length ; i++)
                {
                    insertHash(input , pos + i , head , prev);
                }

                pos += length + 1;
            }
            else{
                writer.writeBit(0);
                writer.writeBits(input[pos] , 8);

                insertHash(input , pos , head , prev);
                pos += 1;
            }
        }
        writer.finish();
        return writer.getData();
    };

    ByteBuffer lz77Compressor::decompress(const ByteBuffer& input)const{
        if(input.empty())
        {
            return {};
        }

        bitstream::BitReader reader(input , input.size() * 8);

        uint32_t originalSize = 0;//原始数据大小
        if(!reader.readBits(32 , originalSize))
        {
            return {};
        }

        ByteBuffer result;
        result.reserve(originalSize);

        while(result.size() < originalSize)
        {
            int flag = 0;
            if(!reader.readBit(flag))
            {
                break;
            }
            if(flag == 1)
            {
                std::uint32_t offset = 0;
                std::uint32_t length = 0;
                std::uint32_t nextByte = 0;
                if(!reader.readBits(12 , offset) ||
                    !reader.readBits(8 , length) ||
                    !reader.readBits(8 , nextByte))
                {
                    break;
                }
                
                std::size_t copyStart = result.size() - offset;
                for(std::uint32_t i = 0; i < length ; ++i)
                {
                    result.push_back(result[copyStart + i]);
                }
                result.push_back((static_cast<Byte>(nextByte)));
            }
            else
            {
                std::uint32_t b = 0;
                if(!reader.readBits(8 , b))
                {
                    break;
                }
                result.push_back(static_cast<Byte>(b));
            }
        }
        return result;
    }

    std::string lz77Compressor::name()const
    {
        return "lz77";
    }
}