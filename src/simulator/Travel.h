//
// Created by t-yabeny on 6/10/2020.
//

#ifndef SHIP_STOWAGE_MODEL_TRAVEL_H
#define SHIP_STOWAGE_MODEL_TRAVEL_H


#include "../common/data_objects/ShipRoute.h"
#include "../common/data_objects/ShipPlan.h"

/// Defines a single travel.
class Travel {
    std::string travelPath;
    ShipPlan plan;
    ShipRoute route;

public:

    // region Constructor

    Travel(const std::string &travelPath, ShipPlan &plan, ShipRoute &route);

    // endregion

    // region API

    /// Returns a copy of the ship plan.
    ShipPlan getPlanCopy() const;

    /// Returns a copy of the ship route.
    ShipRoute getRouteCopy() const;

    // endregion

    // region Operator

     operator std::string() {
        return travelPath;
    }

    // endregion
};


#endif //SHIP_STOWAGE_MODEL_TRAVEL_H
