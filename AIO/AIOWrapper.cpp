#include "AIOWrapper.hpp"

AIOWrapper::AIOWrapper(int fd, int op, Callable callable) : _last_err(0), _callable(callable)
{
    _aiocb.aio_fildes = fd;
    _aiocb.aio_reqprio = 0;
    _aiocb.aio_lio_opcode = op;

    _aiocb.aio_sigevent.sigev_notify = SIGEV_THREAD;
    _aiocb.aio_sigevent.sigev_notify_attributes = nullptr;
}

int AIOWrapper::get_last_error() const noexcept
{
    return _last_err;
}

AIOWrapper::~AIOWrapper()
{
    aio_cancel(_aiocb.aio_fildes, &_aiocb);
}
