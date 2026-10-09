#include"prism/bitstream/bit_reader.h"
#include"prism/bitstream/bit_writer.h"

#include<cassert>
#include<iostream>

void testRoundTripFullByte()
{
    prism::bitstream::BitWriter writer_test_1;
    writer_test_1.writeBits(0b10001011 , 8);
    writer_test_1.finish();
    prism::ByteBuffer ByteBuffer_test_1;
    ByteBuffer_test_1 = writer_test_1.getData();

    prism::bitstream::BitReader reader_test_1(ByteBuffer_test_1 , 8);
    std::uint32_t result_1;
    bool is_read = reader_test_1.readBits(8 , result_1);

    assert(is_read);
    assert(result_1 == 0b10001011);

    std::cout << "pass" << std::endl;
}

void testRoundTripSingleBit()
{
    prism::bitstream::BitWriter writer_test_2;
    writer_test_2.writeBit(1);
    writer_test_2.finish();
    
    int result_2;
    prism::bitstream::BitReader reader_test_2(writer_test_2.getData() , 1);
    int is_read = reader_test_2.readBit(result_2);

    assert(is_read);
    assert(result_2 == 1);

    std::cout << "pass" << std::endl;
}

void testRoundTripPartialByte()
{
    prism::bitstream::BitWriter writer_test_3;
    writer_test_3.writeBits(0b101011 , 6);
    writer_test_3.finish();
    prism::ByteBuffer ByteBuffer_test_3;
    ByteBuffer_test_3 = writer_test_3.getData();

    prism::bitstream::BitReader reader_test_3(ByteBuffer_test_3 , 6);
    std::uint32_t result_3;
    bool is_read = reader_test_3.readBits(6 , result_3);

    assert(is_read);
    assert(result_3 == 0b101011);

    std::cout << "pass" << std::endl;
}

void testRoundTripDoubleByte()
{
    prism::bitstream::BitWriter writer_test_4;
    writer_test_4.writeBits(0b10001011 , 8);
    writer_test_4.writeBits(0b111110 , 6);
    writer_test_4.finish();
    prism::ByteBuffer ByteBuffer_test_4;
    ByteBuffer_test_4 = writer_test_4.getData();

    prism::bitstream::BitReader reader_test_4(ByteBuffer_test_4 , 14);
    std::uint32_t result_41;
    std::uint32_t result_42;

    bool is_read_1 = reader_test_4.readBits(8 , result_41);
    bool is_read_2 = reader_test_4.readBits(6 , result_42);

    assert(is_read_1);
    assert(is_read_2);
    assert(result_41 == 0b10001011);
    assert(result_42 == 0b111110);

    std::cout << "pass" << std::endl;
}

int main()
{
    testRoundTripFullByte();
    testRoundTripSingleBit();
    testRoundTripPartialByte();
    testRoundTripDoubleByte();  
}