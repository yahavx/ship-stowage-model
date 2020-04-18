//
// Created by t-yabeny on 4/8/2020.
//

#include "WeightBalanceCalculator.h"

WeightBalanceCalculator::WeightBalanceCalculator(const ShipPlan &plan) : plan(plan), cargo(plan){}

BalanceStatus WeightBalanceCalculator::tryOperation(char loadUnload, int kg, int X, int Y) {
    return BalanceStatus::APPROVED;
    std::cout << loadUnload << kg << X << Y;  // TODO: remove
}

Cargo &WeightBalanceCalculator::getCargo() {
    return cargo;
}

void WeightBalanceCalculator::setCargo(Cargo &cargo) {
    this->cargo = cargo;
}

//void WeightBalanceCalculator::setCargo(Cargo &cargo) {
//    WeightBalanceCalculator::cargo = cargo;
//}
