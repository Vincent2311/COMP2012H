#include "ShadowPriest.h"

ShadowPriest::ShadowPriest(int x, int y) : Monster(x, y)
{
    cur_hp = max_hp = 15;
    atk = 5, def = 2;
}
ShadowPriest::~ShadowPriest() {}
string ShadowPriest::get_monster_str() const
{
    string s = "S";
    if (cur_hp < 10)
        s += "0";
    s += to_string(cur_hp);
    return s;
}
void ShadowPriest::action(Player &p, MapUnit *map[][MAP_SIZE])
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
            if (y + j < 0 || y + j >= MAP_SIZE)
                continue;
            if (i == 0 && j == 0)
                continue;
            MapUnit *mu = map[x + i][y + j];
            if (mu->is_valid() && mu->get_unit_type() == UnitType::MONSTER)
            {
                ((Monster *)mu)->recover_hp(atk);
            }
        }
    }
}
