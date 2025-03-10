#if !defined(BETTER_DRAGSCROLL_DIVISOR_H)
    #define BETTER_DRAGSCROLL_DIVISOR_H 4
#endif

#if !defined(DRAGSCROLL_DIVISOR_V)
    #define BETTER_DRAGSCROLL_DIVISOR_V 4
#endif

bool better_dragscroll_enabled = 0;

void better_dragscroll_toggle(bool pressed);
void better_dragscroll_momentary(bool pressed);
