#pragma once

#include "AIOWrapper.hpp"

#include <list>
#include <vector>

class AIOWriter final : public AIOWrapper
{
private:
    static void callback(union sigval sv);
    std::list<std::vector<byte>> _data;
    bool _last_complete;

private:
    int async_write(byte *buf, size_t sz);

public:
    AIOWriter(int fd, Callable callable = {});

    int write(byte *buf, size_t sz, bool eof);
};
