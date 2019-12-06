#include "uTimers.h"
#include <Arduino.h>

class BangServo {
    public:
        void setup(int pin);
        void write(int val) { value = val <= 180? remap(val): val > 1990? 1990: val < 90? 90: val; };
        void update();
        void setBlocking(bool b) { blocking = b; }
    private:
        long remap(int val) { return map(val, 0, 180, 90, 1990); }
        uTimer* pulse;
        int value;
        int out;
        char cnt;
        bool blocking;

};