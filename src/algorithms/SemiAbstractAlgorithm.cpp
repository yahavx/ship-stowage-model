//
// Created by t-yabeny on 5/2/2020.
//

#include "SemiAbstractAlgorithm.h"
#include "../common/utils/Errors.h"
#include "../common/io/ObjectsReader.h"

int SemiAbstractAlgorithm::readShipPlan(const std::string &shipPlanPath) {
    Errors errors;
    auto shipPlan = readShipPlanFromFile(shipPlanPath, errors);
    this->ship.setShipPlan(shipPlan);

    if (errors.hasFatalError()) {
        this->algoErrors = errors.toErrorFlag();
    }

    else {
        this->algoErrors &= (~ErrorFlag::ShipPlan_FatalError);  // turn off the flag
    }

    return errors.toErrorFlag();
}

int SemiAbstractAlgorithm::readShipRoute(const std::string &shipRoutePath) {
    Errors errors;
    ShipRoute route = readShipRouteFromFile(shipRoutePath, errors);
    this->ship.setShipRoute(route);

    if (errors.hasFatalError()) {
        this->algoErrors = errors.toErrorFlag();
    }

    else {
        this->algoErrors &= (~ErrorFlag::ShipRoute_FatalError);  // turn off the flag
    }

    return errors.toErrorFlag();
}

int SemiAbstractAlgorithm::setWeightBalanceCalculator(NaiveWeightBalancer &calculator) {
    this->ship.setBalanceCalculator(calculator);
    this->ship.getBalanceCalculator().setPlan(this->ship.getShipPlan());
    return 0;  // TODO: this can fail?
}