#include <gtest/gtest.h>

#include "BitBuffer.hpp"

namespace Media {
namespace Test {

class BitBufferTest : public ::testing::Test
{
protected:
    virtual void SetUp() {}

    virtual void TearDown() {}
};

TEST_F(BitBufferTest, Construct)
{
    BitBuffer buffer;

    EXPECT_EQ(size_t {0}, buffer.BytesLeft());
    EXPECT_EQ(size_t {0}, buffer.BitsLeft());
}

TEST_F(BitBufferTest, SetData)
{
    std::vector<uint8_t> data({0x00});

    BitBuffer buffer;

    buffer.SetData(data);

    EXPECT_EQ(size_t {1}, buffer.BytesLeft());
    EXPECT_EQ(size_t {8}, buffer.BitsLeft());
}

TEST_F(BitBufferTest, ReadBit)
{
    std::vector<uint8_t> data({0xA5});

    BitBuffer buffer;

    buffer.SetData(data);

    EXPECT_EQ(size_t {1}, buffer.BytesLeft());
    EXPECT_EQ(size_t {8}, buffer.BitsLeft());

    EXPECT_EQ(uint8_t {1}, buffer.ReadBit());
    EXPECT_EQ(size_t {0}, buffer.BytesLeft());
    EXPECT_EQ(size_t {7}, buffer.BitsLeft());
}


} // namespace Test
} // namespace Media
