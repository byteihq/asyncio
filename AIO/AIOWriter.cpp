#include "AIOWriter.hpp"

#include <fcntl.h>

AIOWriter::AIOWriter(int fd, Callable callable) : AIOWrapper(fd, LIO_WRITE, callable), _last_completed(true), _total_size(0)
{
    _aiolist[0] = &_aiocb;

    _aiocb.aio_offset = 0;

    _aiocb.aio_sigevent.sigev_value.sival_ptr = this;
    _aiocb.aio_sigevent.sigev_notify_function = &AIOWriter::callback;
}

void AIOWriter::callback(union sigval sv)
{
    auto aio = reinterpret_cast<AIOWriter *>(sv.sival_ptr);
    auto err = aio_error(&aio->_aiocb);
    if (err == 0)
    {
        aio->_aiocb.aio_offset += aio_return(&aio->_aiocb);
        aio->_total_size -= aio->_data.front().size();
        aio->_data.pop_front();
        aio->_last_completed = true;
        aio->_cv.notify_one();
    }
    aio->_last_err = err;
}

int AIOWriter::async_write(byte *buf, size_t sz)
{
    if (_data.empty())
        _data.emplace_back();

    _total_size += sz;
    std::copy(buf, buf + sz, std::back_inserter(_data.back()));
    _aiocb.aio_buf = _data.back().data();
    _aiocb.aio_nbytes = _data.back().size();
    return aio_write(&_aiocb);
}

int AIOWriter::write(byte *buf, size_t sz, bool eof)
{
    size_t total_write = 0;
    int res = 0;
    for (;;)
    {
        wait_async_call(_last_completed);
        if (_stop_requested)
            return 0;

        auto free_size = MAX_BUF_SZ - _total_size;
        if (free_size < sz - total_write)
        {
            res += async_write(buf + total_write, free_size);
            _last_completed = false;
            _data.emplace_back();
            total_write += free_size;
        }
        else
        {
            if (eof)
            {
                wait_async_call(_last_completed);
                if (_stop_requested)
                    return 0;
                res += async_write(buf + total_write, sz - total_write);
                _last_completed = false;
            }
            else
            {
                _total_size += sz - total_write;
                if (_data.empty())
                    _data.emplace_back();
                std::copy(buf + total_write, buf + sz, std::back_inserter(_data.back()));
            }
            break;
        }
    }

    return res;
}
