/*
*  mapSerial.h
*
*/

#ifndef MAPSERIAL_H
#define MAPSERIAL_H
#define MAPSERIAL_VERSION "0"
#include "Module.h"
#include "Manchester.h"


class MapSerial: public Module {
    public:
        MapSerial(Manchester* s) { mapSerial = s; };
    protected:
        Manchester* mapSerial;
};

#endif