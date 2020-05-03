//
// Created by t-yabeny on 4/8/2020.
//

#include "NaiveWeightBalancer.h"

// region Constructors

NaiveWeightBalancer::NaiveWeightBalancer(const ShipPlan &plan) : plan(plan) {}

NaiveWeightBalancer::NaiveWeightBalancer() {}

// endregion

// region Getters and setters

Cargo &NaiveWeightBalancer::getCargo() const {
    return *cargo;
}

void NaiveWeightBalancer::setCargo(Cargo &cargo) {
    NaiveWeightBalancer::cargo = &cargo;
}

const ShipPlan &NaiveWeightBalancer::getPlan() const {
    return plan;
}

void NaiveWeightBalancer::setPlan(const ShipPlan &plan) {
    NaiveWeightBalancer::plan = plan;
}

// endregion

// region Functions

BalanceStatus NaiveWeightBalancer::tryOperation(char loadUnload, int kg, int X, int Y) {
    return BalanceStatus::APPROVED;
    std::cout << loadUnload << kg << X << Y << cargo;  // TODO: remove
}

// endregion