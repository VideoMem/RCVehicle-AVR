#include "StringBuffer.h"

StringBuffer::StringBuffer(Manchester* m):MapSerial(m) {
    clear();
}

void StringBuffer::clear() {
    idx=0;
    blank();
}

void StringBuffer::add(char str) {
    buff[idx] = str;
    ++idx;
    idx = idx >= STRBUF_LEN ? STRBUF_LEN - 1 : idx;
    buff[idx] = 0; //null term
}

void StringBuffer::del() {
    if (idx > 0) --idx;
    buff[idx] = 0; //null term
}
