//
// Created by yahav on 06/04/2020.
//

#ifndef SHIP_STOWAGE_MODEL_NAIVEWEIGHTBALANCER_H
#define SHIP_STOWAGE_MODEL_NAIVEWEIGHTBALANCER_H

#include "../data_structures/Cargo.h"
#include "../../interfaces/WeightBalanceCalculator.h"

class NaiveWeightBalancer : public WeightBalanceCalculator {
    ShipPlan plan;
    Cargo *cargo;

public:
    // region Constructors

    NaiveWeightBalancer();

    NaiveWeightBalancer(const ShipPlan &plan);

    // endregion

    // region Getters and setters

    Cargo &getCargo() const;

    const ShipPlan &getPlan() const;

    void setPlan(const ShipPlan &plan);

    void setCargo(Cargo &cargo);

    // endregion

    // region Functions

    BalanceStatus tryOperation(char loadUnload, int kg, int X, int Y);

    // endregion
};



#endif //SHIP_STOWAGE_MODEL_NAIVEWEIGHTBALANCER_H
