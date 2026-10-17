#include"prism/io/file_io.h"
#include<fstream>

namespace prism::io{
ByteBuffer FileIO::readFile(const std::string& path){
    std::ifstream file(path , std::ios::binary);//输入文件流 二进制模式

    if(!file.is_open())
    {
        return {};
    }

    file.seekg(0 , std::ios::end);//移到末尾
    std::streamsize size = file.tellg();//获取位置 = 文件大小
    file.seekg(0 , std::ios::beg);//回到开头

    if(size < 0)
    {
        return {};
    }

    ByteBuffer result(static_cast<std::size_t>(size));
    file.read(reinterpret_cast<char*>(result.data()), size);//*uint8_t强转char，read需要char

    if(!file)
    {
        return {};
    }
    return result;
}

bool FileIO::writeFile(const std::string& path , const ByteBuffer& data)
{
    std::ofstream file(path , std::ios::binary);
    if(!file.is_open())
    {
        return false;
    }
    file.write(reinterpret_cast<const char*>(data.data()),
               static_cast<std::streamsize>(data.size()));//写入所有字节

    return file.good();//文件完好
}
}