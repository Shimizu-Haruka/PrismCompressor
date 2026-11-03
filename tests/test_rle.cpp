#include"prism/algorithms/rle.h"

#include<iostream>
#include<string>

namespace {

int failures = 0;

void check(bool ok, const std::string& what)
{
    if(!ok)
    {
        std::cout << "[FAIL] " << what << std::endl;
        ++failures;
    }
}

prism::ByteBuffer repeatByte(prism::Byte value, std::size_t times)
{
    return prism::ByteBuffer(times, value);
}

// 先校验压缩后再解压与原数据逐字节一致，再校验压缩后字节数。
void verify(const prism::ByteBuffer& input,
            std::size_t expectedCompressedSize,
            const std::string& label)
{
    prism::algorithms::RLECompressor rle;
    const prism::ByteBuffer compressed = rle.compress(input);
    const prism::ByteBuffer restored = rle.decompress(compressed);

    bool sameSize = restored.size() == input.size();
    check(sameSize, label + "：解压后长度应与原数据一致");

    bool sameBytes = sameSize;
    for(std::size_t i = 0; sameBytes && i < input.size(); ++i)
    {
        sameBytes = restored[i] == input[i];
    }
    check(sameBytes, label + "：压缩再解压应逐字节一致");

    check(compressed.size() == expectedCompressedSize,
          label + "：压缩后应为 " + std::to_string(expectedCompressedSize) +
          " 字节，实际 " + std::to_string(compressed.size()) + " 字节");

    if(expectedCompressedSize < input.size())
    {
        check(compressed.size() < input.size(), label + "：可压缩数据压缩后应小于原长");
    }

    std::cout << "[INFO] " << label << " 输入=" << input.size()
              << " 压缩后=" << compressed.size() << std::endl;
}

}  // namespace

int main()
{
    verify({}, 0, "空输入");
    verify({'a'}, 2, "单字节");
    verify({'a', 'a', 'a', 'a'}, 2, "aaaa");
    verify({'a', 'b', 'a', 'b'}, 8, "abab");

    verify(repeatByte('a', 255), 2, "255 个重复字节");
    verify(repeatByte('a', 256), 4, "256 个重复字节");
    verify(repeatByte('a', 300), 4, "300 个重复字节");

    // 原有复杂用例：4 个 a + 3 个 b + 1000 个 X
    prism::ByteBuffer complexData{'a', 'a', 'a', 'a', 'b', 'b', 'b'};
    for(int i = 0; i < 1000; i++)
    {
        complexData.push_back('X');
    }
    // 4a 与 3b 各 1 段；1000 个 X 按 255 上限分 4 段，共 6 段、每段 2 字节
    verify(complexData, 12, "aaaa bbb + 1000 个 X");

    if(failures == 0)
    {
        std::cout << "test rle all pass" << std::endl;
        return 0;
    }

    std::cout << "test rle failed: " << failures << " check(s)" << std::endl;
    return 1;
}
