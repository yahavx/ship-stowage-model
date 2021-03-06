#include "AbstractWeightBalancer.h"
//
// Created by t-yabeny on 4/8/2020.
//

#include "NaiveWeightBalancer.h"

// region Constructors

NaiveWeightBalancer::NaiveWeightBalancer() {}

NaiveWeightBalancer::NaiveWeightBalancer(const ShipPlan &plan) : AbstractWeightBalancer(plan) {}

// endregion

// region Functions

WeightBalanceCalculator::BalanceStatus NaiveWeightBalancer::tryOperation(char loadUnload, int kg, int X, int Y) {
    return BalanceStatus::APPROVED;
    (void) loadUnload;
    (void) kg;
    (void) X;
    (void) Y;
}

// endregion