#include "Terrain.h"

//write your codes here
Terrain::Terrain(int x, int y) : MapUnit(x,y){}

Terrain::~Terrain(){}

UnitType Terrain::get_unit_type() const {
    return UnitType::TERRAIN;
}
