#include <inttypes.h>
#include <Arduino.h>
#define STRBUF_LEN 32

class StringBuffer {
    public:
        StringBuffer();
        void add(char str);
        void del();
        void get(char (&b) [STRBUF_LEN]) { strcpy(b, buff); }
        void blank();
        void clear();
        uint8_t size() { return buff[0] != 0?  idx: 0; }
        void dump() { for(int i = 0; i < STRBUF_LEN; i++) { Serial.print(" "); Serial.print((uint8_t)buff[i]); } }

    protected:
        char buff[STRBUF_LEN];
        uint8_t idx;
};
