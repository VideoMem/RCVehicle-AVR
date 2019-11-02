#include <inttypes.h>
#include <Arduino.h>
#define STRBUF_LEN 32

class StringBuffer {
    public:
        StringBuffer(char* p);
        void add(char str);
        void del();
        const char* get() { return buff; }

        void blank();
        void clear();
        uint8_t size() { return idx; }
        void dump() { for(int i = 0; i < STRBUF_LEN; i++) { Serial.print(" "); Serial.print((uint8_t)buff[i]); } }

    protected:
        char* buff;
        uint8_t idx;
};
