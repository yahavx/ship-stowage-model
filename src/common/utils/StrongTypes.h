//
// Created by Orr on 6/7/2020.
//

#ifndef SHIP_STOWAGE_MODEL_STRONGTYPES_H
#define SHIP_STOWAGE_MODEL_STRONGTYPES_H

/// helper classes for named arguments

template<typename T>
class Named {
    T value;
public:
    explicit Named(T value): value{value} {}
    operator T() const {return value;}
};

class NumThreads: public Named<int> {
    using Named<int>::Named;
};

#endif //SHIP_STOWAGE_MODEL_STRONGTYPES_H
