#pragma once

class BaseStrategy {
public:
    virtual ~BaseStrategy() = default;
    virtual void execute() = 0;
};
