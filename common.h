#ifndef SPIDER_COMMON
#define SPIDER_COMMON



#define RESET_SECONDS 5000000
#define LONG_PRESS_SECONDS 2000000

#define IS_CLICK(duration) ((duration > 0) && (duration <= LONG_PRESS_SECONDS))
#define IS_LONG_PRESS(duration) (duration > LONG_PRESS_SECONDS)
#define IS_RESET_PRESS(duration) (duration > RESET_SECONDS)

#define bitset(byte, nbit) ((byte) |= (1 << (nbit)))
#define bitclear(byte, nbit) ((byte) &= ~(1 << (nbit)))
#define bitflip(byte, nbit) ((byte) ^= (1 << (nbit)))
#define bitcheck(byte, nbit) ((byte) & (1 << (nbit)))


#endif SPIDER_COMMON