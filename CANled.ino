#include <Adafruit_MCP2515.h>
#include <Adafruit_NeoPixel.h>

#include "src/solid.h"

#include <RPi_Pico_TimerInterrupt.h>

#define CS_PIN PIN_CAN_CS
#define CAN_BAUDRATE (1000000)

#define CANID_TYPE(id) ((id >> 24) & 0x1f)
#define CANID_TYPE_ROBOT 1

#define CANID_MFG(id) ((id >> 16) & 0xff)
#define CANID_MFG_NI 1

#define CANID_API(id) ((id >> 10) & 0x3f)
#define CANID_INDEX(id) ((id >> 6) & 0xf)
#define CANID_DEVICE(id) (id & 0x3f)

#define LED_COUNT 100

Adafruit_MCP2515 mcp(CS_PIN);
Adafruit_NeoPixel neopixel(100, 5, NEO_RGBW + NEO_KHZ800);

Adafruit_NeoPixel statuspix(1, PIN_NEOPIXEL, NEO_RGB);

// eventually set them to the stored default values set in config
bool printMessages = false;
bool runFunctions = true;
uint8_t brightness = 20;
uint16_t LEDCount = 100;

void setup() {
  mcp.onReceive(PIN_CAN_INTERRUPT, rx);

  pinMode(NEOPIXEL_POWER, OUTPUT);
  digitalWrite(NEOPIXEL_POWER, HIGH);

  statuspix.begin();
  statuspix.clear();
  statuspix.show();
}

void loop() {
  if ((millis()/500)%2) {
    statuspix.setPixelColor(0, 10,10,0);
  } else {
    statuspix.clear();
  }
  statuspix.show();
}

RPI_PICO_Timer ITimer(0);

Stack stack(100);

bool TimerHandler(struct repeating_timer *t) {
  for (int i = 0; i < stack.getSize(); i++) {
    Pixel c = stack.get(i);
    uint8_t a = c.getAlpha();

    if (a > 0) {
      neopixel.setPixelColor(i, c.getRed(), c.getGreen(), c.getBlue(), c.getWhite());
    } else {
      neopixel.setPixelColor(i, 0, 0, 0);
    }
  }
  stack.next();
  neopixel.show();
  return true;
}

void animate(uint32_t index, uint64_t data) {
  switch (index) {
    case 0: { // set color
      uint8_t r = data & 0xFF;
      uint8_t g = (data >> 8) & 0xFF;
      uint8_t b = (data >> 16) & 0xFF;
      uint8_t w = (data >> 24) & 0xFF;

      Pixel color = Pixel(r, g, b, w);
      Solid *solid = new Solid(stack.getSize(), color);
      stack.Push(solid);
      break;
    }
    case 1: { // reset LEDs
      stack.Clear();
      break;
    }
    case 2: { // rotate or bounce color
      uint8_t r = data & 0xFF;
      uint8_t g = (data >> 8) & 0xFF;
      uint8_t b = (data >> 16) & 0xFF;
      uint8_t w = (data >> 24) & 0xFF;
      uint16_t speedBinary = (data >> 32) & 0xFFFF;
      uint8_t segmentCount = (data >> 48) & 0x3;
      uint8_t mode = (data >> 56) & 0x3;

      float speed = speedBinary / 6553.5;
      segmentCount += 1;

      Pixel color = Pixel(r, g, b, w);
      Chase *chase = new Chase(stack.getSize(), color, (mode >= 2));
      stack.Push(chase);
      break;
    }
    case 3: // rotate a rainbow color
      break;
    case 4: { // flash color
      uint8_t r = data & 0xFF;
      uint8_t g = (data >> 8) & 0xFF;
      uint8_t b = (data >> 16) & 0xFF;
      uint8_t w = (data >> 24) & 0xFF;
      uint16_t speedBinary = (data >> 32) & 0xFFFF;
      uint8_t durationBinary = (data >> 48) & 0x3;

      float speed = speedBinary / 13107;
      float duration = durationBinary / 6553.5;

      Pixel color = Pixel(r, g, b, w);
      break;
    }
    case 5: // pulse color
      break;
    default:
      break;
  }
}

void config(uint32_t index, uint64_t data) {
  return sessionConfig(index, data);
  switch (index) {
    case 0: {
      break;
    }
    default:
      break;
  }
}

void sessionConfig(uint32_t index, uint64_t data) {
  switch (index) {
    case 0: { // config message printing
      int boolean = data & 0x1;
      printMessages = (boolean == 1);
      break;
    }
    case 1: { // config function running
      int boolean = data & 0x1;
      runFunctions = (boolean == 1);
      break;
    }
    case 2: { // config the brightness of the neopixel
      brightness = data & 0xFF;
      break;
    }
    case 3: { // config the LED count
      LEDCount = data & 0xFFFF;
      break;
    }
    default:
      break;
  }
}

void test(uint32_t index, uint64_t data) {
  switch (index) {
    case 0: { // test communications
      Serial.println("Testing Communications...");

      uint16_t value1 = data & 0x7;
      uint16_t value2 = (data >> 3) & 0x1F;
      uint16_t value3 = (data >> 8) & 0x3FFF;
      uint16_t value4 = (data >> 22) & 0x7F;
      uint16_t value5 = (data >> 31) & 0x1FF;
      uint16_t value6 = (data >> 48) & 0x3FFF;

      Serial.println("Expected Values: 5 - 12 - 12345 - 55 - 400 - 13333");
      Serial.println("Received Values: " + String(value1) + " - " + String(value2) +
                                   " - " + String(value3) + " - " + String(value4) +
                                   " - " + String(value5) + " - " + String(value6));
      Serial.println("...Communications Are Functioning Properly.\n");
      break;
    }
    case 1: { // test LEDs
      break;
    }
    default:
      break;
  }
}

// the setup function runs once when you press reset or power the board
void setup1() {
  //pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);

  Serial.println("MCP2515 Receiver test!");

  if (!mcp.begin(CAN_BAUDRATE)) {
    Serial.println("Error initializing MCP2515.");
    while (1) delay(10);
  }
  Serial.println("MCP2515 chip found");

  neopixel.begin();
  neopixel.setBrightness(brightness);
  neopixel.clear();
  neopixel.show();

  if (ITimer.attachInterrupt(100, TimerHandler)) {
    Serial.println("set up timer");
  }
}

static int hue = 0;

void loop1() {
  uint32_t apiClass;
  if (!rp2040.fifo.pop_nb(&apiClass)) {
    return;
  }
  uint32_t index = rp2040.fifo.pop();
  uint32_t data1 = rp2040.fifo.pop();
  uint32_t data2 = rp2040.fifo.pop();

  uint64_t data = ((uint64_t)data2 << 32) | data1;

  switch (apiClass) {
    case 0:
      animate(index, data);
      break;
    case 1:
      config(index, data);
      break;
    case 2:
      sessionConfig(index, data);
      break;
    case 3:
      test(index, data);
      break;
    default:
      break;
  }
}

void rx(int available) {
  // received a packet

  int id = mcp.packetId();
  int devNum = CANID_DEVICE(id);
  int index = CANID_INDEX(id);
  int apiClass = CANID_API(id);
  int mfg = CANID_MFG(id);
  int devType = CANID_TYPE(id);

  if (devType != 10 || mfg != 8 || devNum != 0) {
    return;
  }

  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)

  if (mcp.packetRtr()) {
    // Remote transmission request, packet contains no data
    Serial.print("[R]");
  }

  rp2040.fifo.push(apiClass);
  rp2040.fifo.push(index);

  char data[8];
  int i = 0;
  while (mcp.available()) {
    int dataByte = mcp.read();

    data[i++] = dataByte;
  }

  uint32_t *splitData = (uint32_t *) &data;

  rp2040.fifo.push(splitData[0]);
  rp2040.fifo.push(splitData[1]);

  digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
}
