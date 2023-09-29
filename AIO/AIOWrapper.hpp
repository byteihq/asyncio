#pragma once

#include <cstdint>
#include <aio.h>
#include <signal.h>

#include <vector>
#include <functional>
#include <mutex>
#include <condition_variable>

class AIOWrapper
{
public:
    using byte = uint8_t;
    using Callable = std::function<void(byte *, size_t, bool)>;

protected:
    aiocb _aiocb;
    int _last_err;
    Callable _callable;
    bool _stop_requested;
    mutable std::condition_variable _cv;
    mutable std::mutex _m;

protected:
    AIOWrapper(int fd, int op, Callable callable);

    int get_last_error() const noexcept;

    void wait_async_call(bool &expression);

    virtual ~AIOWrapper();

public:
    void stop();
};
