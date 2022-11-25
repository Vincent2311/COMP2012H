#include "Wall.h"

Wall::Wall(int x, int y) : Terrain(x, y) {}
Wall::~Wall() {}
bool Wall::is_blocked() const { return true; }
string Wall::get_display_str() const { return SYM_WALL; }
