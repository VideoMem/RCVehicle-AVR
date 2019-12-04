#include "uTimers.h"
#include <Arduino.h>

uTimer::uTimer() {
  elapsed = 0;
  enabled = 0;
  preset = 0;
  last_read = 0;
  trigged = 0;
}

void uTimer::check() {
 if (preset == 0)
    enabled = 0; 
 else
    if (elapsed > preset) {
      trigged = 1;
      elapsed = 0;
      enabled = 0;
    } 
}

void uTimer::update() {
   unsigned long read = micros();

   if (enabled == 1) {   
     // read overflow
     if (read < last_read)
       elapsed += read;
     else {
       elapsed += read - last_read;
     }     
   }
   
   last_read = read;
   check();

}

unsigned long uTimer::valueUS() {
  return elapsed; 
}

unsigned long uTimer::valueMS() {
  return elapsed / 1000; 
}

int uTimer::event() {
  if (trigged == 1) {
     trigged = 0;
     enabled = 1;
     return 1;
  }
  return 0;
}

void uTimer::setUS(unsigned long value) {
   preset = value; 
   enabled = 1;
}

void uTimer::setMS(unsigned int s) {
  unsigned long aux = s * 1000;
  setMS(aux);
}

void uTimer::reset() {
  update();
  elapsed = 0;
  trigged = 0;
}

void uTimer::enable() {
  reset();
}

void uTimer::disable() {
  reset();
  enabled = 0;
}

