#include "Warlock.h"

Warlock::Warlock(int x, int y) : Monster(x, y)
{
    cur_hp = max_hp = 15;
    atk = 5, def = 1;
}

//write your code here
Warlock::~Warlock(){}

string Warlock::get_monster_str() const {
    if(cur_hp < 10) return "W0" + to_string(cur_hp);
    else return "W" + to_string(cur_hp);
}

void Warlock::action(Player &p, MapUnit *map[][MAP_SIZE]) {
    int player_x = 0;
    int player_y = 0;
    p.get_position(player_x,player_y);
    int distance = abs(player_x - x) + abs(player_y - y);
    if (distance <= 1) {
        p.attacked_by(atk);
    }
    for(int i = 0; i < 3; i ++) {
        for(int j = 0; j < 3; j++) {
            if(i==1 && j == 1) continue;
            if(x-1+i >= MAP_SIZE || x-1+i < 0 || y-1+j >= MAP_SIZE || y-1+j < 0) continue;
            if ((x-1+i != player_x || y-1+j != player_y) && !map[x-1+i][y-1+j]->is_blocked()) {
                delete map[x-1+i][y-1+j];
                map[x-1+i][y-1+j] = new Zombie(x-1+i,y-1+j);
            }
        } 
    }
}