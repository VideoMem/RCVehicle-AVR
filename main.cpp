#include <Arduino.h>
#include "Timers.h"
#include "Toggle.h"
#include "Battery.h"
#include "Manchester.h"
#include "MapSerial.h"
#include "VFD.h"
#include "Pty.h"

//#ifdef __cplusplus
//extern "C" {
//#include <avr/avr_mcu_section.h>
//AVR_MCU(F_CPU, "atmega328p");
//}
//#endif

#define BOOT_DELAY 100
#define NO_ERROR 500
#define ON_ERROR 50
#define MINUTE 60
#define BAUDRATE 9600


const unsigned char ledPin = 13;                             // LED connected to digital pin 13
static unsigned char error = 0;

Timer blinkTimer;
Manchester* mapSerial = new Manchester();
Battery battery(mapSerial);
Motor* Motor::pointer;//
volatile Motor mstate;
volatile int counter = 0;
VFD* Drive = new VFD(mapSerial);


static char prompt[STRBUF_LEN];
Pty* pty = new Pty(mapSerial, prompt);

Toggle blinker;

ISR (TIMER1_COMPA_vect) {
    Motor::pointer->advance(0);
}


void setup() {
    Serial.begin(BAUDRATE);
    mapSerial->setDirect(true);
    mapSerial->print("Initializing ... \n");
    pinMode(ledPin, OUTPUT);
	blinkTimer.setMS(NO_ERROR);
    battery.check();


    Serial.flush();
    Drive->setup();
    mapSerial->print("Done\n");
}

void ledDrive() {
    if(blinker.value() == true)
        digitalWrite(ledPin, HIGH);   // set the LED on
    else
        digitalWrite(ledPin, LOW);    // set the LED off

    if(blinkTimer.event()) {
        blinker.change();
        blinkTimer.reset();
    }
    
    blinkTimer.update();
}

void errorDrive() {
    if(error == 1) {
        blinkTimer.setMS(ON_ERROR);         
    } else {
        blinkTimer.setMS(NO_ERROR);         
    }
}

void motorDrive() {
    Drive->update();
}

void loop() {
    ledDrive();
    battery.update();
    errorDrive();
    motorDrive();
    pty->update();
}

int main(void) {
    init();
    setup();

    for (;;) {
       loop();
    }
}

