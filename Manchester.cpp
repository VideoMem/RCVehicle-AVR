#include "Manchester.h"
#include <Arduino.h>

char Manchester::read() {
    char nonDir[2];
    if(getDirect()) {
        return Serial.read();
    } else {
        nonDir[0] = Serial.read();
        nonDir[1] = Serial.read();
        return decode(nonDir);
   }
}

unsigned char* Manchester::encode(unsigned char byte) {
    table(byte);
    return buffer;
}

unsigned char Manchester::decode(const char* encoded) {
    unsigned char decoded = Rtable(encoded[0], encoded[1]);    
    return decoded;
}

void Manchester::print(const char* string) {
    if(!direct) {
        const char* byte = string;
        for(byte = string; *byte; ++byte) {
            table((unsigned char) *byte);
            Serial.write(buffer[0]);
            Serial.write(buffer[1]);
        }
    } else {
        Serial.write(string);
    }
}

void Manchester::print(char arg) {
    if(!direct) {
        table((unsigned char) arg);
        Serial.write(buffer[0]);
        Serial.write(buffer[1]);
    } else
        Serial.write(arg);
}

void Manchester::print(float arg) {
    char charVal[10]; 
    dtostrf(arg, 4, 3, charVal);
    print(charVal);
}

void Manchester::print(int arg) {
    char charVal[10]; 
    dtostrf(arg, 1, 0, charVal);
    print(charVal);
}

void Manchester::print(unsigned long arg) {
    char charVal[20];
    ultoa(arg, charVal, 10);
    print(charVal);
}

void Manchester::print(long arg) {
    char charVal[20];
    ltoa(arg, charVal, 10);
    print(charVal);
}

void Manchester::print(volatile uint16_t& arg) {
    char charVal[20];
    unsigned int v = arg;
    utoa(v, charVal, 10);
    print(charVal);
}

void Manchester::print(unsigned char arg) {
    int v = (int) arg;
    print(v);
}

void Manchester::printn(const char* arg) {
    print(arg);
    print("\r\n");
}

void Manchester::printn(const char* arg, int val) {
    print(arg);
    print(val);
    print("\r\n");
}

void Manchester::printn(const char* arg, float val) {
    print(arg);
    print(val);
    print("\r\n");
}

void Manchester::printn(const char* arg, uint16_t val) {
    print(arg);
    print(val);
    print("\r\n");
}


void Manchester::table(unsigned char index) {
    unsigned char buf[2];

    switch(index) {
        case 0:
            // 00000000 => 10101010 : 10101010
            buf[0] = 0xAA;
            buf[1] = 0xAA;
        break;
        case 1:
            // 00000001 => 10101010 : 10101001
            buf[0] = 0xAA;
            buf[1] = 0xA9;
        break;
        case 2:
            // 00000010 => 10101010 : 10100110
            buf[0] = 0xAA;
            buf[1] = 0xA6;
        break;
        case 3:
            // 00000011 => 10101010 : 10100101
            buf[0] = 0xAA;
            buf[1] = 0xA5;
        break;
        case 4:
            // 00000100 => 10101010 : 10011010
            buf[0] = 0xAA;
            buf[1] = 0x9A;
        break;
        case 5:
            // 00000101 => 10101010 : 10011001
            buf[0] = 0xAA;
            buf[1] = 0x99;
        break;
        case 6:
            // 00000110 => 10101010 : 10010110
            buf[0] = 0xAA;
            buf[1] = 0x96;
        break;
        case 7:
            // 00000111 => 10101010 : 10010101
            buf[0] = 0xAA;
            buf[1] = 0x95;
        break;
        case 8:
            // 00001000 => 10101010 : 01101010
            buf[0] = 0xAA;
            buf[1] = 0x6A;
        break;
        case 9:
            // 00001001 => 10101010 : 01101001
            buf[0] = 0xAA;
            buf[1] = 0x69;
        break;
        case 10:
            // 00001010 => 10101010 : 01100110
            buf[0] = 0xAA;
            buf[1] = 0x66;
        break;
        case 11:
            // 00001011 => 10101010 : 01100101
            buf[0] = 0xAA;
            buf[1] = 0x65;
        break;
        case 12:
            // 00001100 => 10101010 : 01011010
            buf[0] = 0xAA;
            buf[1] = 0x5A;
        break;
        case 13:
            // 00001101 => 10101010 : 01011001
            buf[0] = 0xAA;
            buf[1] = 0x59;
        break;
        case 14:
            // 00001110 => 10101010 : 01010110
            buf[0] = 0xAA;
            buf[1] = 0x56;
        break;
        case 15:
            // 00001111 => 10101010 : 01010101
            buf[0] = 0xAA;
            buf[1] = 0x55;
        break;
        case 16:
            // 00010000 => 10101001 : 10101010
            buf[0] = 0xA9;
            buf[1] = 0xAA;
        break;
        case 17:
            // 00010001 => 10101001 : 10101001
            buf[0] = 0xA9;
            buf[1] = 0xA9;
        break;
        case 18:
            // 00010010 => 10101001 : 10100110
            buf[0] = 0xA9;
            buf[1] = 0xA6;
        break;
        case 19:
            // 00010011 => 10101001 : 10100101
            buf[0] = 0xA9;
            buf[1] = 0xA5;
        break;
        case 20:
            // 00010100 => 10101001 : 10011010
            buf[0] = 0xA9;
            buf[1] = 0x9A;
        break;
        case 21:
            // 00010101 => 10101001 : 10011001
            buf[0] = 0xA9;
            buf[1] = 0x99;
        break;
        case 22:
            // 00010110 => 10101001 : 10010110
            buf[0] = 0xA9;
            buf[1] = 0x96;
        break;
        case 23:
            // 00010111 => 10101001 : 10010101
            buf[0] = 0xA9;
            buf[1] = 0x95;
        break;
        case 24:
            // 00011000 => 10101001 : 01101010
            buf[0] = 0xA9;
            buf[1] = 0x6A;
        break;
        case 25:
            // 00011001 => 10101001 : 01101001
            buf[0] = 0xA9;
            buf[1] = 0x69;
        break;
        case 26:
            // 00011010 => 10101001 : 01100110
            buf[0] = 0xA9;
            buf[1] = 0x66;
        break;
        case 27:
            // 00011011 => 10101001 : 01100101
            buf[0] = 0xA9;
            buf[1] = 0x65;
        break;
        case 28:
            // 00011100 => 10101001 : 01011010
            buf[0] = 0xA9;
            buf[1] = 0x5A;
        break;
        case 29:
            // 00011101 => 10101001 : 01011001
            buf[0] = 0xA9;
            buf[1] = 0x59;
        break;
        case 30:
            // 00011110 => 10101001 : 01010110
            buf[0] = 0xA9;
            buf[1] = 0x56;
        break;
        case 31:
            // 00011111 => 10101001 : 01010101
            buf[0] = 0xA9;
            buf[1] = 0x55;
        break;
        case 32:
            // 00100000 => 10100110 : 10101010
            buf[0] = 0xA6;
            buf[1] = 0xAA;
        break;
        case 33:
            // 00100001 => 10100110 : 10101001
            buf[0] = 0xA6;
            buf[1] = 0xA9;
        break;
        case 34:
            // 00100010 => 10100110 : 10100110
            buf[0] = 0xA6;
            buf[1] = 0xA6;
        break;
        case 35:
            // 00100011 => 10100110 : 10100101
            buf[0] = 0xA6;
            buf[1] = 0xA5;
        break;
        case 36:
            // 00100100 => 10100110 : 10011010
            buf[0] = 0xA6;
            buf[1] = 0x9A;
        break;
        case 37:
            // 00100101 => 10100110 : 10011001
            buf[0] = 0xA6;
            buf[1] = 0x99;
        break;
        case 38:
            // 00100110 => 10100110 : 10010110
            buf[0] = 0xA6;
            buf[1] = 0x96;
        break;
        case 39:
            // 00100111 => 10100110 : 10010101
            buf[0] = 0xA6;
            buf[1] = 0x95;
        break;
        case 40:
            // 00101000 => 10100110 : 01101010
            buf[0] = 0xA6;
            buf[1] = 0x6A;
        break;
        case 41:
            // 00101001 => 10100110 : 01101001
            buf[0] = 0xA6;
            buf[1] = 0x69;
        break;
        case 42:
            // 00101010 => 10100110 : 01100110
            buf[0] = 0xA6;
            buf[1] = 0x66;
        break;
        case 43:
            // 00101011 => 10100110 : 01100101
            buf[0] = 0xA6;
            buf[1] = 0x65;
        break;
        case 44:
            // 00101100 => 10100110 : 01011010
            buf[0] = 0xA6;
            buf[1] = 0x5A;
        break;
        case 45:
            // 00101101 => 10100110 : 01011001
            buf[0] = 0xA6;
            buf[1] = 0x59;
        break;
        case 46:
            // 00101110 => 10100110 : 01010110
            buf[0] = 0xA6;
            buf[1] = 0x56;
        break;
        case 47:
            // 00101111 => 10100110 : 01010101
            buf[0] = 0xA6;
            buf[1] = 0x55;
        break;
        case 48:
            // 00110000 => 10100101 : 10101010
            buf[0] = 0xA5;
            buf[1] = 0xAA;
        break;
        case 49:
            // 00110001 => 10100101 : 10101001
            buf[0] = 0xA5;
            buf[1] = 0xA9;
        break;
        case 50:
            // 00110010 => 10100101 : 10100110
            buf[0] = 0xA5;
            buf[1] = 0xA6;
        break;
        case 51:
            // 00110011 => 10100101 : 10100101
            buf[0] = 0xA5;
            buf[1] = 0xA5;
        break;
        case 52:
            // 00110100 => 10100101 : 10011010
            buf[0] = 0xA5;
            buf[1] = 0x9A;
        break;
        case 53:
            // 00110101 => 10100101 : 10011001
            buf[0] = 0xA5;
            buf[1] = 0x99;
        break;
        case 54:
            // 00110110 => 10100101 : 10010110
            buf[0] = 0xA5;
            buf[1] = 0x96;
        break;
        case 55:
            // 00110111 => 10100101 : 10010101
            buf[0] = 0xA5;
            buf[1] = 0x95;
        break;
        case 56:
            // 00111000 => 10100101 : 01101010
            buf[0] = 0xA5;
            buf[1] = 0x6A;
        break;
        case 57:
            // 00111001 => 10100101 : 01101001
            buf[0] = 0xA5;
            buf[1] = 0x69;
        break;
        case 58:
            // 00111010 => 10100101 : 01100110
            buf[0] = 0xA5;
            buf[1] = 0x66;
        break;
        case 59:
            // 00111011 => 10100101 : 01100101
            buf[0] = 0xA5;
            buf[1] = 0x65;
        break;
        case 60:
            // 00111100 => 10100101 : 01011010
            buf[0] = 0xA5;
            buf[1] = 0x5A;
        break;
        case 61:
            // 00111101 => 10100101 : 01011001
            buf[0] = 0xA5;
            buf[1] = 0x59;
        break;
        case 62:
            // 00111110 => 10100101 : 01010110
            buf[0] = 0xA5;
            buf[1] = 0x56;
        break;
        case 63:
            // 00111111 => 10100101 : 01010101
            buf[0] = 0xA5;
            buf[1] = 0x55;
        break;
        case 64:
            // 01000000 => 10011010 : 10101010
            buf[0] = 0x9A;
            buf[1] = 0xAA;
        break;
        case 65:
            // 01000001 => 10011010 : 10101001
            buf[0] = 0x9A;
            buf[1] = 0xA9;
        break;
        case 66:
            // 01000010 => 10011010 : 10100110
            buf[0] = 0x9A;
            buf[1] = 0xA6;
        break;
        case 67:
            // 01000011 => 10011010 : 10100101
            buf[0] = 0x9A;
            buf[1] = 0xA5;
        break;
        case 68:
            // 01000100 => 10011010 : 10011010
            buf[0] = 0x9A;
            buf[1] = 0x9A;
        break;
        case 69:
            // 01000101 => 10011010 : 10011001
            buf[0] = 0x9A;
            buf[1] = 0x99;
        break;
        case 70:
            // 01000110 => 10011010 : 10010110
            buf[0] = 0x9A;
            buf[1] = 0x96;
        break;
        case 71:
            // 01000111 => 10011010 : 10010101
            buf[0] = 0x9A;
            buf[1] = 0x95;
        break;
        case 72:
            // 01001000 => 10011010 : 01101010
            buf[0] = 0x9A;
            buf[1] = 0x6A;
        break;
        case 73:
            // 01001001 => 10011010 : 01101001
            buf[0] = 0x9A;
            buf[1] = 0x69;
        break;
        case 74:
            // 01001010 => 10011010 : 01100110
            buf[0] = 0x9A;
            buf[1] = 0x66;
        break;
        case 75:
            // 01001011 => 10011010 : 01100101
            buf[0] = 0x9A;
            buf[1] = 0x65;
        break;
        case 76:
            // 01001100 => 10011010 : 01011010
            buf[0] = 0x9A;
            buf[1] = 0x5A;
        break;
        case 77:
            // 01001101 => 10011010 : 01011001
            buf[0] = 0x9A;
            buf[1] = 0x59;
        break;
        case 78:
            // 01001110 => 10011010 : 01010110
            buf[0] = 0x9A;
            buf[1] = 0x56;
        break;
        case 79:
            // 01001111 => 10011010 : 01010101
            buf[0] = 0x9A;
            buf[1] = 0x55;
        break;
        case 80:
            // 01010000 => 10011001 : 10101010
            buf[0] = 0x99;
            buf[1] = 0xAA;
        break;
        case 81:
            // 01010001 => 10011001 : 10101001
            buf[0] = 0x99;
            buf[1] = 0xA9;
        break;
        case 82:
            // 01010010 => 10011001 : 10100110
            buf[0] = 0x99;
            buf[1] = 0xA6;
        break;
        case 83:
            // 01010011 => 10011001 : 10100101
            buf[0] = 0x99;
            buf[1] = 0xA5;
        break;
        case 84:
            // 01010100 => 10011001 : 10011010
            buf[0] = 0x99;
            buf[1] = 0x9A;
        break;
        case 85:
            // 01010101 => 10011001 : 10011001
            buf[0] = 0x99;
            buf[1] = 0x99;
        break;
        case 86:
            // 01010110 => 10011001 : 10010110
            buf[0] = 0x99;
            buf[1] = 0x96;
        break;
        case 87:
            // 01010111 => 10011001 : 10010101
            buf[0] = 0x99;
            buf[1] = 0x95;
        break;
        case 88:
            // 01011000 => 10011001 : 01101010
            buf[0] = 0x99;
            buf[1] = 0x6A;
        break;
        case 89:
            // 01011001 => 10011001 : 01101001
            buf[0] = 0x99;
            buf[1] = 0x69;
        break;
        case 90:
            // 01011010 => 10011001 : 01100110
            buf[0] = 0x99;
            buf[1] = 0x66;
        break;
        case 91:
            // 01011011 => 10011001 : 01100101
            buf[0] = 0x99;
            buf[1] = 0x65;
        break;
        case 92:
            // 01011100 => 10011001 : 01011010
            buf[0] = 0x99;
            buf[1] = 0x5A;
        break;
        case 93:
            // 01011101 => 10011001 : 01011001
            buf[0] = 0x99;
            buf[1] = 0x59;
        break;
        case 94:
            // 01011110 => 10011001 : 01010110
            buf[0] = 0x99;
            buf[1] = 0x56;
        break;
        case 95:
            // 01011111 => 10011001 : 01010101
            buf[0] = 0x99;
            buf[1] = 0x55;
        break;
        case 96:
            // 01100000 => 10010110 : 10101010
            buf[0] = 0x96;
            buf[1] = 0xAA;
        break;
        case 97:
            // 01100001 => 10010110 : 10101001
            buf[0] = 0x96;
            buf[1] = 0xA9;
        break;
        case 98:
            // 01100010 => 10010110 : 10100110
            buf[0] = 0x96;
            buf[1] = 0xA6;
        break;
        case 99:
            // 01100011 => 10010110 : 10100101
            buf[0] = 0x96;
            buf[1] = 0xA5;
        break;
        case 100:
            // 01100100 => 10010110 : 10011010
            buf[0] = 0x96;
            buf[1] = 0x9A;
        break;
        case 101:
            // 01100101 => 10010110 : 10011001
            buf[0] = 0x96;
            buf[1] = 0x99;
        break;
        case 102:
            // 01100110 => 10010110 : 10010110
            buf[0] = 0x96;
            buf[1] = 0x96;
        break;
        case 103:
            // 01100111 => 10010110 : 10010101
            buf[0] = 0x96;
            buf[1] = 0x95;
        break;
        case 104:
            // 01101000 => 10010110 : 01101010
            buf[0] = 0x96;
            buf[1] = 0x6A;
        break;
        case 105:
            // 01101001 => 10010110 : 01101001
            buf[0] = 0x96;
            buf[1] = 0x69;
        break;
        case 106:
            // 01101010 => 10010110 : 01100110
            buf[0] = 0x96;
            buf[1] = 0x66;
        break;
        case 107:
            // 01101011 => 10010110 : 01100101
            buf[0] = 0x96;
            buf[1] = 0x65;
        break;
        case 108:
            // 01101100 => 10010110 : 01011010
            buf[0] = 0x96;
            buf[1] = 0x5A;
        break;
        case 109:
            // 01101101 => 10010110 : 01011001
            buf[0] = 0x96;
            buf[1] = 0x59;
        break;
        case 110:
            // 01101110 => 10010110 : 01010110
            buf[0] = 0x96;
            buf[1] = 0x56;
        break;
        case 111:
            // 01101111 => 10010110 : 01010101
            buf[0] = 0x96;
            buf[1] = 0x55;
        break;
        case 112:
            // 01110000 => 10010101 : 10101010
            buf[0] = 0x95;
            buf[1] = 0xAA;
        break;
        case 113:
            // 01110001 => 10010101 : 10101001
            buf[0] = 0x95;
            buf[1] = 0xA9;
        break;
        case 114:
            // 01110010 => 10010101 : 10100110
            buf[0] = 0x95;
            buf[1] = 0xA6;
        break;
        case 115:
            // 01110011 => 10010101 : 10100101
            buf[0] = 0x95;
            buf[1] = 0xA5;
        break;
        case 116:
            // 01110100 => 10010101 : 10011010
            buf[0] = 0x95;
            buf[1] = 0x9A;
        break;
        case 117:
            // 01110101 => 10010101 : 10011001
            buf[0] = 0x95;
            buf[1] = 0x99;
        break;
        case 118:
            // 01110110 => 10010101 : 10010110
            buf[0] = 0x95;
            buf[1] = 0x96;
        break;
        case 119:
            // 01110111 => 10010101 : 10010101
            buf[0] = 0x95;
            buf[1] = 0x95;
        break;
        case 120:
            // 01111000 => 10010101 : 01101010
            buf[0] = 0x95;
            buf[1] = 0x6A;
        break;
        case 121:
            // 01111001 => 10010101 : 01101001
            buf[0] = 0x95;
            buf[1] = 0x69;
        break;
        case 122:
            // 01111010 => 10010101 : 01100110
            buf[0] = 0x95;
            buf[1] = 0x66;
        break;
        case 123:
            // 01111011 => 10010101 : 01100101
            buf[0] = 0x95;
            buf[1] = 0x65;
        break;
        case 124:
            // 01111100 => 10010101 : 01011010
            buf[0] = 0x95;
            buf[1] = 0x5A;
        break;
        case 125:
            // 01111101 => 10010101 : 01011001
            buf[0] = 0x95;
            buf[1] = 0x59;
        break;
        case 126:
            // 01111110 => 10010101 : 01010110
            buf[0] = 0x95;
            buf[1] = 0x56;
        break;
        case 127:
            // 01111111 => 10010101 : 01010101
            buf[0] = 0x95;
            buf[1] = 0x55;
        break;
        case 128:
            // 10000000 => 01101010 : 10101010
            buf[0] = 0x6A;
            buf[1] = 0xAA;
        break;
        case 129:
            // 10000001 => 01101010 : 10101001
            buf[0] = 0x6A;
            buf[1] = 0xA9;
        break;
        case 130:
            // 10000010 => 01101010 : 10100110
            buf[0] = 0x6A;
            buf[1] = 0xA6;
        break;
        case 131:
            // 10000011 => 01101010 : 10100101
            buf[0] = 0x6A;
            buf[1] = 0xA5;
        break;
        case 132:
            // 10000100 => 01101010 : 10011010
            buf[0] = 0x6A;
            buf[1] = 0x9A;
        break;
        case 133:
            // 10000101 => 01101010 : 10011001
            buf[0] = 0x6A;
            buf[1] = 0x99;
        break;
        case 134:
            // 10000110 => 01101010 : 10010110
            buf[0] = 0x6A;
            buf[1] = 0x96;
        break;
        case 135:
            // 10000111 => 01101010 : 10010101
            buf[0] = 0x6A;
            buf[1] = 0x95;
        break;
        case 136:
            // 10001000 => 01101010 : 01101010
            buf[0] = 0x6A;
            buf[1] = 0x6A;
        break;
        case 137:
            // 10001001 => 01101010 : 01101001
            buf[0] = 0x6A;
            buf[1] = 0x69;
        break;
        case 138:
            // 10001010 => 01101010 : 01100110
            buf[0] = 0x6A;
            buf[1] = 0x66;
        break;
        case 139:
            // 10001011 => 01101010 : 01100101
            buf[0] = 0x6A;
            buf[1] = 0x65;
        break;
        case 140:
            // 10001100 => 01101010 : 01011010
            buf[0] = 0x6A;
            buf[1] = 0x5A;
        break;
        case 141:
            // 10001101 => 01101010 : 01011001
            buf[0] = 0x6A;
            buf[1] = 0x59;
        break;
        case 142:
            // 10001110 => 01101010 : 01010110
            buf[0] = 0x6A;
            buf[1] = 0x56;
        break;
        case 143:
            // 10001111 => 01101010 : 01010101
            buf[0] = 0x6A;
            buf[1] = 0x55;
        break;
        case 144:
            // 10010000 => 01101001 : 10101010
            buf[0] = 0x69;
            buf[1] = 0xAA;
        break;
        case 145:
            // 10010001 => 01101001 : 10101001
            buf[0] = 0x69;
            buf[1] = 0xA9;
        break;
        case 146:
            // 10010010 => 01101001 : 10100110
            buf[0] = 0x69;
            buf[1] = 0xA6;
        break;
        case 147:
            // 10010011 => 01101001 : 10100101
            buf[0] = 0x69;
            buf[1] = 0xA5;
        break;
        case 148:
            // 10010100 => 01101001 : 10011010
            buf[0] = 0x69;
            buf[1] = 0x9A;
        break;
        case 149:
            // 10010101 => 01101001 : 10011001
            buf[0] = 0x69;
            buf[1] = 0x99;
        break;
        case 150:
            // 10010110 => 01101001 : 10010110
            buf[0] = 0x69;
            buf[1] = 0x96;
        break;
        case 151:
            // 10010111 => 01101001 : 10010101
            buf[0] = 0x69;
            buf[1] = 0x95;
        break;
        case 152:
            // 10011000 => 01101001 : 01101010
            buf[0] = 0x69;
            buf[1] = 0x6A;
        break;
        case 153:
            // 10011001 => 01101001 : 01101001
            buf[0] = 0x69;
            buf[1] = 0x69;
        break;
        case 154:
            // 10011010 => 01101001 : 01100110
            buf[0] = 0x69;
            buf[1] = 0x66;
        break;
        case 155:
            // 10011011 => 01101001 : 01100101
            buf[0] = 0x69;
            buf[1] = 0x65;
        break;
        case 156:
            // 10011100 => 01101001 : 01011010
            buf[0] = 0x69;
            buf[1] = 0x5A;
        break;
        case 157:
            // 10011101 => 01101001 : 01011001
            buf[0] = 0x69;
            buf[1] = 0x59;
        break;
        case 158:
            // 10011110 => 01101001 : 01010110
            buf[0] = 0x69;
            buf[1] = 0x56;
        break;
        case 159:
            // 10011111 => 01101001 : 01010101
            buf[0] = 0x69;
            buf[1] = 0x55;
        break;
        case 160:
            // 10100000 => 01100110 : 10101010
            buf[0] = 0x66;
            buf[1] = 0xAA;
        break;
        case 161:
            // 10100001 => 01100110 : 10101001
            buf[0] = 0x66;
            buf[1] = 0xA9;
        break;
        case 162:
            // 10100010 => 01100110 : 10100110
            buf[0] = 0x66;
            buf[1] = 0xA6;
        break;
        case 163:
            // 10100011 => 01100110 : 10100101
            buf[0] = 0x66;
            buf[1] = 0xA5;
        break;
        case 164:
            // 10100100 => 01100110 : 10011010
            buf[0] = 0x66;
            buf[1] = 0x9A;
        break;
        case 165:
            // 10100101 => 01100110 : 10011001
            buf[0] = 0x66;
            buf[1] = 0x99;
        break;
        case 166:
            // 10100110 => 01100110 : 10010110
            buf[0] = 0x66;
            buf[1] = 0x96;
        break;
        case 167:
            // 10100111 => 01100110 : 10010101
            buf[0] = 0x66;
            buf[1] = 0x95;
        break;
        case 168:
            // 10101000 => 01100110 : 01101010
            buf[0] = 0x66;
            buf[1] = 0x6A;
        break;
        case 169:
            // 10101001 => 01100110 : 01101001
            buf[0] = 0x66;
            buf[1] = 0x69;
        break;
        case 170:
            // 10101010 => 01100110 : 01100110
            buf[0] = 0x66;
            buf[1] = 0x66;
        break;
        case 171:
            // 10101011 => 01100110 : 01100101
            buf[0] = 0x66;
            buf[1] = 0x65;
        break;
        case 172:
            // 10101100 => 01100110 : 01011010
            buf[0] = 0x66;
            buf[1] = 0x5A;
        break;
        case 173:
            // 10101101 => 01100110 : 01011001
            buf[0] = 0x66;
            buf[1] = 0x59;
        break;
        case 174:
            // 10101110 => 01100110 : 01010110
            buf[0] = 0x66;
            buf[1] = 0x56;
        break;
        case 175:
            // 10101111 => 01100110 : 01010101
            buf[0] = 0x66;
            buf[1] = 0x55;
        break;
        case 176:
            // 10110000 => 01100101 : 10101010
            buf[0] = 0x65;
            buf[1] = 0xAA;
        break;
        case 177:
            // 10110001 => 01100101 : 10101001
            buf[0] = 0x65;
            buf[1] = 0xA9;
        break;
        case 178:
            // 10110010 => 01100101 : 10100110
            buf[0] = 0x65;
            buf[1] = 0xA6;
        break;
        case 179:
            // 10110011 => 01100101 : 10100101
            buf[0] = 0x65;
            buf[1] = 0xA5;
        break;
        case 180:
            // 10110100 => 01100101 : 10011010
            buf[0] = 0x65;
            buf[1] = 0x9A;
        break;
        case 181:
            // 10110101 => 01100101 : 10011001
            buf[0] = 0x65;
            buf[1] = 0x99;
        break;
        case 182:
            // 10110110 => 01100101 : 10010110
            buf[0] = 0x65;
            buf[1] = 0x96;
        break;
        case 183:
            // 10110111 => 01100101 : 10010101
            buf[0] = 0x65;
            buf[1] = 0x95;
        break;
        case 184:
            // 10111000 => 01100101 : 01101010
            buf[0] = 0x65;
            buf[1] = 0x6A;
        break;
        case 185:
            // 10111001 => 01100101 : 01101001
            buf[0] = 0x65;
            buf[1] = 0x69;
        break;
        case 186:
            // 10111010 => 01100101 : 01100110
            buf[0] = 0x65;
            buf[1] = 0x66;
        break;
        case 187:
            // 10111011 => 01100101 : 01100101
            buf[0] = 0x65;
            buf[1] = 0x65;
        break;
        case 188:
            // 10111100 => 01100101 : 01011010
            buf[0] = 0x65;
            buf[1] = 0x5A;
        break;
        case 189:
            // 10111101 => 01100101 : 01011001
            buf[0] = 0x65;
            buf[1] = 0x59;
        break;
        case 190:
            // 10111110 => 01100101 : 01010110
            buf[0] = 0x65;
            buf[1] = 0x56;
        break;
        case 191:
            // 10111111 => 01100101 : 01010101
            buf[0] = 0x65;
            buf[1] = 0x55;
        break;
        case 192:
            // 11000000 => 01011010 : 10101010
            buf[0] = 0x5A;
            buf[1] = 0xAA;
        break;
        case 193:
            // 11000001 => 01011010 : 10101001
            buf[0] = 0x5A;
            buf[1] = 0xA9;
        break;
        case 194:
            // 11000010 => 01011010 : 10100110
            buf[0] = 0x5A;
            buf[1] = 0xA6;
        break;
        case 195:
            // 11000011 => 01011010 : 10100101
            buf[0] = 0x5A;
            buf[1] = 0xA5;
        break;
        case 196:
            // 11000100 => 01011010 : 10011010
            buf[0] = 0x5A;
            buf[1] = 0x9A;
        break;
        case 197:
            // 11000101 => 01011010 : 10011001
            buf[0] = 0x5A;
            buf[1] = 0x99;
        break;
        case 198:
            // 11000110 => 01011010 : 10010110
            buf[0] = 0x5A;
            buf[1] = 0x96;
        break;
        case 199:
            // 11000111 => 01011010 : 10010101
            buf[0] = 0x5A;
            buf[1] = 0x95;
        break;
        case 200:
            // 11001000 => 01011010 : 01101010
            buf[0] = 0x5A;
            buf[1] = 0x6A;
        break;
        case 201:
            // 11001001 => 01011010 : 01101001
            buf[0] = 0x5A;
            buf[1] = 0x69;
        break;
        case 202:
            // 11001010 => 01011010 : 01100110
            buf[0] = 0x5A;
            buf[1] = 0x66;
        break;
        case 203:
            // 11001011 => 01011010 : 01100101
            buf[0] = 0x5A;
            buf[1] = 0x65;
        break;
        case 204:
            // 11001100 => 01011010 : 01011010
            buf[0] = 0x5A;
            buf[1] = 0x5A;
        break;
        case 205:
            // 11001101 => 01011010 : 01011001
            buf[0] = 0x5A;
            buf[1] = 0x59;
        break;
        case 206:
            // 11001110 => 01011010 : 01010110
            buf[0] = 0x5A;
            buf[1] = 0x56;
        break;
        case 207:
            // 11001111 => 01011010 : 01010101
            buf[0] = 0x5A;
            buf[1] = 0x55;
        break;
        case 208:
            // 11010000 => 01011001 : 10101010
            buf[0] = 0x59;
            buf[1] = 0xAA;
        break;
        case 209:
            // 11010001 => 01011001 : 10101001
            buf[0] = 0x59;
            buf[1] = 0xA9;
        break;
        case 210:
            // 11010010 => 01011001 : 10100110
            buf[0] = 0x59;
            buf[1] = 0xA6;
        break;
        case 211:
            // 11010011 => 01011001 : 10100101
            buf[0] = 0x59;
            buf[1] = 0xA5;
        break;
        case 212:
            // 11010100 => 01011001 : 10011010
            buf[0] = 0x59;
            buf[1] = 0x9A;
        break;
        case 213:
            // 11010101 => 01011001 : 10011001
            buf[0] = 0x59;
            buf[1] = 0x99;
        break;
        case 214:
            // 11010110 => 01011001 : 10010110
            buf[0] = 0x59;
            buf[1] = 0x96;
        break;
        case 215:
            // 11010111 => 01011001 : 10010101
            buf[0] = 0x59;
            buf[1] = 0x95;
        break;
        case 216:
            // 11011000 => 01011001 : 01101010
            buf[0] = 0x59;
            buf[1] = 0x6A;
        break;
        case 217:
            // 11011001 => 01011001 : 01101001
            buf[0] = 0x59;
            buf[1] = 0x69;
        break;
        case 218:
            // 11011010 => 01011001 : 01100110
            buf[0] = 0x59;
            buf[1] = 0x66;
        break;
        case 219:
            // 11011011 => 01011001 : 01100101
            buf[0] = 0x59;
            buf[1] = 0x65;
        break;
        case 220:
            // 11011100 => 01011001 : 01011010
            buf[0] = 0x59;
            buf[1] = 0x5A;
        break;
        case 221:
            // 11011101 => 01011001 : 01011001
            buf[0] = 0x59;
            buf[1] = 0x59;
        break;
        case 222:
            // 11011110 => 01011001 : 01010110
            buf[0] = 0x59;
            buf[1] = 0x56;
        break;
        case 223:
            // 11011111 => 01011001 : 01010101
            buf[0] = 0x59;
            buf[1] = 0x55;
        break;
        case 224:
            // 11100000 => 01010110 : 10101010
            buf[0] = 0x56;
            buf[1] = 0xAA;
        break;
        case 225:
            // 11100001 => 01010110 : 10101001
            buf[0] = 0x56;
            buf[1] = 0xA9;
        break;
        case 226:
            // 11100010 => 01010110 : 10100110
            buf[0] = 0x56;
            buf[1] = 0xA6;
        break;
        case 227:
            // 11100011 => 01010110 : 10100101
            buf[0] = 0x56;
            buf[1] = 0xA5;
        break;
        case 228:
            // 11100100 => 01010110 : 10011010
            buf[0] = 0x56;
            buf[1] = 0x9A;
        break;
        case 229:
            // 11100101 => 01010110 : 10011001
            buf[0] = 0x56;
            buf[1] = 0x99;
        break;
        case 230:
            // 11100110 => 01010110 : 10010110
            buf[0] = 0x56;
            buf[1] = 0x96;
        break;
        case 231:
            // 11100111 => 01010110 : 10010101
            buf[0] = 0x56;
            buf[1] = 0x95;
        break;
        case 232:
            // 11101000 => 01010110 : 01101010
            buf[0] = 0x56;
            buf[1] = 0x6A;
        break;
        case 233:
            // 11101001 => 01010110 : 01101001
            buf[0] = 0x56;
            buf[1] = 0x69;
        break;
        case 234:
            // 11101010 => 01010110 : 01100110
            buf[0] = 0x56;
            buf[1] = 0x66;
        break;
        case 235:
            // 11101011 => 01010110 : 01100101
            buf[0] = 0x56;
            buf[1] = 0x65;
        break;
        case 236:
            // 11101100 => 01010110 : 01011010
            buf[0] = 0x56;
            buf[1] = 0x5A;
        break;
        case 237:
            // 11101101 => 01010110 : 01011001
            buf[0] = 0x56;
            buf[1] = 0x59;
        break;
        case 238:
            // 11101110 => 01010110 : 01010110
            buf[0] = 0x56;
            buf[1] = 0x56;
        break;
        case 239:
            // 11101111 => 01010110 : 01010101
            buf[0] = 0x56;
            buf[1] = 0x55;
        break;
        case 240:
            // 11110000 => 01010101 : 10101010
            buf[0] = 0x55;
            buf[1] = 0xAA;
        break;
        case 241:
            // 11110001 => 01010101 : 10101001
            buf[0] = 0x55;
            buf[1] = 0xA9;
        break;
        case 242:
            // 11110010 => 01010101 : 10100110
            buf[0] = 0x55;
            buf[1] = 0xA6;
        break;
        case 243:
            // 11110011 => 01010101 : 10100101
            buf[0] = 0x55;
            buf[1] = 0xA5;
        break;
        case 244:
            // 11110100 => 01010101 : 10011010
            buf[0] = 0x55;
            buf[1] = 0x9A;
        break;
        case 245:
            // 11110101 => 01010101 : 10011001
            buf[0] = 0x55;
            buf[1] = 0x99;
        break;
        case 246:
            // 11110110 => 01010101 : 10010110
            buf[0] = 0x55;
            buf[1] = 0x96;
        break;
        case 247:
            // 11110111 => 01010101 : 10010101
            buf[0] = 0x55;
            buf[1] = 0x95;
        break;
        case 248:
            // 11111000 => 01010101 : 01101010
            buf[0] = 0x55;
            buf[1] = 0x6A;
        break;
        case 249:
            // 11111001 => 01010101 : 01101001
            buf[0] = 0x55;
            buf[1] = 0x69;
        break;
        case 250:
            // 11111010 => 01010101 : 01100110
            buf[0] = 0x55;
            buf[1] = 0x66;
        break;
        case 251:
            // 11111011 => 01010101 : 01100101
            buf[0] = 0x55;
            buf[1] = 0x65;
        break;
        case 252:
            // 11111100 => 01010101 : 01011010
            buf[0] = 0x55;
            buf[1] = 0x5A;
        break;
        case 253:
            // 11111101 => 01010101 : 01011001
            buf[0] = 0x55;
            buf[1] = 0x59;
        break;
        case 254:
            // 11111110 => 01010101 : 01010110
            buf[0] = 0x55;
            buf[1] = 0x56;
        break;
        case 255:
            // 11111111 => 01010101 : 01010101
            buf[0] = 0x55;
            buf[1] = 0x55;
        break;
    }
    memcpy(buffer,buf,sizeof(unsigned char) * 2);
}

unsigned char Manchester::Rtable(unsigned char a, unsigned char b) {

    // 10101010 : 10101010 => 00000000
    if (a == 0xAA && b == 0xAA)
        return 0;

    // 10101010 : 10101001 => 00000001
    if (a == 0xAA && b == 0xA9)
        return 1;

    // 10101010 : 10100110 => 00000010
    if (a == 0xAA && b == 0xA6)
        return 2;

    // 10101010 : 10100101 => 00000011
    if (a == 0xAA && b == 0xA5)
        return 3;

    // 10101010 : 10011010 => 00000100
    if (a == 0xAA && b == 0x9A)
        return 4;

    // 10101010 : 10011001 => 00000101
    if (a == 0xAA && b == 0x99)
        return 5;

    // 10101010 : 10010110 => 00000110
    if (a == 0xAA && b == 0x96)
        return 6;

    // 10101010 : 10010101 => 00000111
    if (a == 0xAA && b == 0x95)
        return 7;

    // 10101010 : 01101010 => 00001000
    if (a == 0xAA && b == 0x6A)
        return 8;

    // 10101010 : 01101001 => 00001001
    if (a == 0xAA && b == 0x69)
        return 9;

    // 10101010 : 01100110 => 00001010
    if (a == 0xAA && b == 0x66)
        return 10;

    // 10101010 : 01100101 => 00001011
    if (a == 0xAA && b == 0x65)
        return 11;

    // 10101010 : 01011010 => 00001100
    if (a == 0xAA && b == 0x5A)
        return 12;

    // 10101010 : 01011001 => 00001101
    if (a == 0xAA && b == 0x59)
        return 13;

    // 10101010 : 01010110 => 00001110
    if (a == 0xAA && b == 0x56)
        return 14;

    // 10101010 : 01010101 => 00001111
    if (a == 0xAA && b == 0x55)
        return 15;

    // 10101001 : 10101010 => 00010000
    if (a == 0xA9 && b == 0xAA)
        return 16;

    // 10101001 : 10101001 => 00010001
    if (a == 0xA9 && b == 0xA9)
        return 17;

    // 10101001 : 10100110 => 00010010
    if (a == 0xA9 && b == 0xA6)
        return 18;

    // 10101001 : 10100101 => 00010011
    if (a == 0xA9 && b == 0xA5)
        return 19;

    // 10101001 : 10011010 => 00010100
    if (a == 0xA9 && b == 0x9A)
        return 20;

    // 10101001 : 10011001 => 00010101
    if (a == 0xA9 && b == 0x99)
        return 21;

    // 10101001 : 10010110 => 00010110
    if (a == 0xA9 && b == 0x96)
        return 22;

    // 10101001 : 10010101 => 00010111
    if (a == 0xA9 && b == 0x95)
        return 23;

    // 10101001 : 01101010 => 00011000
    if (a == 0xA9 && b == 0x6A)
        return 24;

    // 10101001 : 01101001 => 00011001
    if (a == 0xA9 && b == 0x69)
        return 25;

    // 10101001 : 01100110 => 00011010
    if (a == 0xA9 && b == 0x66)
        return 26;

    // 10101001 : 01100101 => 00011011
    if (a == 0xA9 && b == 0x65)
        return 27;

    // 10101001 : 01011010 => 00011100
    if (a == 0xA9 && b == 0x5A)
        return 28;

    // 10101001 : 01011001 => 00011101
    if (a == 0xA9 && b == 0x59)
        return 29;

    // 10101001 : 01010110 => 00011110
    if (a == 0xA9 && b == 0x56)
        return 30;

    // 10101001 : 01010101 => 00011111
    if (a == 0xA9 && b == 0x55)
        return 31;

    // 10100110 : 10101010 => 00100000
    if (a == 0xA6 && b == 0xAA)
        return 32;

    // 10100110 : 10101001 => 00100001
    if (a == 0xA6 && b == 0xA9)
        return 33;

    // 10100110 : 10100110 => 00100010
    if (a == 0xA6 && b == 0xA6)
        return 34;

    // 10100110 : 10100101 => 00100011
    if (a == 0xA6 && b == 0xA5)
        return 35;

    // 10100110 : 10011010 => 00100100
    if (a == 0xA6 && b == 0x9A)
        return 36;

    // 10100110 : 10011001 => 00100101
    if (a == 0xA6 && b == 0x99)
        return 37;

    // 10100110 : 10010110 => 00100110
    if (a == 0xA6 && b == 0x96)
        return 38;

    // 10100110 : 10010101 => 00100111
    if (a == 0xA6 && b == 0x95)
        return 39;

    // 10100110 : 01101010 => 00101000
    if (a == 0xA6 && b == 0x6A)
        return 40;

    // 10100110 : 01101001 => 00101001
    if (a == 0xA6 && b == 0x69)
        return 41;

    // 10100110 : 01100110 => 00101010
    if (a == 0xA6 && b == 0x66)
        return 42;

    // 10100110 : 01100101 => 00101011
    if (a == 0xA6 && b == 0x65)
        return 43;

    // 10100110 : 01011010 => 00101100
    if (a == 0xA6 && b == 0x5A)
        return 44;

    // 10100110 : 01011001 => 00101101
    if (a == 0xA6 && b == 0x59)
        return 45;

    // 10100110 : 01010110 => 00101110
    if (a == 0xA6 && b == 0x56)
        return 46;

    // 10100110 : 01010101 => 00101111
    if (a == 0xA6 && b == 0x55)
        return 47;

    // 10100101 : 10101010 => 00110000
    if (a == 0xA5 && b == 0xAA)
        return 48;

    // 10100101 : 10101001 => 00110001
    if (a == 0xA5 && b == 0xA9)
        return 49;

    // 10100101 : 10100110 => 00110010
    if (a == 0xA5 && b == 0xA6)
        return 50;

    // 10100101 : 10100101 => 00110011
    if (a == 0xA5 && b == 0xA5)
        return 51;

    // 10100101 : 10011010 => 00110100
    if (a == 0xA5 && b == 0x9A)
        return 52;

    // 10100101 : 10011001 => 00110101
    if (a == 0xA5 && b == 0x99)
        return 53;

    // 10100101 : 10010110 => 00110110
    if (a == 0xA5 && b == 0x96)
        return 54;

    // 10100101 : 10010101 => 00110111
    if (a == 0xA5 && b == 0x95)
        return 55;

    // 10100101 : 01101010 => 00111000
    if (a == 0xA5 && b == 0x6A)
        return 56;

    // 10100101 : 01101001 => 00111001
    if (a == 0xA5 && b == 0x69)
        return 57;

    // 10100101 : 01100110 => 00111010
    if (a == 0xA5 && b == 0x66)
        return 58;

    // 10100101 : 01100101 => 00111011
    if (a == 0xA5 && b == 0x65)
        return 59;

    // 10100101 : 01011010 => 00111100
    if (a == 0xA5 && b == 0x5A)
        return 60;

    // 10100101 : 01011001 => 00111101
    if (a == 0xA5 && b == 0x59)
        return 61;

    // 10100101 : 01010110 => 00111110
    if (a == 0xA5 && b == 0x56)
        return 62;

    // 10100101 : 01010101 => 00111111
    if (a == 0xA5 && b == 0x55)
        return 63;

    // 10011010 : 10101010 => 01000000
    if (a == 0x9A && b == 0xAA)
        return 64;

    // 10011010 : 10101001 => 01000001
    if (a == 0x9A && b == 0xA9)
        return 65;

    // 10011010 : 10100110 => 01000010
    if (a == 0x9A && b == 0xA6)
        return 66;

    // 10011010 : 10100101 => 01000011
    if (a == 0x9A && b == 0xA5)
        return 67;

    // 10011010 : 10011010 => 01000100
    if (a == 0x9A && b == 0x9A)
        return 68;

    // 10011010 : 10011001 => 01000101
    if (a == 0x9A && b == 0x99)
        return 69;

    // 10011010 : 10010110 => 01000110
    if (a == 0x9A && b == 0x96)
        return 70;

    // 10011010 : 10010101 => 01000111
    if (a == 0x9A && b == 0x95)
        return 71;

    // 10011010 : 01101010 => 01001000
    if (a == 0x9A && b == 0x6A)
        return 72;

    // 10011010 : 01101001 => 01001001
    if (a == 0x9A && b == 0x69)
        return 73;

    // 10011010 : 01100110 => 01001010
    if (a == 0x9A && b == 0x66)
        return 74;

    // 10011010 : 01100101 => 01001011
    if (a == 0x9A && b == 0x65)
        return 75;

    // 10011010 : 01011010 => 01001100
    if (a == 0x9A && b == 0x5A)
        return 76;

    // 10011010 : 01011001 => 01001101
    if (a == 0x9A && b == 0x59)
        return 77;

    // 10011010 : 01010110 => 01001110
    if (a == 0x9A && b == 0x56)
        return 78;

    // 10011010 : 01010101 => 01001111
    if (a == 0x9A && b == 0x55)
        return 79;

    // 10011001 : 10101010 => 01010000
    if (a == 0x99 && b == 0xAA)
        return 80;

    // 10011001 : 10101001 => 01010001
    if (a == 0x99 && b == 0xA9)
        return 81;

    // 10011001 : 10100110 => 01010010
    if (a == 0x99 && b == 0xA6)
        return 82;

    // 10011001 : 10100101 => 01010011
    if (a == 0x99 && b == 0xA5)
        return 83;

    // 10011001 : 10011010 => 01010100
    if (a == 0x99 && b == 0x9A)
        return 84;

    // 10011001 : 10011001 => 01010101
    if (a == 0x99 && b == 0x99)
        return 85;

    // 10011001 : 10010110 => 01010110
    if (a == 0x99 && b == 0x96)
        return 86;

    // 10011001 : 10010101 => 01010111
    if (a == 0x99 && b == 0x95)
        return 87;

    // 10011001 : 01101010 => 01011000
    if (a == 0x99 && b == 0x6A)
        return 88;

    // 10011001 : 01101001 => 01011001
    if (a == 0x99 && b == 0x69)
        return 89;

    // 10011001 : 01100110 => 01011010
    if (a == 0x99 && b == 0x66)
        return 90;

    // 10011001 : 01100101 => 01011011
    if (a == 0x99 && b == 0x65)
        return 91;

    // 10011001 : 01011010 => 01011100
    if (a == 0x99 && b == 0x5A)
        return 92;

    // 10011001 : 01011001 => 01011101
    if (a == 0x99 && b == 0x59)
        return 93;

    // 10011001 : 01010110 => 01011110
    if (a == 0x99 && b == 0x56)
        return 94;

    // 10011001 : 01010101 => 01011111
    if (a == 0x99 && b == 0x55)
        return 95;

    // 10010110 : 10101010 => 01100000
    if (a == 0x96 && b == 0xAA)
        return 96;

    // 10010110 : 10101001 => 01100001
    if (a == 0x96 && b == 0xA9)
        return 97;

    // 10010110 : 10100110 => 01100010
    if (a == 0x96 && b == 0xA6)
        return 98;

    // 10010110 : 10100101 => 01100011
    if (a == 0x96 && b == 0xA5)
        return 99;

    // 10010110 : 10011010 => 01100100
    if (a == 0x96 && b == 0x9A)
        return 100;

    // 10010110 : 10011001 => 01100101
    if (a == 0x96 && b == 0x99)
        return 101;

    // 10010110 : 10010110 => 01100110
    if (a == 0x96 && b == 0x96)
        return 102;

    // 10010110 : 10010101 => 01100111
    if (a == 0x96 && b == 0x95)
        return 103;

    // 10010110 : 01101010 => 01101000
    if (a == 0x96 && b == 0x6A)
        return 104;

    // 10010110 : 01101001 => 01101001
    if (a == 0x96 && b == 0x69)
        return 105;

    // 10010110 : 01100110 => 01101010
    if (a == 0x96 && b == 0x66)
        return 106;

    // 10010110 : 01100101 => 01101011
    if (a == 0x96 && b == 0x65)
        return 107;

    // 10010110 : 01011010 => 01101100
    if (a == 0x96 && b == 0x5A)
        return 108;

    // 10010110 : 01011001 => 01101101
    if (a == 0x96 && b == 0x59)
        return 109;

    // 10010110 : 01010110 => 01101110
    if (a == 0x96 && b == 0x56)
        return 110;

    // 10010110 : 01010101 => 01101111
    if (a == 0x96 && b == 0x55)
        return 111;

    // 10010101 : 10101010 => 01110000
    if (a == 0x95 && b == 0xAA)
        return 112;

    // 10010101 : 10101001 => 01110001
    if (a == 0x95 && b == 0xA9)
        return 113;

    // 10010101 : 10100110 => 01110010
    if (a == 0x95 && b == 0xA6)
        return 114;

    // 10010101 : 10100101 => 01110011
    if (a == 0x95 && b == 0xA5)
        return 115;

    // 10010101 : 10011010 => 01110100
    if (a == 0x95 && b == 0x9A)
        return 116;

    // 10010101 : 10011001 => 01110101
    if (a == 0x95 && b == 0x99)
        return 117;

    // 10010101 : 10010110 => 01110110
    if (a == 0x95 && b == 0x96)
        return 118;

    // 10010101 : 10010101 => 01110111
    if (a == 0x95 && b == 0x95)
        return 119;

    // 10010101 : 01101010 => 01111000
    if (a == 0x95 && b == 0x6A)
        return 120;

    // 10010101 : 01101001 => 01111001
    if (a == 0x95 && b == 0x69)
        return 121;

    // 10010101 : 01100110 => 01111010
    if (a == 0x95 && b == 0x66)
        return 122;

    // 10010101 : 01100101 => 01111011
    if (a == 0x95 && b == 0x65)
        return 123;

    // 10010101 : 01011010 => 01111100
    if (a == 0x95 && b == 0x5A)
        return 124;

    // 10010101 : 01011001 => 01111101
    if (a == 0x95 && b == 0x59)
        return 125;

    // 10010101 : 01010110 => 01111110
    if (a == 0x95 && b == 0x56)
        return 126;

    // 10010101 : 01010101 => 01111111
    if (a == 0x95 && b == 0x55)
        return 127;

    // 01101010 : 10101010 => 10000000
    if (a == 0x6A && b == 0xAA)
        return 128;

    // 01101010 : 10101001 => 10000001
    if (a == 0x6A && b == 0xA9)
        return 129;

    // 01101010 : 10100110 => 10000010
    if (a == 0x6A && b == 0xA6)
        return 130;

    // 01101010 : 10100101 => 10000011
    if (a == 0x6A && b == 0xA5)
        return 131;

    // 01101010 : 10011010 => 10000100
    if (a == 0x6A && b == 0x9A)
        return 132;

    // 01101010 : 10011001 => 10000101
    if (a == 0x6A && b == 0x99)
        return 133;

    // 01101010 : 10010110 => 10000110
    if (a == 0x6A && b == 0x96)
        return 134;

    // 01101010 : 10010101 => 10000111
    if (a == 0x6A && b == 0x95)
        return 135;

    // 01101010 : 01101010 => 10001000
    if (a == 0x6A && b == 0x6A)
        return 136;

    // 01101010 : 01101001 => 10001001
    if (a == 0x6A && b == 0x69)
        return 137;

    // 01101010 : 01100110 => 10001010
    if (a == 0x6A && b == 0x66)
        return 138;

    // 01101010 : 01100101 => 10001011
    if (a == 0x6A && b == 0x65)
        return 139;

    // 01101010 : 01011010 => 10001100
    if (a == 0x6A && b == 0x5A)
        return 140;

    // 01101010 : 01011001 => 10001101
    if (a == 0x6A && b == 0x59)
        return 141;

    // 01101010 : 01010110 => 10001110
    if (a == 0x6A && b == 0x56)
        return 142;

    // 01101010 : 01010101 => 10001111
    if (a == 0x6A && b == 0x55)
        return 143;

    // 01101001 : 10101010 => 10010000
    if (a == 0x69 && b == 0xAA)
        return 144;

    // 01101001 : 10101001 => 10010001
    if (a == 0x69 && b == 0xA9)
        return 145;

    // 01101001 : 10100110 => 10010010
    if (a == 0x69 && b == 0xA6)
        return 146;

    // 01101001 : 10100101 => 10010011
    if (a == 0x69 && b == 0xA5)
        return 147;

    // 01101001 : 10011010 => 10010100
    if (a == 0x69 && b == 0x9A)
        return 148;

    // 01101001 : 10011001 => 10010101
    if (a == 0x69 && b == 0x99)
        return 149;

    // 01101001 : 10010110 => 10010110
    if (a == 0x69 && b == 0x96)
        return 150;

    // 01101001 : 10010101 => 10010111
    if (a == 0x69 && b == 0x95)
        return 151;

    // 01101001 : 01101010 => 10011000
    if (a == 0x69 && b == 0x6A)
        return 152;

    // 01101001 : 01101001 => 10011001
    if (a == 0x69 && b == 0x69)
        return 153;

    // 01101001 : 01100110 => 10011010
    if (a == 0x69 && b == 0x66)
        return 154;

    // 01101001 : 01100101 => 10011011
    if (a == 0x69 && b == 0x65)
        return 155;

    // 01101001 : 01011010 => 10011100
    if (a == 0x69 && b == 0x5A)
        return 156;

    // 01101001 : 01011001 => 10011101
    if (a == 0x69 && b == 0x59)
        return 157;

    // 01101001 : 01010110 => 10011110
    if (a == 0x69 && b == 0x56)
        return 158;

    // 01101001 : 01010101 => 10011111
    if (a == 0x69 && b == 0x55)
        return 159;

    // 01100110 : 10101010 => 10100000
    if (a == 0x66 && b == 0xAA)
        return 160;

    // 01100110 : 10101001 => 10100001
    if (a == 0x66 && b == 0xA9)
        return 161;

    // 01100110 : 10100110 => 10100010
    if (a == 0x66 && b == 0xA6)
        return 162;

    // 01100110 : 10100101 => 10100011
    if (a == 0x66 && b == 0xA5)
        return 163;

    // 01100110 : 10011010 => 10100100
    if (a == 0x66 && b == 0x9A)
        return 164;

    // 01100110 : 10011001 => 10100101
    if (a == 0x66 && b == 0x99)
        return 165;

    // 01100110 : 10010110 => 10100110
    if (a == 0x66 && b == 0x96)
        return 166;

    // 01100110 : 10010101 => 10100111
    if (a == 0x66 && b == 0x95)
        return 167;

    // 01100110 : 01101010 => 10101000
    if (a == 0x66 && b == 0x6A)
        return 168;

    // 01100110 : 01101001 => 10101001
    if (a == 0x66 && b == 0x69)
        return 169;

    // 01100110 : 01100110 => 10101010
    if (a == 0x66 && b == 0x66)
        return 170;

    // 01100110 : 01100101 => 10101011
    if (a == 0x66 && b == 0x65)
        return 171;

    // 01100110 : 01011010 => 10101100
    if (a == 0x66 && b == 0x5A)
        return 172;

    // 01100110 : 01011001 => 10101101
    if (a == 0x66 && b == 0x59)
        return 173;

    // 01100110 : 01010110 => 10101110
    if (a == 0x66 && b == 0x56)
        return 174;

    // 01100110 : 01010101 => 10101111
    if (a == 0x66 && b == 0x55)
        return 175;

    // 01100101 : 10101010 => 10110000
    if (a == 0x65 && b == 0xAA)
        return 176;

    // 01100101 : 10101001 => 10110001
    if (a == 0x65 && b == 0xA9)
        return 177;

    // 01100101 : 10100110 => 10110010
    if (a == 0x65 && b == 0xA6)
        return 178;

    // 01100101 : 10100101 => 10110011
    if (a == 0x65 && b == 0xA5)
        return 179;

    // 01100101 : 10011010 => 10110100
    if (a == 0x65 && b == 0x9A)
        return 180;

    // 01100101 : 10011001 => 10110101
    if (a == 0x65 && b == 0x99)
        return 181;

    // 01100101 : 10010110 => 10110110
    if (a == 0x65 && b == 0x96)
        return 182;

    // 01100101 : 10010101 => 10110111
    if (a == 0x65 && b == 0x95)
        return 183;

    // 01100101 : 01101010 => 10111000
    if (a == 0x65 && b == 0x6A)
        return 184;

    // 01100101 : 01101001 => 10111001
    if (a == 0x65 && b == 0x69)
        return 185;

    // 01100101 : 01100110 => 10111010
    if (a == 0x65 && b == 0x66)
        return 186;

    // 01100101 : 01100101 => 10111011
    if (a == 0x65 && b == 0x65)
        return 187;

    // 01100101 : 01011010 => 10111100
    if (a == 0x65 && b == 0x5A)
        return 188;

    // 01100101 : 01011001 => 10111101
    if (a == 0x65 && b == 0x59)
        return 189;

    // 01100101 : 01010110 => 10111110
    if (a == 0x65 && b == 0x56)
        return 190;

    // 01100101 : 01010101 => 10111111
    if (a == 0x65 && b == 0x55)
        return 191;

    // 01011010 : 10101010 => 11000000
    if (a == 0x5A && b == 0xAA)
        return 192;

    // 01011010 : 10101001 => 11000001
    if (a == 0x5A && b == 0xA9)
        return 193;

    // 01011010 : 10100110 => 11000010
    if (a == 0x5A && b == 0xA6)
        return 194;

    // 01011010 : 10100101 => 11000011
    if (a == 0x5A && b == 0xA5)
        return 195;

    // 01011010 : 10011010 => 11000100
    if (a == 0x5A && b == 0x9A)
        return 196;

    // 01011010 : 10011001 => 11000101
    if (a == 0x5A && b == 0x99)
        return 197;

    // 01011010 : 10010110 => 11000110
    if (a == 0x5A && b == 0x96)
        return 198;

    // 01011010 : 10010101 => 11000111
    if (a == 0x5A && b == 0x95)
        return 199;

    // 01011010 : 01101010 => 11001000
    if (a == 0x5A && b == 0x6A)
        return 200;

    // 01011010 : 01101001 => 11001001
    if (a == 0x5A && b == 0x69)
        return 201;

    // 01011010 : 01100110 => 11001010
    if (a == 0x5A && b == 0x66)
        return 202;

    // 01011010 : 01100101 => 11001011
    if (a == 0x5A && b == 0x65)
        return 203;

    // 01011010 : 01011010 => 11001100
    if (a == 0x5A && b == 0x5A)
        return 204;

    // 01011010 : 01011001 => 11001101
    if (a == 0x5A && b == 0x59)
        return 205;

    // 01011010 : 01010110 => 11001110
    if (a == 0x5A && b == 0x56)
        return 206;

    // 01011010 : 01010101 => 11001111
    if (a == 0x5A && b == 0x55)
        return 207;

    // 01011001 : 10101010 => 11010000
    if (a == 0x59 && b == 0xAA)
        return 208;

    // 01011001 : 10101001 => 11010001
    if (a == 0x59 && b == 0xA9)
        return 209;

    // 01011001 : 10100110 => 11010010
    if (a == 0x59 && b == 0xA6)
        return 210;

    // 01011001 : 10100101 => 11010011
    if (a == 0x59 && b == 0xA5)
        return 211;

    // 01011001 : 10011010 => 11010100
    if (a == 0x59 && b == 0x9A)
        return 212;

    // 01011001 : 10011001 => 11010101
    if (a == 0x59 && b == 0x99)
        return 213;

    // 01011001 : 10010110 => 11010110
    if (a == 0x59 && b == 0x96)
        return 214;

    // 01011001 : 10010101 => 11010111
    if (a == 0x59 && b == 0x95)
        return 215;

    // 01011001 : 01101010 => 11011000
    if (a == 0x59 && b == 0x6A)
        return 216;

    // 01011001 : 01101001 => 11011001
    if (a == 0x59 && b == 0x69)
        return 217;

    // 01011001 : 01100110 => 11011010
    if (a == 0x59 && b == 0x66)
        return 218;

    // 01011001 : 01100101 => 11011011
    if (a == 0x59 && b == 0x65)
        return 219;

    // 01011001 : 01011010 => 11011100
    if (a == 0x59 && b == 0x5A)
        return 220;

    // 01011001 : 01011001 => 11011101
    if (a == 0x59 && b == 0x59)
        return 221;

    // 01011001 : 01010110 => 11011110
    if (a == 0x59 && b == 0x56)
        return 222;

    // 01011001 : 01010101 => 11011111
    if (a == 0x59 && b == 0x55)
        return 223;

    // 01010110 : 10101010 => 11100000
    if (a == 0x56 && b == 0xAA)
        return 224;

    // 01010110 : 10101001 => 11100001
    if (a == 0x56 && b == 0xA9)
        return 225;

    // 01010110 : 10100110 => 11100010
    if (a == 0x56 && b == 0xA6)
        return 226;

    // 01010110 : 10100101 => 11100011
    if (a == 0x56 && b == 0xA5)
        return 227;

    // 01010110 : 10011010 => 11100100
    if (a == 0x56 && b == 0x9A)
        return 228;

    // 01010110 : 10011001 => 11100101
    if (a == 0x56 && b == 0x99)
        return 229;

    // 01010110 : 10010110 => 11100110
    if (a == 0x56 && b == 0x96)
        return 230;

    // 01010110 : 10010101 => 11100111
    if (a == 0x56 && b == 0x95)
        return 231;

    // 01010110 : 01101010 => 11101000
    if (a == 0x56 && b == 0x6A)
        return 232;

    // 01010110 : 01101001 => 11101001
    if (a == 0x56 && b == 0x69)
        return 233;

    // 01010110 : 01100110 => 11101010
    if (a == 0x56 && b == 0x66)
        return 234;

    // 01010110 : 01100101 => 11101011
    if (a == 0x56 && b == 0x65)
        return 235;

    // 01010110 : 01011010 => 11101100
    if (a == 0x56 && b == 0x5A)
        return 236;

    // 01010110 : 01011001 => 11101101
    if (a == 0x56 && b == 0x59)
        return 237;

    // 01010110 : 01010110 => 11101110
    if (a == 0x56 && b == 0x56)
        return 238;

    // 01010110 : 01010101 => 11101111
    if (a == 0x56 && b == 0x55)
        return 239;

    // 01010101 : 10101010 => 11110000
    if (a == 0x55 && b == 0xAA)
        return 240;

    // 01010101 : 10101001 => 11110001
    if (a == 0x55 && b == 0xA9)
        return 241;

    // 01010101 : 10100110 => 11110010
    if (a == 0x55 && b == 0xA6)
        return 242;

    // 01010101 : 10100101 => 11110011
    if (a == 0x55 && b == 0xA5)
        return 243;

    // 01010101 : 10011010 => 11110100
    if (a == 0x55 && b == 0x9A)
        return 244;

    // 01010101 : 10011001 => 11110101
    if (a == 0x55 && b == 0x99)
        return 245;

    // 01010101 : 10010110 => 11110110
    if (a == 0x55 && b == 0x96)
        return 246;

    // 01010101 : 10010101 => 11110111
    if (a == 0x55 && b == 0x95)
        return 247;

    // 01010101 : 01101010 => 11111000
    if (a == 0x55 && b == 0x6A)
        return 248;

    // 01010101 : 01101001 => 11111001
    if (a == 0x55 && b == 0x69)
        return 249;

    // 01010101 : 01100110 => 11111010
    if (a == 0x55 && b == 0x66)
        return 250;

    // 01010101 : 01100101 => 11111011
    if (a == 0x55 && b == 0x65)
        return 251;

    // 01010101 : 01011010 => 11111100
    if (a == 0x55 && b == 0x5A)
        return 252;

    // 01010101 : 01011001 => 11111101
    if (a == 0x55 && b == 0x59)
        return 253;

    // 01010101 : 01010110 => 11111110
    if (a == 0x55 && b == 0x56)
        return 254;

    // 01010101 : 01010101 => 11111111
    if (a == 0x55 && b == 0x55)
        return 255;

    return 0;
}
