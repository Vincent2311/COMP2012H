#include "Gem.h"

// write your codes here
Gem::Gem(int x, int y, GemType gem_type) : Item(x, y)
{
    gtype = gem_type;
}

Gem::~Gem() {}

void Gem::triggered_by(Player &p)
{
    if (!is_valid())
    {
        return;
    }
    else
    {
        Role role = p.get_role();
        if (role == Role::WARRIOR && gtype == RUBY || role == Role::ARCHER && gtype == EMERALD || role == Role::MAGE && gtype == SAPPHIRE)
        {
            p.charge_mp();
        }
        invalidate();
    }
}

string Gem::get_item_str() const
{
    if (gtype == RUBY)
    {
        return SYM_RUBY;
    }
    else if (gtype == EMERALD)
    {
        return SYM_EMERALD;
    }
    else
    {
        return SYM_SAPPHIRE;
    }
}