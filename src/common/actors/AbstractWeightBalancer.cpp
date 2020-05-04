//
// Created by t-yabeny on 5/4/2020.
//

#include "AbstractWeightBalancer.h"

// region Constructors

AbstractWeightBalancer::AbstractWeightBalancer() {}

AbstractWeightBalancer::AbstractWeightBalancer(const ShipPlan &plan) : plan(plan) {}

// endregion

// region Getters and setters

Cargo &AbstractWeightBalancer::getCargo() const {
    return *cargo;
}

void AbstractWeightBalancer::setCargo(Cargo &cargo) {
    AbstractWeightBalancer::cargo = &cargo;
}

const ShipPlan &AbstractWeightBalancer::getPlan() const {
    return plan;
}

void AbstractWeightBalancer::setPlan(const ShipPlan &plan) {
    AbstractWeightBalancer::plan = plan;
}

// endregion