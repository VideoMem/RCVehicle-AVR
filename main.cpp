#include "main.h"

int main(void) {
    init();
    setup();

    for (;;) {
       loop();
    }
}

