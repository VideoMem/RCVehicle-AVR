//#include "Module.h"
//#include "VFD.h"
#include "MapSerial.h"
#include "StringBuffer.h"

class T_gcode {
    public:
    T_gcode() { blank(); }
    void blank();
    int  g;
    int  m;
    int  s;
    int  x;
    int  y;
    int  u;
    int  v;
    int  b;
};

class Pty : public MapSerial {
    public:
        Pty(Manchester* m):MapSerial(m) { stdin = new StringBuffer(); setup();  }
        void update();
        void setup();
        void gstatus();
        void gcode(T_gcode &g);
        void clear() { code.blank(); }
        char getLast() { return lastCode; }
    protected:
        bool error;
        T_gcode code;
        StringBuffer* stdin;
        char lastCode;
        bool decode(char (&p) [STRBUF_LEN]);
        void submit(char rcode, char (&number) [STRBUF_LEN]);
        void blank(char (&number) [STRBUF_LEN]);
        void exec();
        void dump(char (&q) [STRBUF_LEN]) { for(int i = 0; i < STRBUF_LEN; i++) { Serial.print(" "); Serial.print((uint8_t)q[i]); } }
};