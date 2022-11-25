#include "ShadowPriest.h"

ShadowPriest::ShadowPriest(int x, int y) : Monster(x, y)
{
    cur_hp = max_hp = 15;
    atk = 5, def = 2;
}

// write your code here
ShadowPriest::~ShadowPriest() {}

string ShadowPriest::get_monster_str() const
{
    if (cur_hp < 10)
        return "S0" + to_string(cur_hp);
    else
        return "S" + to_string(cur_hp);
}

void ShadowPriest::action(Player &p, MapUnit *map[][MAP_SIZE])
{
    int player_x = 0;
    int player_y = 0;
    p.get_position(player_x, player_y);
    int distance = abs(player_x - x) + abs(player_y - y);
    if (distance <= 1)
    {
        p.attacked_by(atk);
    }
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (i == 1 && j == 1 || (x-1+i == player_x && y-1+j == player_y))
                continue;
            if(x-1+i >= MAP_SIZE || x-1+i < 0 || y-1+j >= MAP_SIZE || y-1+j < 0) continue;
            if (map[x-1+i][y-1+j]->get_unit_type() == UnitType::MONSTER) {
                if(map[x-1+i][y-1+j]->is_valid()) {
                    static_cast<Monster*>(map[x-1+i][y-1+j])->recover_hp(5);
                }
            }
        }
    }
}