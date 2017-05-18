#include <cox.h>

Timer timerHello;
Timer timerLEDOff;
BLEMac *ble;

static const uint8_t beaconInfo[0x17] = {
  0x02, //Beacon
  0x15, //Length
  0x01, 0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78, 0x89, 0x9a, 0xab, 0xbc, 0xcd, 0xde, 0xef, 0xf0, //UUID
  0x01, 0x02, //Major
  0x03, 0x04, //Minor
  0xC3, // The Beacon's measured TX power in this implementation.
};

static void taskHello(void *) {
  System.ledOn(0);
  timerLEDOff.startOneShot(10);
}

static void taskLEDOff(void *) {
  System.ledOff(0);
}

static void eventButtonPressed() {
  if (ble->isAdvertising()) {
    ble->endAdvertise();
    timerHello.stop();
  } else {
    if (ble->beginAdvertise() == ERROR_SUCCESS) {
      timerHello.startPeriodic(1000);
    }
  }
}

void setup() {
  System.ledOn(0);

  System.onButtonPressed(0, eventButtonPressed);
  timerHello.onFired(taskHello, NULL);
  timerLEDOff.onFired(taskLEDOff, NULL);

  uint32_t ramReq;
  ble = System.initSoftDevice(0, 0, &ramReq);
  if (ble) {
    ble->setManufacturerData(
      0x0059, // Nordic Semiconductor
      beaconInfo,
      sizeof(beaconInfo)
    );
    error_t err = ble->beginAdvertise();
    if (err == ERROR_SUCCESS) {
      timerHello.startPeriodic(1000);
    }
  }
}
