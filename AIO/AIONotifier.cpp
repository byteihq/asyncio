#include "AIONotifier.hpp"

AIONotifier::AIONotifier(std::initializer_list<AIOWrapper *> aios)
{
    for (auto aio : aios)
    {
        if (aio)
            _aio_handles.push_back(aio);
    }
}

void AIONotifier::push(AIOWrapper *aio)
{
    if (!aio)
        return;
    
    _aio_handles.push_back(aio);
}

void AIONotifier::stop_all()
{
    for (auto aio : _aio_handles)
        aio->stop();
}

void AIONotifier::pop()
{
    if (!_aio_handles.empty())
        _aio_handles.pop_back();
}
