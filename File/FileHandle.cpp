#include "FileHandle.hpp"

#include <unistd.h>
#include <fcntl.h>

FileHandle::FileHandle(std::string_view path, int flags) : _file(open(path.data(), flags))
{
}

FileHandle::operator int()
{
    return _file;
}

FileHandle::operator bool()
{
    return _file != INVALID_HANDLE;
}

void FileHandle::reset()
{
    if (_file != INVALID_HANDLE)
    {
        close(_file);
    }
}

FileHandle::~FileHandle()
{
    reset();
}
