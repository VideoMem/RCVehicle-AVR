/*
*  Manchester.h
*   MANCHESTER Interface
*   
*/

#ifndef MANCHESTER_H
#define MANCHESTER_H
#define MANCHESTER_VERSION "0"
#include <inttypes.h>
#include "Arduino.h"

class Manchester {
    public:
        Manchester() { direct = false; }
        unsigned char* encode(unsigned char byte);
        unsigned char decode(const char* encoded);
        void print(String &str) { print(str.c_str()); }
        void print(const char* string);
        void print(float arg);
        void print(int arg);
        void print(unsigned long arg);
        void print(long arg);
        void print(volatile uint16_t& arg);
        void printn(const char* string);
        void printn(const char* string, int val);
        void printn(const char* string, float val);
     //   void printn(const char* string, int16_t val) { printn(string, val); };
        void printn(const char* string, uint16_t val);
        void printn(const char* string, uint8_t val) { printn(string, (uint16_t) val); };
        void print(unsigned char arg);
        void print(char arg);
        void setDirect(bool flag) { direct = flag; }
        bool getDirect(void) { return direct; }
        int  available(void) { return Serial.available(); }
        char read();
    protected:
        bool direct;
        unsigned char buffer[2];
        void table(unsigned char index);
        unsigned char Rtable(unsigned char a, unsigned char b);
    private:
};

#endif

