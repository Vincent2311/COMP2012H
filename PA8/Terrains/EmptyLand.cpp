#include "EmptyLand.h"

//write your codes here

EmptyLand::EmptyLand(int x, int y) : Terrain(x,y){}

EmptyLand::~EmptyLand() {}

bool EmptyLand::is_blocked() const {
    return false;
}

string EmptyLand::get_display_str() const {
    return SYM_EMPTY; 
}

