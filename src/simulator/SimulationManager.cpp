//
// Created by t-yabeny on 5/16/2020.
//

#include "SimulationManager.h"
#include "../common/io/ObjectsReader.h"
#include "SimulatorUtil.h"
#include "AlgorithmValidation.h"
#include "../common/actors/CranesManagement.h"
#include "../common/utils/Printers.h"
#include "../common/utils/UtilFunctions.h"

// region Constructor

SimulationManager::SimulationManager(SimulatorFileManager &manager, Tracer &tracer) : fileManager(manager),  errors(tracer),  totalNumberOfOps(0), tracer(tracer) {}

// endregion

// region Init

void SimulationManager::initSimulationShip(WeightBalanceCalculator &calculator) {
    tracer.traceVerbose("Initializing simulator ship...");
    ShipPlan shipPlan = readShipPlanFromFile(fileManager.shipPlanPath(), errors);
    ShipRoute shipRoute = readShipRouteFromFile(fileManager.shipRoutePath(), errors);

    if (errors.hasFatalError()) {
        tracer.traceInfo("Simulation ship failed to initialize.");  // this shouldn't happen (travel should be skipped)
        return;
    }

    tracer.traceVerbose("Ship Plan and Route:");
    tracer.traceVerbose(genericToString(shipPlan));
    tracer.traceVerbose(genericToString(shipRoute), true);

    ship = ContainerShip(shipPlan, shipRoute, calculator);
}

int SimulationManager::initAlgorithmShip(AbstractAlgorithm *algorithm, WeightBalanceCalculator &calculator) {
    int ret = algorithm->readShipPlan(fileManager.shipPlanPath());
    int ret2 = algorithm->readShipRoute(fileManager.shipRoutePath());
    int ret3 = algorithm->setWeightBalanceCalculator(calculator);

    // TODO: below we check if algorithm report matches to what we except, and penalise him if yes. Decided to abandon it for this exercise (risky).
//    int errorsDiff = errors.compareReports(algorithmErrors);
//
//    if (errorsDiff > 0) {
//        errors.addError({ErrorFlag::AlgorithmError_MissingReport, intToStr(errorsDiff)});
//    }
//
//    if (errorsDiff < 0) {
//        errors.addError({ErrorFlag::AlgorithmError_ExtraReport, intToStr(-errorsDiff)});
//    }

    algorithmReport = ret | ret2 | ret3;

    bool algorithmError = Error(algorithmReport).isFatalError();

    if (algorithmError) { // failed to initialize
        errors.addError(ErrorFlag::AlgorithmError_FailedToInitialize);
    }

    return !algorithmError;  // true indicates success
}

void SimulationManager::initCargoData() {
    cargoData = fileManager.getCargoDataFiles(errors);  // get list of .cargo_data files, ordered for each port
    initPortsVisits(ship.getShipRoute());
    filterUnusedPorts();  // remove the port files which are not on the ship route

    addInitReport();  // initCargoData happens last in the init
}

void SimulationManager::initPortsVisits(ShipRoute &shipRoute) {
    for (PortId &port: shipRoute.getPorts()) {
        portsVisits[port] = 0;  // we may do it for the same port twice - no problem with that (one copy will be overwritten)
    }
}

void SimulationManager::filterUnusedPorts() {
    StringVector toErase;

    for (auto &entry: cargoData) {
        std::string currPortCode = entry.first;  // get a port id

        bool found = false;  // indicates if we found this port in the route
        for (const PortId &portId : ship.getShipRoute().getPorts()) {
            if (portId.getCode() == currPortCode) {
                found = true;
                break;
            }
        }

        if (!found) {  // port is not in the route, remove it
            tracer.traceVerbose("Warning: port " + currPortCode + " has cargo files but doesn't appear in the route, ignoring");
            errors.addError({ErrorFlag::Travel_CargoData_PortNotInRoute, currPortCode});
            toErase.push_back(currPortCode);  // we don't erase in-place because it will crash the map iterator
        }
    }

    for (std::string &port : toErase) {
        cargoData.erase(port);
    }
}

void SimulationManager::addInitReport() {
    longUInt simulationReport = errors.toErrorFlag(false, true);
    if (simulationReport != 0) {
        errors.addErrorReport(simulationReport, "Simulator");
    }

    if (algorithmReport != 0) {
        errors.addErrorReport(algorithmReport, "Algorithm");
    }

    errors.addSimulationInitLog();
}
// endregion

// region Setter

void SimulationManager::setTotalNumberOfOps(int totalNumberOfOps) {
    if (SimulationManager::totalNumberOfOps == -1) {  // If the algorithm is shit there is no way back
        return;
    }
    SimulationManager::totalNumberOfOps = totalNumberOfOps;
}

// endregion

// region Simulation

std::vector<PortId> SimulationManager::getRoutePorts() {
    auto ports = ship.getShipRoute().getPorts();  // return a copy
    return ports;
}

int SimulationManager::currentPortVisitNum(bool increment) {
    if (increment)
        return ++portsVisits[ship.getCurrentPortId()];
    return portsVisits[ship.getCurrentPortId()];
}

bool SimulationManager::isRouteFinished() {
    return ship.getShipRoute().getPorts().empty();
}

bool SimulationManager::isCurrentLastPort() {
    return ship.getShipRoute().getPorts().size() == 1;
}

std::string SimulationManager::getNextFileForPort() {
    PortId portId = ship.getCurrentPortId();
    StringVector &filesForPort = cargoData[portId];

    int portVisitNum = currentPortVisitNum(true);

    if (!filesForPort.empty()) {
        std::string portCargoFile = filesForPort[0];  // retrieve cargo_data with smallest index

        if (extractNumberFromCargoFile(portCargoFile) == portVisitNum) {  // it matches the port visit number, pop it and return
            filesForPort.erase(filesForPort.begin());
            return fileManager.cargoFilePath(portCargoFile);
        }
    }

    // there is no matching cargo_data file, so we will generate an empty one
    std::string filePath = fileManager.createCargoDataTempFilePath(portId);
    return filePath;
}

std::string SimulationManager::getInstructionsForCargo(AbstractAlgorithm *algorithm) {
    std::string cargoDataFile = this->getNextFileForPort();
    this->initPort(cargoDataFile);  // init self port

    std::string instructionsOutputPath = fileManager.craneInstructionsOutputPath(currentPort.getId(), this->currentPortVisitNum());
    this->algorithmReport = algorithm->getInstructionsForCargo(cargoDataFile, instructionsOutputPath);

    return instructionsOutputPath;
}

void SimulationManager::initPort(const std::string &cargoDataPath) {
    auto portId = ship.getCurrentPortId();
    auto storage = readPortCargoFromFile(cargoDataPath, errors);
    currentPort = Port(portId, storage);

    if (isCurrentLastPort() && !storage.isEmpty()) { // Last port has containers
        errors.addError({ErrorFlag::ContainersAtPort_LastPortHasContainers});
    }
}

bool SimulationManager::performPackingOperations(const std::string &operationsPath) { // Perform operations on local ship and port
    Operations ops = readPackingOperationsFromFile(operationsPath, errors);

    tracer.traceInfo("Operations from the algorithm:");
    tracer.traceInfo(genericToString(ops));

    if (errors.hasAlgorithmErrors()) {
        tracer.traceInfo("The operations contains an invalid line.");
        errors.addSimulationPortVisitLog(currentPortVisitNum(), ship.getCurrentPortId(), ++portsVisited);
        reportSimulationError();
        return false;
    }

    StringVector badContainers;
    if (!isCurrentLastPort()) {
        // Removes invalid containers from port (invalid format, etc)
        badContainers = currentPort.removeBadContainers(errors);

        // Remove invalid containers from port, according to ship (not on route, duplicate ID)
        StringVector moreBadContainers = ship.filterContainers(currentPort.getStorage().getContainers(), errors);
        currentPort.removeContainers(moreBadContainers);
        badContainers.insert(badContainers.begin(), moreBadContainers.begin(), moreBadContainers.end());
    }

    AlgorithmValidation validation(ship, currentPort, badContainers, errors);
    CranesManagement crane(ship, currentPort);

    for (const PackingOperation &op : ops.ops) {

        if (!validation.validatePackingOperation(op)) {
            tracer.traceInfo("Operation '" + op.toString() + "' is illegal");
            errors.addSimulationPortVisitLog(currentPortVisitNum(), ship.getCurrentPortId(), ++portsVisited);
            reportSimulationError();
            return false;
        }

        if (op.getType() == PackingType::reject)
            continue;

        auto opResult = crane.preformOperation(op);
        checkCraneResult(op, opResult);
    }

    bool allRelevantContainersLoaded = validation.validateNoContainersLeftOnPort();
    bool allRelevantContainersUnloaded = validation.validateNoContainersLeftOnShip();

    addPortErrorReport();  // and simulation and algorithm reports summary, if needed

    if (!allRelevantContainersLoaded) {
        tracer.traceInfo("A good container wasn't loaded from the port, while the ship isn't full.");
        reportSimulationError();
        return false;
    }

    if (!allRelevantContainersUnloaded) {
        tracer.traceInfo("A container that his destination is this port, wasn't unloaded from the ship.");
    }

    ship.advanceToNextPort();

    this->setTotalNumberOfOps(totalNumberOfOps + ops.size(true));
    return true;
}

void SimulationManager::addPortErrorReport() {
    longUInt simulationReport = errors.toErrorFlag(false, true);
    if (simulationReport != 0) {
        errors.addErrorReport(simulationReport, "Simulator");
    }

    if (algorithmReport != 0) {
        errors.addErrorReport(algorithmReport, "Algorithm");
    }

    errors.addSimulationPortVisitLog(currentPortVisitNum(), ship.getCurrentPortId(), ++portsVisited);
}

void SimulationManager::checkCraneResult(const PackingOperation &op, CraneOperationResult opResult) {
    if (opResult == CraneOperationResult::FAIL_CONTAINER_NOT_FOUND) {
        tracer.traceInfo("Crane received illegal operation, didn't find container with ID: " + op.getContainerId());
        errors.addError({ErrorFlag::AlgorithmError_CraneOperationWithInvalidId, op.getContainerId(), currentPort.getId(), op.toString()});
    }
    if (opResult == CraneOperationResult::FAIL_ILLEGAL_OP) {
        tracer.traceInfo("Illegal crane operation: " + op.toString());
        errors.addError({ErrorFlag::AlgorithmError_InvalidCraneOperation, op.toString()});
    }
}
// endregion

// region Finish

void SimulationManager::reportSimulationError() {
    tracer.traceInfo("Found an error in the algorithm, terminating");
    tracer.separator(TraceVerbosity::Info, 1, 3);

    setTotalNumberOfOps(-1);
    errors.addSimulationErrorLog();
//    fileManager.saveSimulationErrors(errors);
}

int SimulationManager::finishSimulation() {
    this->validateNoCargoFilesLeft();

    if (!ship.getCargo().isEmpty()) {
        tracer.traceInfo("The travel is finished, but the ship is not empty (algorithm error)", true);
        errors.addError(AlgorithmError_ShipNotEmptyAtEndOfRoute);
        setTotalNumberOfOps(-1);
    }

    errors.addSimulationFinishLog();
    saveErrors();
    return totalNumberOfOps;
}

void SimulationManager::saveErrors() {
    if (errors.hasErrors()) {
        fileManager.saveSimulationErrors(errors);
    }
}

void SimulationManager::validateNoCargoFilesLeft() {
    for (auto &entry: cargoData) {
        std::string portId = entry.first;
        if (!cargoData[portId].empty()) {
            tracer.traceVerbose("Warning: finished the route, but port " + portId + " have cargo files that were not used");
            errors.addError({ErrorFlag::Travel_CargoData_RemainingFilesAfterFinish, portId, intToStr(cargoData[portId].size())});
        }
    }
}

// endregion