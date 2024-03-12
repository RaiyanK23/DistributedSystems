#pragma once

#include <string>

class ServerInstance {
public:
    virtual void acceptConnection() = 0;
};
