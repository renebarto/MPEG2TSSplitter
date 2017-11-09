#include <gtest/gtest.h>

#include "tools/Console.hpp"
#include "tools/Stopwatch.hpp"
#include "media/MPEG2Splitter.hpp"

using namespace std;

namespace Media {
namespace Test {

class MPEG2SplitterTest : public ::testing::Test
{
protected:
    virtual void SetUp() {}

    virtual void TearDown() {}
};

void AssertFilesEqual(const string & referencePath, const string & actualPath)
{
    ifstream referenceFile(referencePath, ios_base::in | ios_base::binary);
    ifstream actualFile(actualPath, ios_base::in | ios_base::binary);
    ASSERT_TRUE(referenceFile.good());
    ASSERT_TRUE(actualFile.good());
    bool done = false;
    constexpr size_t BufferSize = 512;
    char referenceBuffer[BufferSize];
    char actualBuffer[BufferSize];
    size_t offset = 0;
    while (!done)
    {
        referenceFile.read(referenceBuffer, sizeof(referenceBuffer));
        actualFile.read(actualBuffer, sizeof(actualBuffer));
        if (referenceFile.good())
        {
            if (!actualFile.good())
                FAIL();
            ASSERT_TRUE(actualFile.good());
            ASSERT_TRUE(memcmp(referenceBuffer, actualBuffer, sizeof(referenceBuffer)) == 0);
            offset += BufferSize;
        } else
        {
            ASSERT_EQ(referenceFile.gcount(), actualFile.gcount());
            ASSERT_TRUE(memcmp(referenceBuffer, actualBuffer, actualFile.gcount()) == 0);
            done = true;
        }
    }
}

TEST_F(MPEG2SplitterTest, TestRun)
{
    Tools::Stopwatch stopwatch;
    string outputVideoPath = string(TEST_DATA_ROOT) + "/elephants.ts.mp4";
    string outputAudioPath = string(TEST_DATA_ROOT) + "/elephants.ts.aac";
    unlink(outputAudioPath.c_str());
    unlink(outputVideoPath.c_str());
    stopwatch.Start();
    MPEG2Splitter splitter(string(TEST_DATA_ROOT) + "/elephants.ts");
    splitter.Run();
    stopwatch.Lap();
    AssertFilesEqual(string(TEST_DATA_ROOT) + "/elephants.ts.mp4.ref", outputVideoPath);
    AssertFilesEqual(string(TEST_DATA_ROOT) + "/elephants.ts.aac.ref", outputAudioPath);
    Tools::DefaultConsole() << "Elapsed time: " << stopwatch.GetElapsedTime() << " seconds" << endl;
}

} // namespace Test
} // namespace Media
