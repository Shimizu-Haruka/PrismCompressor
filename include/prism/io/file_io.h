#pragma once
#include"prism/core/types.h"
#include<string>

namespace prism::io{

class FileIO{
public:
    static ByteBuffer readFile(const std::string& path);//读文件
    static bool writeFile(const std::string& path , const ByteBuffer& data);//写文件
};
}