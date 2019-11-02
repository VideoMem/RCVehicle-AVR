#include "StringBuffer.h"

StringBuffer::StringBuffer(char* p) {
    buff = p;
    clear();
}

void StringBuffer::blank() {
    memset(buff, 0, sizeof(buff));
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
    //if(idx == 1) add(str); //hack
}

void StringBuffer::del() {
    if (idx > 0) --idx;
}
