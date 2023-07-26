//
// Created by zyzshishui on 23-7-22.
//

#include "IdGenerator.h"

IdGenerator::IdGenerator()
        : id(0L) {}

IdGenerator::IdGenerator(long id)
        : id(id) {}


long IdGenerator::nextId() {
    this->id++;
    return this->id;
}

