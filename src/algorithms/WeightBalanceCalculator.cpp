//
// Created by t-yabeny on 4/8/2020.
//

#include "WeightBalanceCalculator.h"

BalanceStatus WeightBalanceCalculator::tryOperation(char loadUnload, int kg, int X, int Y) {
    return APPROVED;
}

Cargo &WeightBalanceCalculator::getCargo() {
    return cargo;
}

void WeightBalanceCalculator::setCargo(Cargo &cargo) {

}

//void WeightBalanceCalculator::setCargo(Cargo &cargo) {
//    WeightBalanceCalculator::cargo = cargo;
//}
