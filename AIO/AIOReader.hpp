#pragma once

#include "AIOWrapper.hpp"

class AIOReader final : public AIOWrapper
{
private:
    static void callback(union sigval sv);

private:
    static constexpr auto BLOCK_SZ = 1024;
    byte _block_buf[BLOCK_SZ];
    uint64_t _total_read;
    uint64_t _max_read;

public:
    AIOReader(int fd, int offset = 0, uint64_t max_read = 0, Callable callable = {});

    int read();
};
