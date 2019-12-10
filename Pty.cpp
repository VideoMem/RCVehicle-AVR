#include "Pty.h"

void T_gcode::blank() {
    g = 0;
    m = 0;
    s = 0;
    x = 0;
    y = 0;
    u = 0;
    v = 0;
    b = 0;
}

void Pty::gstatus() {
    mapSerial->printn("MDI status:");
    mapSerial->printn("G ", code.g);
    mapSerial->printn("M ", code.m);
    mapSerial->printn("S ", code.s);
    mapSerial->printn("X ", code.x);
    mapSerial->printn("Y ", code.y);
    mapSerial->printn("U ", code.u);
    mapSerial->printn("V ", code.v);
    mapSerial->printn("B ", code.b);
}

void Pty::gcode(T_gcode &g) {
    memcpy(&g, &code, sizeof(T_gcode));
}

void Pty::submit(char rcode, char (&number) [STRBUF_LEN]) {
    int value = atoi(number);
    //mapSerial->print("Submit: ");
    //mapSerial->print(rcode);
    //mapSerial->printn(number);
    lastCode = rcode;
    switch(rcode) {
        case 'g':
            code.g = value;
        break;
        case 'm':
            code.m = value;
        break;
        case 's':
            code.s = value;
        break;
        case 'x':
            code.x = value;
        break;
        case 'y':
            code.y = value;
        break;
        case 'u':
            code.u = value;
        break;
        case 'v':
            code.v = value;
        break;
        case 'b':
            code.b = value;
        break;
        default:
        break;
    }
}

bool Pty::decode() {
    uint8_t index = 0;
    char rcode = 0;
    char chr;
    char number[STRBUF_LEN]; blank(number);
    error = false;
    int i, idx = 0;
    //mapSerial->print("to decode:");
    //mapSerial->printn(get());
    for(i = 0; get()[i]; ++i) {
        chr = get()[i];
        switch((unsigned char) chr) {
            case 32:
            break;
            case 71:
            case 103:
                if(rcode != 0)
                   submit(rcode, number);
                blank(number); idx = 0;
                rcode = 'g';
                //mapSerial->print("G code found\n");
            break;
            case 77:
            case 109:
                if(rcode != 0)
                     submit(rcode, number);
                blank(number); idx = 0;
                rcode = 'm';
                //mapSerial->print("M code found\n");
            break;
            case 83:
            case 115:
                if(rcode != 0)
                     submit(rcode, number);
                blank(number); idx = 0;
                rcode = 's';
                //mapSerial->print("S code found\n");
            break;
            case 88:
            case 120:
                if(rcode != 0)
                     submit(rcode, number);
                blank(number); idx = 0;
                rcode = 'x';
                //mapSerial->print("X code found\n");
            break;
            case 89:
            case 121:
                if(rcode != 0)
                     submit(rcode, number);
                blank(number); idx = 0;
                rcode = 'y';
                //mapSerial->print("Y code found\n");
            break;
            case 85:
            case 117:
                if(rcode != 0)
                     submit(rcode, number);
                blank(number); idx = 0;
                rcode = 'u';
                //mapSerial->print("U code found\n");
            break;
            case 86:
            case 118:
                if(rcode != 0)
                    submit(rcode, number);
                blank(number); idx = 0;
                rcode = 'v';
                //mapSerial->print("V code found\n");
            break;
            case 66:
            case 98:
                if(rcode != 0)
                    submit(rcode, number);
                blank(number); idx = 0;
                rcode = 'b';
                //mapSerial->print("B code found\n");
            break;
            case 45: // -
            case 46: // .
            case 48: // 0
            case 49: // 1
            case 50: // 2
            case 51: // 3
            case 52: // 4
            case 53: // 5
            case 54: // 6
            case 55: // 7
            case 56: // 8
            case 57: // 9
                number[idx] = chr;
                ++idx;
            break;
            default:
                error=true;
                mapSerial->print("Unknown char ");
                mapSerial->print(chr);
                mapSerial->printn(" at col ", index);
                blank(number); idx = 0;
                rcode = 0;
        }
        index++;
    }
    //mapSerial->print(index);
    if(rcode != 0)
        submit(rcode, number);
    return !error;
}

void Pty::update() {
    int size2read;
    char read;

    do {
        size2read = mapSerial->getDirect() ? mapSerial->available() : mapSerial->available() / 2;
        read = mapSerial->read();

        switch((uint8_t) read) {
            case 13: //enter
                exec();
                clear();
            break;
            case 127: //backspace
                mapSerial->print("\r");
                for(uint8_t i = 0; i < size(); ++i)
                    mapSerial->print(" ");
                mapSerial->print("\r");
                del();
                mapSerial->print(get());
            break;
            case 255:
            break;
            default: //is printable?
                if(read >= 32 && read <= 126) {
                    if (echo) mapSerial->print(read);
                    if(read < 45 || read > 57) //is not number?
                        if(size() > 1) {
                            exec();
                            clear();
                        }
                    add(read);
                }
            break;
        }
    } while(size2read > 0);

}

void Pty::exec() {
    //mapSerial->print("\r\n");
    //dump(get());
    //mapSerial->print("\r\n");
    if(size() > 0) {
        if(decode()) {
            //gstatus();
        } else {
            mapSerial->print(get());
            mapSerial->print("(");
            mapSerial->print(size());
            mapSerial->print(")");
            mapSerial->print(": command not found!\r\n");
        }
    } else {
        mapSerial->print("\r\n");
        gstatus();
    }
}