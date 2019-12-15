//#include "Module.h"
//#include "VFD.h"
#include "MapSerial.h"
#include "StringBuffer.h"

struct T_twoAxis {
    int u;
    int v;
};

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

class Pty : public StringBuffer {
    public:
        Pty(Manchester* m):StringBuffer(m) { setup();  }
        void update();
        void setup() { code.blank(); echo = false; };
        void gstatus();
        void gcode(T_gcode &g);
        char getLast() { return lastCode; }
        void flush() { lastCode = 0; }
        void setEcho(bool b) { echo = b; }
        void resetMcode() { code.m = 0; }
        void resetGcode() { code.g = 0; }

    protected:
        bool error;
        T_gcode code;
        //StringBuffer* stdin;
        //char stdin[STRBUF_LEN];
        //int stdid;
        char lastCode;
        bool decode();
        //void dump(char (&d) [STRBUF_LEN]) { for(int i=0; d[i]; ++i) { mapSerial->print(" "); mapSerial->print(d[i]); } }
        void submit(char rcode, char (&number) [STRBUF_LEN]);
        void exec();
        bool echo;
};