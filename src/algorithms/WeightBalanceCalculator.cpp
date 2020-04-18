//
// Created by t-yabeny on 4/8/2020.
//

#include "WeightBalanceCalculator.h"

// region Constructors

WeightBalanceCalculator::WeightBalanceCalculator(const ShipPlan &plan) : plan(plan) {}

WeightBalanceCalculator::WeightBalanceCalculator() {}
// endregion

// region Functions

BalanceStatus WeightBalanceCalculator::tryOperation(Cargo &cargo, char loadUnload, int kg, int X, int Y) {
    return BalanceStatus::APPROVED;
    std::cout << loadUnload << kg << X << Y << cargo;  // TODO: remove
}
// endregion