#pragma once

#include "media/IDecoder.hpp"

namespace Media
{

class StreamWriter : public IDecoder
{
public:
    explicit StreamWriter(std::ostream & stream)
        : _stream(stream)
    {
    }

    virtual void FeedData(const uint8_t * data, size_t size) override;

private:
    std::ostream & _stream;
};

} // namespace Media