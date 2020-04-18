//
// Created by t-yabeny on 4/8/2020.
//

#include "WeightBalanceCalculator.h"

// region Constructors

WeightBalanceCalculator::WeightBalanceCalculator(const ShipPlan &plan) : plan(plan) {}

WeightBalanceCalculator::WeightBalanceCalculator() {}
// endregion

// region Getters and setters

Cargo &WeightBalanceCalculator::getCargo() const {
    return *cargo;
}

void WeightBalanceCalculator::setCargo(Cargo &cargo) {
    WeightBalanceCalculator::cargo = &cargo;
}
// endregion

// region Functions

BalanceStatus WeightBalanceCalculator::tryOperation(char loadUnload, int kg, int X, int Y) {
    return BalanceStatus::APPROVED;
    std::cout << loadUnload << kg << X << Y << cargo;  // TODO: remove
}

// endregion