#include "AIOWriter.hpp"

#include <fcntl.h>

AIOWriter::AIOWriter(int fd, Callable callable) : AIOWrapper(fd, LIO_WRITE, callable), _last_complete(true)
{
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
        aio->_data.pop_front();
        aio->_last_complete = true;
    }
    aio->_last_err = err;
}

int AIOWriter::async_write(byte *buf, size_t sz)
{
    if (_data.empty())
        _data.push_back({});
    std::copy(buf, buf + sz, std::back_inserter(_data.back()));
    _aiocb.aio_buf = _data.back().data();
    _aiocb.aio_nbytes = _data.back().size();
    return aio_write(&_aiocb);
}

int AIOWriter::write(byte *buf, size_t sz, bool eof)
{
    if (eof)
    {
        aiocb *aiolist[1];
        aiolist[0] = &_aiocb;
        aio_suspend(aiolist, 1, nullptr);
        return async_write(buf, sz);
    }
    
    if (_last_complete)
    {
        int res = async_write(buf, sz);
        _last_complete = false;
        _data.push_back({});
        return res;
    }

    std::copy(buf, buf + sz, std::back_inserter(_data.back()));
    return 0;
}
