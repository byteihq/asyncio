#include "AIOWrapper.hpp"

AIOWrapper::AIOWrapper(int fd, int op, Callable callable) : _last_err(0), _stop_requested(false), _callable(callable)
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

void AIOWrapper::wait_async_call(bool &expression)
{
    if (expression)
        return;

    std::unique_lock<std::mutex> lk(_m);
    _cv.wait(lk, [this, &expression]()
             { return expression || _stop_requested; });
}

void AIOWrapper::stop()
{
    _stop_requested = true;
    _cv.notify_one();
}

AIOWrapper::~AIOWrapper()
{
    aio_cancel(_aiocb.aio_fildes, &_aiocb);
}
