#pragma once

#include <aio.h>
#include <cstdio>

#include <string_view>

class FileHandle final
{
private:
    static constexpr auto INVALID_HANDLE = -1;
    int _file = INVALID_HANDLE;

public:
    FileHandle(std::string_view path, int flags);

    FileHandle(const FileHandle &) = delete;
    FileHandle &operator=(const FileHandle &) = delete;

    operator int();
    operator bool();

    void reset();

    ~FileHandle();
};
