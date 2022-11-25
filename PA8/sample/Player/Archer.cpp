#include "Archer.h"

void Archer::level_up()
{
    max_hp += 2;
    cur_hp = max_hp;
    atk += 3;
    def += 2;
    max_mp += 1;
    level += 1;
    max_exp += 2;
}

Archer::Archer(int x_, int y_, const string &name_) : Player(x_, y_, name_)
{
    atk = 4;
    def = 3;
    cur_hp = max_hp = 12;
}
Archer::~Archer(){}
int Archer::get_range() const
{
    return 3;
}
int Archer::skill(int &atk_, int &range)
{
    if (cur_mp > 0)
    {
        cur_mp--;
        atk_ = MAX_ATK;
        range = 3;
        return 0;
    }
    else
        return 1;
}
Role Archer::get_role() const
{
    return Role::ARCHER;
}
