#include"test_roundtrip.cpp"
#include<iostream>

int main()
{
    testRoundTripFullByte();
    testRoundTripSingleBit();
    testRoundTripPartialByte();
    testRoundTripDoubleByte();
    return 0;
}