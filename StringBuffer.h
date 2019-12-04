#include <inttypes.h>
#include <Arduino.h>
#include "MapSerial.h"
#define STRBUF_LEN 32

class StringBuffer: public MapSerial {
    public:
        StringBuffer(Manchester* m);
        void add(char str);
        void del();
        const char* get() { return buff; }
        void blank(char (&d) [STRBUF_LEN]) { memset(d, 0, STRBUF_LEN); }
        void blank() { blank(buff); };
        void clear();
        uint8_t size() { return idx; }
        void dump(char (&d) [STRBUF_LEN]) { for(int i=0; d[i]; ++i) { mapSerial->print(" "); mapSerial->print(d[i]); } }
        void dump(const char* d) { for(int i = 0; d[i]; i++) { mapSerial->print(" "); mapSerial->print((uint8_t)d[i]); } }
        void setup() { blank(); }
        void update() {}
    protected:
        char buff[STRBUF_LEN];
        uint8_t idx;
};
