#include"prism/format/prism_format.h"
#include<vector>

namespace prism::format{
namespace{

void writeUint16(ByteBuffer& buf , std::uint16_t value)//拆出2Byte
{
    buf.push_back(static_cast<Byte>(value & 0xff));
    buf.push_back(static_cast<Byte>((value >> 8) & 0xff));
}

void writeUint32(ByteBuffer& buf,std::uint32_t value)//拆出4Byte
{
    buf.push_back(static_cast<Byte>(value & 0xff));
    buf.push_back(static_cast<Byte>((value >>  8) & 0xff));
    buf.push_back(static_cast<Byte>((value >> 16) & 0xff));
    buf.push_back(static_cast<Byte>((value >> 24) & 0xff));
}

std::uint16_t readUint16(const ByteBuffer& buf, std::size_t& pos)
{
    std::uint16_t value = 
        static_cast<std::uint16_t>(buf[pos]) | 
        (static_cast<std::uint16_t>(buf[pos + 1] << 8));
        pos += 2;
    return value;
}

std::uint32_t readUint32(const ByteBuffer& buf, std::size_t& pos)
{
    std::uint32_t value = 
        static_cast<std::uint32_t>(buf[pos]) | 
        (static_cast<std::uint32_t>(buf[pos + 1] << 8)) |
        (static_cast<std::uint32_t>(buf[pos + 2] << 16)) |
        (static_cast<std::uint32_t>(buf[pos + 3] << 24));
    pos += 4;
    return value;
}

}

std::uint32_t PrismFile::computeChecksum(const ByteBuffer& data)
{
    std::uint32_t sum = 0;
    for(auto i : data)
    {
        sum = (sum << 5) - sum + i;
    }
    return sum;
}

ByteBuffer PrismFile::pack(const PrismHeader& header, const ByteBuffer& payload)
{
    ByteBuffer result;
    result.reserve(kHeaderSize + payload.size()); 

    writeUint32(result, header.magic);
    result.push_back(header.version);
    result.push_back(header.algorithm);
    writeUint16(result , header.reserved);
    writeUint32(result , header.originalSize);
    writeUint32(result , header.compressedSize);
    writeUint32(result , header.checkSum);

    result.insert(result.end(), payload.begin(), payload.end());

    return result;
}

bool PrismFile::unpack(const ByteBuffer& fileData, PrismHeader& header ,ByteBuffer& payload)
{
    if(fileData.size() < kHeaderSize)
    {
        return false;
    }

    std::size_t pos = 0;
    header.magic = readUint32(fileData , pos);
    header.version = fileData[pos++];
    header.algorithm = fileData[pos++];
    header.reserved = readUint16(fileData , pos);
    header.originalSize = readUint32(fileData , pos);
    header.compressedSize = readUint32(fileData , pos);
    header.checkSum = readUint32(fileData , pos);

    if(header.magic != 0x5052534D)
    {
        return false;
    }

    payload.assign(fileData.begin() + kHeaderSize , fileData.end());
    return true;
}
}