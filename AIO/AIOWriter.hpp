#pragma once

#include "AIOWrapper.hpp"

#include <list>
#include <vector>
#include <condition_variable>
#include <mutex>

class AIOWriter final : public AIOWrapper
{
private:
    static constexpr auto MAX_BUF_SZ = 1024 * 1024; // 1 Mb

private:
    static void callback(union sigval sv);
    std::list<std::vector<byte>> _data;
    bool _last_completed;
    size_t _total_size;
    aiocb *_aiolist[1];

private:
    int async_write(byte *buf, size_t sz);

public:
    AIOWriter(int fd, Callable callable = {});

    int write(byte *buf, size_t sz, bool eof);
};
