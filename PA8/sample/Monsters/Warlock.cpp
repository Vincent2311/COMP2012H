#include "Warlock.h"

Warlock::Warlock(int x, int y) : Monster(x, y)
{
    cur_hp = max_hp = 15;
    atk = 5, def = 1;
}
Warlock::~Warlock(){}
string Warlock::get_monster_str() const
{
    string s = "W";
    if (cur_hp < 10)
        s += "0";
    s += to_string(cur_hp);
    return s;
}
void Warlock::action(Player &p, MapUnit *map[][MAP_SIZE])
{
    int px, py;
    p.get_position(px, py);
    int dx = px - x, dy = py - y;
    if (abs(dx) + abs(dy) <= 1)
        p.attacked_by(atk);
    for (int i = -1; i < 2; i++)
    {
        if (x + i < 0 || x + i >= MAP_SIZE)
            continue;
        for (int j = -1; j < 2; j++)
        {
            if (y + i < 0 || y + i >= MAP_SIZE)
                continue;
            if (((x + i) != px || (y + j) != py) && !map[x + i][y + j]->is_blocked())
            {
                delete map[x + i][y + j];
                map[x + i][y + j] = new Zombie(x + i, y + j);
            }
        }
    }
}
