#include "Medicine.h"


//write your codes here
Medicine::Medicine(int x, int y) : Item(x,y){}

Medicine::~Medicine(){}

void Medicine::triggered_by(Player &p) {
    if (!is_valid()) return;
    else {
        p.recover_hp(3);
        invalidate();
        return;
    }
}

string Medicine::get_item_str() const {
    return SYM_MEDICINE;
}