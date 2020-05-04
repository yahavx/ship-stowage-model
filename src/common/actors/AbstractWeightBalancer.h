//
// Created by t-yabeny on 5/4/2020.
//

#ifndef SHIP_STOWAGE_MODEL_ABSTRACTWEIGHTBALANCER_H
#define SHIP_STOWAGE_MODEL_ABSTRACTWEIGHTBALANCER_H

#include "../../interfaces/WeightBalanceCalculator.h"
#include "../data_objects/ShipPlan.h"
#include "../data_structures/Cargo.h"


class AbstractWeightBalancer : public WeightBalanceCalculator {  // our weight balancers will inherit this
protected:
    ShipPlan plan;
    Cargo *cargo;

    AbstractWeightBalancer();

    explicit AbstractWeightBalancer(const ShipPlan &plan);

public:

    // region Getters and setters

    Cargo &getCargo() const;

    const ShipPlan &getPlan() const;

    void setPlan(const ShipPlan &plan);

    void setCargo(Cargo &cargo);

    // endregion
};


#endif //SHIP_STOWAGE_MODEL_ABSTRACTWEIGHTBALANCER_H
