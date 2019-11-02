#include "Pty.h"

void Pty::setup() {
}

void Pty::update() {
    int size2read;
    char read;

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
                mapSerial->print(stdin->get());

            break;
            case 255:
            break;
            default:
                if(read >= 32 && read <= 126 && stdin->size() < STRBUF_LEN) {
                    stdin->add(read);
                    mapSerial->print(read);
                }
            break;
        }
    } while(size2read > 0);

}

void Pty::exec() {
    stdin->dump();
    mapSerial->print("\n");
    //stdin->del();
    mapSerial->print(stdin->get());
    mapSerial->print("(");
    mapSerial->print(stdin->size());
    mapSerial->print(")");
    mapSerial->print(": command not found!\n");
}