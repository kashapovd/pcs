//
// Created by yaroslav on 10/5/21.
//

#include <iostream>

#include "universe.h"

universe::universe(uint size) {
    universeSize = size;
    generation = 0;
    cube cube (
            size, std::vector<std::vector<cell>> (
                    size, std::vector<cell>(size)
            )
    );
    cuboid = std::move(cube);
    genUniverse();
}

uint universe::getNumberOfLiving() const{
    uint numberOfLiving = 0;
    for (uint x=0; x < universeSize; ++x) {
        for (uint y=0; y < universeSize; ++y) {
            for (uint z=0; z < universeSize; ++z) {
                cuboid[x][y][z].isAlive() ? numberOfLiving++ : numberOfLiving;
            }
        }
    }
    return numberOfLiving;
}

void universe::kill(uint x, uint y, uint z) {
    cuboid[x][y][z].die();
}

void universe::revive(uint x, uint y, uint z) {
    cuboid[x][y][z].resurrect();
}

bool universe::nextgen() {
    std::vector<cell*> toBeKilled;
    std::vector<cell*> toBeRevived;
    for (uint x=0; x < universeSize; ++x) {
        for (uint y=0; y < universeSize; ++y) {
            for (uint z=0; z < universeSize; ++z) {
                uint neighs = checkNeigh(x,y,z);
                if (cuboid[x][y][z].isAlive()) {
                    if (cuboid[x][y][z].age() == 2) {
                        toBeKilled.push_back(&cuboid[x][y][z]);
                        continue;
                    }
                    cuboid[x][y][z].age()++;
                        if ((neighs > 5 && neighs < 14)) {
                            toBeKilled.push_back(&cuboid[x][y][z]);
                        }
                } else {
                    if (neighs == 7 or neighs == 21 or neighs == 14 or neighs == 3) {
                        toBeRevived.push_back(&cuboid[x][y][z]);
                    }
                }
            }
        }
    }

    for(auto &cell : toBeKilled)
        cell->die();
    for(auto &cell : toBeRevived)
        cell->resurrect();
    generation++;
    return getNumberOfLiving() != 0;
}

bool universe::checkStagnation() {
    if ( getNumberOfLiving() == 0 || generation > 3)
        return false;
    else if (compareUniverses(cuboid, stagnation_queue.front()))
        return false;
    else if (!compareUniverses(cuboid, stagnation_queue.front())) {
        if (compareUniverses(stagnation_queue.at(0), stagnation_queue.at(2)) and compareUniverses(cuboid, stagnation_queue.at(1)))
            return false;
    }
    return true;
}

bool universe::compareUniverses(cube u1, cube u2) {
    for (uint x=0; x < universeSize; ++x) {
        for (uint y=0; y < universeSize; ++y) {
            for (uint z = 0; z < universeSize; ++z) {
                if (u1[x][y][z].isAlive() and u2[x][y][z].isAlive())
                    return true;
            }
        }
    }
    return false;
}

uint universe::checkNeigh(uint x, uint y, uint z) {
    uint numberOfAlive = 0;
    for (const auto &xitem : calculateNeighCoordsByAxis(x)) {
        for (const auto &yitem : calculateNeighCoordsByAxis(y)) {
            for (const auto &zitem : calculateNeighCoordsByAxis(z)) {
                if (xitem == x && yitem == y && zitem == z)
                    continue;
                cuboid[xitem][yitem][zitem].isAlive() ? numberOfAlive++ : numberOfAlive;
            }
        }
    }
    return numberOfAlive;
}

uint universe::getGeneraton() const {
    return generation;
}

void universe::print() {
    for (uint x=0; x < universeSize; ++x) {
        std::cout << "\nx=" << x;
        for (uint y = 0; y < universeSize; ++y) {
            std::cout << "\n";
            for (uint z = 0; z < universeSize; ++z) {
                std::cout << (cuboid[x][y][z].age()) << " ";
            }
        }
    }
}

void universe::genUniverse() {
    for (uint x=0; x < universeSize; ++x) {
        for (uint y = 0; y < universeSize; ++y) {
            for (uint z = 0; z < universeSize; ++z) {
                if (x == 0 or y == 0 or z == 0 or x == universeSize-1 or y == universeSize-1 or z == universeSize-1)
                    if (std::rand() % 10)
                        cuboid[x][y][z].resurrect();
            }
        }
    }
//    cuboid[4][5][5].resurrect();
//    cuboid[6][5][5].resurrect();
//    cuboid[5][4][5].resurrect();
//    cuboid[5][6][5].resurrect();
//    cuboid[5][5][4].resurrect();
//    cuboid[5][5][6].resurrect();
//    cuboid[6][6][5].resurrect();
}

std::vector<uint> universe::calculateNeighCoordsByAxis(const uint &x) const {
    return {(-1 + x +universeSize) % universeSize, x, (1 + x + universeSize) % universeSize};
}
