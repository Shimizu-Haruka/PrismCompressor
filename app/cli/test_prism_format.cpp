#include"prism/format/prism_format.h"
#include"prism/algorithms/huffman.h"
#include<cassert>
#include<iostream>

void testPackUnpackEmpty()
{
    prism::ByteBuffer testData = {};
    prism::format::PrismHeader header;
    header.algorithm = 0;
    header.originalSize = 0;
    header.compressedSize = 0;
    header.checkSum = 0;

    prism::ByteBuffer payload;
    auto packed = prism::format::PrismFile::pack(header , payload);
    assert(packed.size() == 20);

        prism::format::PrismHeader outHeader;
    prism::ByteBuffer outPayload;

    bool ok = prism::format::PrismFile::unpack(packed, outHeader, outPayload);
    assert(ok);
    assert(outHeader.magic == 0x5052534D);
    assert(outHeader.version == 1);
    assert(outPayload.empty());

    std::cout << "test pack unpack empty passed" << std::endl;
}

void testUnpackBadMagic()
{
    prism::ByteBuffer data(20, 0);

    prism::format::PrismHeader header;
    prism::ByteBuffer payload;

    bool ok = prism::format::PrismFile::unpack(data, header, payload);
    assert(!ok);

    std::cout << "test unpack bad magic passed" << std::endl;
}

void testPackUnpackSimple()
{
    prism::format::PrismHeader header;
    header.algorithm = 2;
    header.originalSize = 100;
    header.compressedSize = 5;
    header.checkSum = 0x12345678;

    prism::ByteBuffer payload = {1, 2, 3, 4, 5};

    auto packed = prism::format::PrismFile::pack(header, payload);
    assert(packed.size() == 25);

    prism::format::PrismHeader outHeader;
    prism::ByteBuffer outPayload;

    bool ok = prism::format::PrismFile::unpack(packed, outHeader, outPayload);
    assert(ok);
    assert(outHeader.algorithm == 2);
    assert(outHeader.originalSize == 100);
    assert(outHeader.compressedSize == 5);
    assert(outHeader.checkSum == 0x12345678);
    assert(outPayload == payload);

    std::cout << "test pack unpack simple passed" << std::endl;
}

int main()
{
    testPackUnpackEmpty();
    testPackUnpackSimple();
    testUnpackBadMagic();
}