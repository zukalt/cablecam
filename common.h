#ifndef SPIDER_COMMON_h
#define SPIDER_COMMON_h

void (*arduinoHardReset)(void) = 0;


#define RESET_SECONDS 3000000
#define LONG_PRESS_SECONDS 2000000

#define IS_CLICK(duration) ((duration > 0) && (duration <= LONG_PRESS_SECONDS))
#define IS_LONG_PRESS(duration) ((duration) > LONG_PRESS_SECONDS)
#define IS_RESET_PRESS(duration) ((duration) > RESET_SECONDS)

#define bitset(byte, nbit) ((byte) |= (1 << (nbit)))
#define bitclear(byte, nbit) ((byte) &= ~(1 << (nbit)))
#define bitflip(byte, nbit) ((byte) ^= (1 << (nbit)))
#define bitcheck(byte, nbit) ((byte) & (1 << (nbit)))

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

#endif SPIDER_COMMON_h