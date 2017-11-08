#include <gtest/gtest.h>

#include "tools/BitBuffer.hpp"

namespace Tools {
namespace Test {

class BitBufferTest : public ::testing::Test
{
protected:
    virtual void SetUp() {}

    virtual void TearDown() {}
};

TEST_F(BitBufferTest, Construct)
{
    std::vector<uint8_t> data;
    BitBuffer buffer(data.begin(), data.end());

    EXPECT_EQ(size_t {0}, buffer.BytesLeft());
    EXPECT_EQ(size_t {0}, buffer.BitsLeft());
}

TEST_F(BitBufferTest, ConstructData)
{
    std::vector<uint8_t> data({0x00});

    BitBuffer buffer(data.begin(), data.end());

    EXPECT_EQ(size_t {1}, buffer.BytesLeft());
    EXPECT_EQ(size_t {8}, buffer.BitsLeft());
}

TEST_F(BitBufferTest, ReadBit)
{
    std::vector<uint8_t> data({0xA5});

    BitBuffer buffer(data.begin(), data.end());

    EXPECT_EQ(size_t {1}, buffer.BytesLeft());
    EXPECT_EQ(size_t {8}, buffer.BitsLeft());

    EXPECT_EQ(uint8_t {1}, buffer.ReadBit());
    EXPECT_EQ(size_t {0}, buffer.BytesLeft());
    EXPECT_EQ(size_t {7}, buffer.BitsLeft());

    EXPECT_EQ(uint8_t {0}, buffer.ReadBit());
    EXPECT_EQ(size_t {0}, buffer.BytesLeft());
    EXPECT_EQ(size_t {6}, buffer.BitsLeft());

    EXPECT_EQ(uint8_t {1}, buffer.ReadBit());
    EXPECT_EQ(size_t {0}, buffer.BytesLeft());
    EXPECT_EQ(size_t {5}, buffer.BitsLeft());

    EXPECT_EQ(uint8_t {0}, buffer.ReadBit());
    EXPECT_EQ(size_t {0}, buffer.BytesLeft());
    EXPECT_EQ(size_t {4}, buffer.BitsLeft());

    EXPECT_EQ(uint8_t {0}, buffer.ReadBit());
    EXPECT_EQ(size_t {0}, buffer.BytesLeft());
    EXPECT_EQ(size_t {3}, buffer.BitsLeft());

    EXPECT_EQ(uint8_t {1}, buffer.ReadBit());
    EXPECT_EQ(size_t {0}, buffer.BytesLeft());
    EXPECT_EQ(size_t {2}, buffer.BitsLeft());

    EXPECT_EQ(uint8_t {0}, buffer.ReadBit());
    EXPECT_EQ(size_t {0}, buffer.BytesLeft());
    EXPECT_EQ(size_t {1}, buffer.BitsLeft());

    EXPECT_EQ(uint8_t {1}, buffer.ReadBit());
    EXPECT_EQ(size_t {0}, buffer.BytesLeft());
    EXPECT_EQ(size_t {0}, buffer.BitsLeft());

    EXPECT_THROW(buffer.ReadBit(), std::runtime_error);
}

TEST_F(BitBufferTest, ReadBits)
{
    std::vector<uint8_t> data({0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x55});

    BitBuffer buffer(data.begin(), data.end());

    EXPECT_EQ(size_t {9}, buffer.BytesLeft());
    EXPECT_EQ(size_t {72}, buffer.BitsLeft());

    EXPECT_EQ(uint64_t {1}, buffer.ReadBits(1));
    EXPECT_EQ(size_t {8}, buffer.BytesLeft());
    EXPECT_EQ(size_t {71}, buffer.BitsLeft());

    EXPECT_EQ(uint64_t {1}, buffer.ReadBits(2));
    EXPECT_EQ(size_t {8}, buffer.BytesLeft());
    EXPECT_EQ(size_t {69}, buffer.BitsLeft());

    EXPECT_EQ(uint64_t {2}, buffer.ReadBits(3));
    EXPECT_EQ(size_t {8}, buffer.BytesLeft());
    EXPECT_EQ(size_t {66}, buffer.BitsLeft());

    EXPECT_EQ(uint64_t {0xAA}, buffer.ReadBits(8));
    EXPECT_EQ(size_t {7}, buffer.BytesLeft());
    EXPECT_EQ(size_t {58}, buffer.BitsLeft());

    EXPECT_EQ(uint64_t {0xAAAA}, buffer.ReadBits(16));
    EXPECT_EQ(size_t {5}, buffer.BytesLeft());
    EXPECT_EQ(size_t {42}, buffer.BitsLeft());

    EXPECT_EQ(uint64_t {0xAAAAAAAA}, buffer.ReadBits(32));
    EXPECT_EQ(size_t {1}, buffer.BytesLeft());
    EXPECT_EQ(size_t {10}, buffer.BitsLeft());

    EXPECT_THROW(buffer.ReadBits(64), std::runtime_error);
    buffer.Reset();

    EXPECT_EQ(uint64_t {0xAAAAAAAAAAAAAAAA}, buffer.ReadBits(64));
    EXPECT_EQ(size_t {1}, buffer.BytesLeft());
    EXPECT_EQ(size_t {8}, buffer.BitsLeft());

    buffer.Reset();

    EXPECT_THROW(buffer.ReadBits(65), std::runtime_error);
}

TEST_F(BitBufferTest, ReadAheadBits)
{
    std::vector<uint8_t> data({0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x55});

    BitBuffer buffer(data.begin(), data.end());

    EXPECT_EQ(size_t {9}, buffer.BytesLeft());
    EXPECT_EQ(size_t {72}, buffer.BitsLeft());

    EXPECT_EQ(uint64_t {0x01}, buffer.ReadAheadBits(1));
    EXPECT_EQ(size_t {9}, buffer.BytesLeft());
    EXPECT_EQ(size_t {72}, buffer.BitsLeft());

    EXPECT_EQ(uint64_t {0x02}, buffer.ReadAheadBits(2));
    EXPECT_EQ(size_t {9}, buffer.BytesLeft());
    EXPECT_EQ(size_t {72}, buffer.BitsLeft());

    EXPECT_EQ(uint64_t {0xAAAAAAAAAAAAAAAA}, buffer.ReadBits(64));
    EXPECT_EQ(size_t {1}, buffer.BytesLeft());
    EXPECT_EQ(size_t {8}, buffer.BitsLeft());

    EXPECT_EQ(uint64_t {0x55}, buffer.ReadAheadBits(8));
    EXPECT_EQ(size_t {1}, buffer.BytesLeft());
    EXPECT_EQ(size_t {8}, buffer.BitsLeft());
}

TEST_F(BitBufferTest, SkipBytes)
{
    std::vector<uint8_t> data({0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x55});

    BitBuffer buffer(data.begin(), data.end());

    EXPECT_EQ(size_t {9}, buffer.BytesLeft());
    EXPECT_EQ(size_t {72}, buffer.BitsLeft());

    EXPECT_EQ(uint64_t {0x01}, buffer.ReadBits(1));
    EXPECT_EQ(size_t {8}, buffer.BytesLeft());
    EXPECT_EQ(size_t {71}, buffer.BitsLeft());

    buffer.SkipBytes(1);
    EXPECT_EQ(size_t {7}, buffer.BytesLeft());
    EXPECT_EQ(size_t {63}, buffer.BitsLeft());

    buffer.SkipBytes(2);
    EXPECT_EQ(size_t {5}, buffer.BytesLeft());
    EXPECT_EQ(size_t {47}, buffer.BitsLeft());

    EXPECT_THROW(buffer.SkipBytes(8), std::runtime_error);
}

} // namespace Test
} // namespace Tools
