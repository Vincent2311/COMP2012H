#include "Monster.h"
#include <cmath>

// write your code here
Monster::Monster(int x, int y) : MapUnit(x, y) {}

Monster::~Monster() {}

bool Monster::is_active() const
{
    return active;
}

void Monster::set_active()
{
    if (is_valid())
    {
        active = true;
    }
}

void Monster::recover_hp(int hp)
{
    cur_hp = min(cur_hp + hp, max_hp);
}

UnitType Monster::get_unit_type() const
{
    return UnitType::MONSTER;
}

bool Monster::is_blocked() const
{
    if (is_valid())
        return true;
    else
        return false;
}

int Monster::attacked_by(int atk_)
{
    if (!is_valid())
        return 0;
    else
    {
        int damage = atk_ - def;
        if (damage < 0)
            return 0;
        else
        {
            cur_hp -= damage;
            if (cur_hp <= 0)
            {
                invalidate();
                active = false;
                return 1;
            }
            else
                return 0;
        }
    }
}

string Monster::get_display_str() const
{
    if (!is_valid())
        return SYM_EMPTY;
    else
    {
        if (!is_discovered())
            return SYM_DANGER;
        else
        {
            return get_monster_str();
        }
    }
}

void Monster::follow(const Player &p, MapUnit *map[][MAP_SIZE])
{
    int player_x = 0;
    int player_y = 0;
    p.get_position(player_x, player_y);
    int distance = abs(player_x - x) + abs(player_y - y);
    if (distance == 1)
        return;
    int direction_x;

    if (player_x > x)
    {
        direction_x = 1;
    }
    else
    {
        direction_x = -1;
    }
    int direction_y;
    if (player_y > y)
    {
        direction_y = 1;
    }
    else
    {
        direction_y = -1;
    }
    if (abs(player_x - x) > 0 && !map[x + direction_x][y]->is_blocked())
    {
        delete map[x + direction_x][y];
        map[x + direction_x][y] = this;
        map[x][y] = new EmptyLand(x, y);
        x += direction_x;
    }
    else if (abs(player_y - y) > 0 && !map[x][y + direction_y]->is_blocked())
    {
        delete map[x][y + direction_y];
        map[x][y + direction_y] = this;
        map[x][y] = new EmptyLand(x, y);
        y += direction_y;
    }
}