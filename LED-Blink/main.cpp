#include <cox.h>

Timer tBlink;

static void taskBlink(void *) {
  System.ledToggle(0);
}

void setup() {
  tBlink.onFired(taskBlink, NULL);
  tBlink.startPeriodic(1000);
}
