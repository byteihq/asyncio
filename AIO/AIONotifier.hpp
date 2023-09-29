#pragma once

#include "AIOWrapper.hpp"
#include <vector>

class AIONotifier final
{
private:
    std::vector<AIOWrapper *> _aio_handles;

public:
    AIONotifier() = default;
    AIONotifier(std::initializer_list<AIOWrapper *> aios);

    void push(AIOWrapper *aio);

    void stop_all();

    void pop();

    ~AIONotifier() = default;
};
