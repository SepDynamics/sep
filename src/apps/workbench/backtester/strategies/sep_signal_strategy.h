#pragma once

#include "base_strategy.h"

class SEPSignalStrategy : public BaseStrategy {
public:
    SEPSignalStrategy();
    ~SEPSignalStrategy();

    void execute() override;
};
