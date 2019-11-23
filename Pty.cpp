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

void Pty::setup() {
    code.blank();
}

void Pty::gcode(T_gcode &g) {
    memcpy(&g, &code, sizeof(T_gcode));
}

void Pty::submit(char rcode, char (&num) [STRBUF_LEN]) {
    String number = num;
    int value = atoi(number.c_str());
    mapSerial->print("Submit: ");
    mapSerial->print(rcode);
    mapSerial->printn(number.c_str());
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

void Pty::blank(char (&number) [STRBUF_LEN]) {
    memset(number, 0, STRBUF_LEN);
}

bool Pty::decode(char (&p) [STRBUF_LEN]) {
    uint8_t index = 0;
    char rcode = 0;
    char chr;
    char number[STRBUF_LEN]; blank(number);
    uint8_t nidx = 0;
    error = false;
    int i;

    for(i = 0; p[i]; ++i) {
        chr = p[i];
        switch((unsigned char) chr) {
            case 32:
            break;
            case 71:
            case 103:
                if(rcode != 0)
                   submit(rcode, number);
                nidx = 0;
                blank(number);
                rcode = 'g';
                //mapSerial->print("G code found\n");
            break;
            case 77:
            case 109:
                if(rcode != 0)
                     submit(rcode, number);
                nidx = 0;
                blank(number);
                rcode = 'm';
                //mapSerial->print("M code found\n");
            break;
            case 83:
            case 115:
                if(rcode != 0)
                     submit(rcode, number);
                nidx = 0;
                blank(number);
                rcode = 's';
                //mapSerial->print("S code found\n");
            break;
            case 88:
            case 120:
                if(rcode != 0)
                     submit(rcode, number);
                nidx = 0;
                blank(number);
                rcode = 'x';
                //mapSerial->print("X code found\n");
            break;
            case 89:
            case 121:
                if(rcode != 0)
                     submit(rcode, number);
                nidx = 0;
                blank(number);
                rcode = 'y';
                //mapSerial->print("Y code found\n");
            break;
            case 85:
            case 117:
                if(rcode != 0)
                     submit(rcode, number);
                nidx = 0;
                blank(number);
                rcode = 'u';
                //mapSerial->print("U code found\n");
            break;
            case 86:
            case 118:
                if(rcode != 0)
                    submit(rcode, number);
                nidx = 0;
                blank(number);
                rcode = 'v';
                //mapSerial->print("V code found\n");
            break;
            case 66:
            case 98:
                if(rcode != 0)
                    submit(rcode, number);
                nidx = 0;
                blank(number);
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
                //mapSerial->print ("Number build : ");
                number[nidx] = chr;
                //mapSerial->printn(chr);
                nidx++;
                //number[nidx] = 0;
            break;
            default:
                error=true;
                mapSerial->print("Unknown char ");
                mapSerial->print(chr);
                mapSerial->printn(" at col ", index);
                if(rcode != 0)
                    submit(rcode, number);
                nidx = 0;
                blank(number);
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
    char input[STRBUF_LEN];
    memset(input, 0, STRBUF_LEN);

    do {
        size2read = mapSerial->getDirect() ? mapSerial->available() : mapSerial->available() / 2;
        read = mapSerial->read();

        switch((uint8_t) read) {
            case 13:
                exec();
                stdin->clear();
            break;
            case 127:
                mapSerial->print("\r");
                for(uint8_t i = 0; i < stdin->size(); ++i)
                    mapSerial->print(" ");
                mapSerial->print("\r");
                stdin->del();
                stdin->get(input);
                mapSerial->print(input);
            break;
            case 255:
            break;
            default:
                if(read >= 32 && read <= 126) {
                    stdin->add(read);
                    mapSerial->print(read);
                    if(read < 45 || read > 57)
                        if(stdin->size() > 1) {
                            //exec();
                            //stdin->clear();
                        }
                }
            break;
        }
    } while(size2read > 0);

}

void Pty::exec() {
    char input[STRBUF_LEN];
    memset(input, 0, STRBUF_LEN);
    stdin->dump();
    mapSerial->print("\r\n");
    stdin->get(input);
    if(decode(input)) {
        gstatus();
        //vfd->target(0, code.s);
    } else {
        mapSerial->print(input);
        mapSerial->print("(");
        mapSerial->print(stdin->size());
        mapSerial->print(")");
        mapSerial->print(": command not found!\n");
    }
}