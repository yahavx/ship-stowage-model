//
// Created by t-yabeny on 5/2/2020.
//

#include "SemiAbstractAlgorithm.h"
#include "../../common/utils/ErrorFlags.h"
#include "../../common/io/ObjectsReader.h"

int SemiAbstractAlgorithm::readShipPlan(const std::string &shipPlanPath) {
    std::vector<ErrorFlag> errors;
    auto shipPlan = readShipPlanFromFile(shipPlanPath, errors);
    this->ship.setShipPlan(shipPlan);
    int flags = errorsVectorToErrorsFlag(errors);

    if (containsFatalError(flags)) {
        this->fatalError = flags;
    }

    return flags;
}

int SemiAbstractAlgorithm::readShipRoute(const std::string &shipRoutePath) {
    std::vector<ErrorFlag> errors;
    ShipRoute route = readShipRouteFromFile(shipRoutePath, errors);
    this->ship.setShipRoute(route);
    int flags = errorsVectorToErrorsFlag(errors);

    if (containsFatalError(flags)) {
        this->fatalError = flags;
    }

    return flags;
}

int SemiAbstractAlgorithm::setWeightBalanceCalculator(WeightBalanceCalculator &calculator) {
    this->ship.setBalanceCalculator(calculator);
    this->ship.getBalanceCalculator().setPlan(this->ship.getShipPlan());
    return 0;  // TODO: this can fail?
}
