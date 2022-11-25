#include "Zombie.h"

Zombie::Zombie(int x, int y) : Monster(x, y)
{
    cur_hp = max_hp = 20;
    atk = 9, def = 3;
}
Zombie::~Zombie(){}
string Zombie::get_monster_str() const
{
    string s = "Z";
    if (cur_hp < 10)
        s += "0";
    s += to_string(cur_hp);
    return s;
}
void Zombie::action(Player &p, MapUnit *map[][MAP_SIZE])
{
    int px, py;
    p.get_position(px, py);
    int dx = px - x, dy = py - y;
    if (abs(dx) + abs(dy) > 1)
        return;
    else
        p.attacked_by(atk);
}
