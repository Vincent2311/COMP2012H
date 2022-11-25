#include "Warrior.h"

void Warrior::level_up()
{
    max_hp += 3;
    cur_hp = max_hp;
    atk += 2;
    def += 1;
    max_mp += 1;
    level += 1;
}

Warrior::Warrior(int x_, int y_, const string &name_) : Player(x_, y_, name_)
{
    atk = 6;
    def = 3;
    cur_hp = max_hp = 15;
}
Warrior::~Warrior() {}
void Warrior::attacked_by(int atk_)
{
    int damage = atk_ - def;
    if (damage >= cur_hp && cur_mp > 0)
    {
        cur_mp--;
        recover_hp(max_hp / 2);
        return;
    }
    else if (damage > 0)
    {
        cur_hp -= damage;
    }
}
int Warrior::get_range() const
{
    return 1;
}
int Warrior::skill(int &atk, int &range) { return 1; }
Role Warrior::get_role() const
{
    return Role::WARRIOR;
}
