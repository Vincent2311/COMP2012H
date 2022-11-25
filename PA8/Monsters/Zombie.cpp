#include "Zombie.h"

Zombie::Zombie(int x, int y) : Monster(x, y)
{
    cur_hp = max_hp = 20;
    atk = 9, def = 3;
}

//write your code here
Zombie::~Zombie(){} 

string Zombie::get_monster_str() const {
    if(cur_hp < 10) return "Z0" + to_string(cur_hp);
    else return "Z" + to_string(cur_hp);
}

void Zombie::action(Player &p, MapUnit *map[][MAP_SIZE]) {
    int player_x = 0;
    int player_y = 0;
    p.get_position(player_x,player_y);
    int distance = abs(player_x - x) + abs(player_y - y);
    if (distance <= 1) {
        p.attacked_by(atk);
    }
}