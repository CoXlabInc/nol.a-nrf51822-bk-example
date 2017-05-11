#include <cox.h>

BLEBeacon &Beacon = getBLEBeaconInstance();

Timer timerBlink, timerCheckButton;

static void taskBlink(void *) {
  if (digitalRead(P_RLED) == HIGH) {
    digitalWrite(P_RLED, LOW);
    timerBlink.startOneShot(10);
  } else {
    digitalWrite(P_RLED, HIGH);
    timerBlink.startOneShot(990);
  }
}

#ifdef PUSH_TO_BEACON
static void taskCheckButton(void *) {
  if (digitalRead(P_SW2) == HIGH) {
    /* Button 1 is released. */
    if (Beacon.isRunning()) {
      Beacon.stop();
      digitalWrite(P_GLED, HIGH);
    }

  } else {
    /* Button 1 is pressed. */
    if (!Beacon.isRunning()) {
      Beacon.resume();
      digitalWrite(P_GLED, LOW);
    }
  }
}
#endif //PUSH_TO_BEACON

void setup() {
  /* LEDs */
  pinMode(P_RLED, OUTPUT);
  digitalWrite(P_RLED, HIGH);
  pinMode(P_GLED, OUTPUT);
  digitalWrite(P_GLED, HIGH);

  /* Button */
  pinMode(P_SW2, INPUT_PULLUP);

  uint8_t advAddr[6] = { 'C', 'O', 'X', 'L', 'A', 'B' };
  uint8_t uuid[16] = {
      0x02, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
      0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    };
  uint16_t companyId = 0xABCD;
  uint16_t major = 0x0000;
  uint16_t minor = 0x1111;
  int8_t txPower = 0;

  nRFRadio.begin();
  Beacon.begin(nRFRadio, advAddr, companyId, uuid, major, minor, txPower);
  Beacon.setInterval(1);

  timerBlink.onFired(taskBlink, NULL);
  timerBlink.startOneShot(10);

#ifdef PUSH_TO_BEACON
  Beacon.stop();

  timerCheckButton.onFired(taskCheckButton, NULL);
  timerCheckButton.startPeriodic(10);
#endif //PUSH_TO_BEACON
}
