#include <cox.h>

Timer tBlink;
Timer tOff;

static void taskOn(void *) {
  System.ledOn(0);
  tOff.startOneShot(10);
}

static void taskOff(void *) {
  System.ledOff(0);
}

void setup() {
  tOff.onFired(taskOff, NULL);
  tBlink.onFired(taskOn, NULL);
  tBlink.startPeriodic(1000);
}
