#include"tools.h"




TYPENAME Assign::assign_val;

TYPENAME Multi::multi_base;

void Multi::set(TYPENAME input) {
    multi_base = input;
}

TYPENAME Multi::call(TYPENAME input) {
    return input * multi_base;
}

void Assign::set(TYPENAME input) {
    assign_val = input;
}

TYPENAME Assign::call(TYPENAME input) {
    return assign_val;
}
