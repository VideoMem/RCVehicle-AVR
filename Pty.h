//#include "Module.h"
#include "MapSerial.h"
#include "StringBuffer.h"

class Pty : public MapSerial {
    public:
        Pty(Manchester* m, char* p):MapSerial(m) { stdin = new StringBuffer(p); setup();  }
        void update();
        void setup();
        //void show() { mapSerial->print(stdin->get()); }
    protected:
        StringBuffer* stdin;
        void exec();
};