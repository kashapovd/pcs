//
// Created by yaroslav on 10/5/21.
//

#include "cell.h"

cell::cell(bool alive) {
    state = alive ? ALIVED : KILLED;
    _age = 0;
}

void cell::die() {
    state = KILLED;
}

void cell::resurrect() {
    state = ALIVED;
}

bool cell::isAlive() const{
    return (state == ALIVED);
}

uint &cell::age() {
    return _age;
}
