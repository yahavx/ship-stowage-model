//
// Created by Orr on 5/6/2020.
//

#ifndef SHIP_STOWAGE_MODEL_ROBUSTSTOWAGEALGORITHM_H
#define SHIP_STOWAGE_MODEL_ROBUSTSTOWAGEALGORITHM_H

#include "SemiAbstractAlgorithm.h"


class RobustStowageAlgorithm : public SemiAbstractAlgorithm {

public:

#ifndef RUNNING_ON_NOVA
    std::string getAlgorithmName() override;
#endif

private:

    Operations generateOperations(ContainerShip &ship, Port &port, const Containers &containersToLoad) override ;

    Containers getContainersToLoad(Port &port) override;
};


#endif //SHIP_STOWAGE_MODEL_ROBUSTSTOWAGEALGORITHM_H
