#include "AIOReader.hpp"

AIOReader::AIOReader(int fd, int offset, uint64_t max_read, Callable callable) : AIOWrapper(fd, LIO_READ, callable), _total_read(0), _max_read(max_read)
{
    _aiocb.aio_buf = _block_buf;
    _aiocb.aio_nbytes = BLOCK_SZ;
    _aiocb.aio_offset = offset;

    _aiocb.aio_sigevent.sigev_value.sival_ptr = this;
    _aiocb.aio_sigevent.sigev_notify_function = &AIOReader::callback;
}

void AIOReader::callback(union sigval sv)
{
    auto aio = reinterpret_cast<AIOReader *>(sv.sival_ptr);
    auto err = aio_error(&aio->_aiocb);
    aio->_last_err = err;
    if (err == 0)
    {
        auto sz = aio_return(&aio->_aiocb);
        aio->_total_read += sz;
        aio->_aiocb.aio_offset += sz;
        if (aio->_callable)
            aio->_callable(aio->_block_buf, sz, sz != BLOCK_SZ);
        if (sz == BLOCK_SZ)
            aio->read();
    }
}

int AIOReader::read()
{
    if (_max_read != 0 && _total_read >= _max_read || _stop_requested)
        return -1;
    return aio_read(&_aiocb);
}
