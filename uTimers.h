/*
*  utimers.h
*
*/

#ifndef UTIMERS_H
#define UTIMERS_H
#define UTIMERS_VERSION "0"

class uTimer {
 public: 
   uTimer();
   void setUS(unsigned long ms);
   void setMS(unsigned int s);
   void reset();
   void enable();
   void disable();
   void update();
   unsigned long valueUS();
   unsigned long valueMS();
   int event();
   unsigned long value() { return elapsed; }
   
 private:
  void check();
  unsigned long last_read;
  unsigned long elapsed;
  unsigned long preset;
  int enabled; 
  int trigged;
};

#endif
