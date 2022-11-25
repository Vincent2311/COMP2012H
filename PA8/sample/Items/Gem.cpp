#include "Gem.h"

Gem::Gem(int x, int y, GemType gem_type) : Item(x, y), gtype(gem_type){}
Gem::~Gem() {}
void Gem::triggered_by(Player &p)
{
    if (is_valid())
    {
        switch (gtype)
        {
        case GemType::EMERALD:
            if (p.get_role() == Role::ARCHER)
                p.charge_mp();
            break;
        case GemType::RUBY:
            if (p.get_role() == Role::WARRIOR)
                p.charge_mp();
            break;
        case GemType::SAPPHIRE:
            if (p.get_role() == Role::MAGE)
                p.charge_mp();
            break;
        default:
            break;
        }
        invalidate();
    }
}
string Gem::get_item_str() const
{
    switch (gtype)
    {
    case GemType::EMERALD:
        return SYM_EMERALD;
        break;
    case GemType::RUBY:
        return SYM_RUBY;
        break;
    case GemType::SAPPHIRE:
        return SYM_SAPPHIRE;
        break;
    default:
        return "";
        break;
    }
}