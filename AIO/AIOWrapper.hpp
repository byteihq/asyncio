#pragma once

#include <cstdint>
#include <aio.h>
#include <signal.h>

#include <vector>
#include <functional>

class AIOWrapper
{
public:
    using byte = uint8_t;
    using Callable = std::function<void(byte *, size_t, bool)>;

protected:
    aiocb _aiocb;
    int _last_err;
    Callable _callable;

protected:
    AIOWrapper(int fd, int op, Callable callable);

    int get_last_error() const noexcept;

    virtual ~AIOWrapper();
};
