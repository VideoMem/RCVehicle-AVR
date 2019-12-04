#include "uTimers.h"

class BangServo {
    public:
        void setup(int pin);
        void write(int val) { value = val > 2000? 2000: val < 1000? 1000: val; };
        void update();
        void setBlocking(bool b) { blocking = b; }
    private:
        uTimer* pulse;
        int value;
        int out;
        char cnt;
        bool blocking;

};