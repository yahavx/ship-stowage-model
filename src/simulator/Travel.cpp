//
// Created by t-yabeny on 6/10/2020.
//

#include "Travel.h"


// region Constructor

Travel::Travel(const std::string &travelPath, ShipPlan &plan, ShipRoute &route) : travelPath(travelPath), plan(std::move(plan)), route(std::move(route)){

}

// endregion

// region API

ShipPlan Travel::getPlanCopy() const {
    return plan;
}

ShipRoute Travel::getRouteCopy() const {
    return route;
}

// endregion
