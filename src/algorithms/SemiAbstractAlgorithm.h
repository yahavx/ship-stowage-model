//
// Created by t-yabeny on 5/2/2020.
//

#ifndef SHIP_STOWAGE_MODEL_SEMIABSTRACTALGORITHM_H
#define SHIP_STOWAGE_MODEL_SEMIABSTRACTALGORITHM_H

#include "../interfaces/AbstractAlgorithm.h"
#include "../common/actors/ContainerShip.h"
#include "../common/utils/Errors.h"
#include "../common/data_objects/Operations.h"


class SemiAbstractAlgorithm : public AbstractAlgorithm {  // implements the basics that each algorithm use - all will inherit from this
protected:
    ContainerShip ship;
    int algoErrors = ErrorFlag::ShipPlan_FatalError_NoFileOrInvalidFirstLine | ErrorFlag::ShipRoute_FatalError_NoFileOrNoLegalPorts;  // we remove the flags after they are set

public:

    int readShipPlan(const std::string &shipPlanPath) override;

    int readShipRoute(const std::string &shipRoutePath) override;

    int setWeightBalanceCalculator(WeightBalanceCalculator &calculator) override;

    /**
     * This method is implemented here, and is same for all inheriting algorithms (kind of general common template).
     * It uses generateOperations (see below), which is implemented differently at each algorithm, and causes them to behave differently.
     */
    int getInstructionsForCargo(const std::string& input_full_path_and_file_name, const std::string& output_full_path_and_file_name) override;

protected:

    /**
    * Receives the ship, next id of port to dock into, and list of containers that needs to be loaded.
    * @return list of operations: first unload all containers for this port, and than load all the containers that needs to be loaded.
    */
    virtual Operations generateOperations(ContainerShip &ship, Port &port, const Containers &containersToLoad, Errors &errors) = 0;

    /**
     * Collect all containers that needs to be loaded from a port, according to the next ports in the route.
     * @param port port to load from.
     * @return list of containers, to load to the ship.
     */
    virtual Containers getContainersToLoad(Port &port);

    bool hasFatalError();
};


#endif //SHIP_STOWAGE_MODEL_SEMIABSTRACTALGORITHM_H
