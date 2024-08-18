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
Stack stack(10);
Chase red(stack.getSize(), Pixel(128));
Chase green(stack.getSize(), PIXEL_GREEN);
Solid blue(stack.getSize(), Pixel(0,0,128));

bool TimerHandler(struct repeating_timer *t) {
  /*
  red.setStart((red.getStart() + 1) % red.getSize());
  red.setEnd((red.getEnd() + 1) % red.getSize());
  blue.setStart((blue.getStart() + 1) % blue.getSize());
  blue.setEnd((blue.getEnd() + 1) % blue.getSize());
  */
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
  Serial.println();
  return true;
}

uint32_t getColor(int colorId) {
  switch (colorId) {
    case 0: // Red
      return neopixel.Color(255, 0, 0);
    case 1: // Green
      return neopixel.Color(0, 255, 0);
    case 2: // Blue
      return neopixel.Color(0, 0, 255);
    case 3: // Yellow
      return neopixel.Color(255, 255, 0);
    case 4: // Cyan
      return neopixel.Color(0, 255, 255);
    case 5: // Magenta
      return neopixel.Color(255, 0, 255);
    case 6: // White
      return neopixel.Color(255, 255, 255);
    case 7: // Off
      return neopixel.Color(0, 0, 0);
    default:
      Serial.println("Warning: Invalid colorId given. Valid colors are between 0 and 7");
      return neopixel.Color(0, 0, 0);
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
  //neopixel.setBrightness(20);
  neopixel.clear();
  neopixel.show();

  red.setSpeed(5, 100);
  green.setSpeed(10, 100);

  blue.setStart(5);
  blue.setWidth(1);

  stack.Push(&red);
  stack.Push(&green);
  stack.Push(&blue);

  if (ITimer.attachInterrupt(100, TimerHandler)) {
    Serial.println("set up timer");
  }
}

static int hue = 0;

void loop1() {
  uint32_t funcId;
  if (!rp2040.fifo.pop_nb(&funcId)) {
    return;
  }
  uint32_t index = rp2040.fifo.pop();
  uint32_t argList1 = rp2040.fifo.pop();
  uint32_t argList2 = rp2040.fifo.pop();

  int colorId = argList1 & 0x7;
  uint32_t color;
  if (colorId 0= 7) {
    uint8_t r = (argList1 >> 8) & 0xFF;
    uint8_t g = (argList1 >> 16) & 0xFF;
    uint8_t b = (argList1 >> 24) & 0xFF;
    color = neopixel.Color(r, g, b);
  } else {
    color = getColor(colorId);
  }

  switch (funcId) {
    case 0: // set color
      neopixel.fill(color, 0, LED_COUNT);
      break;
    case 1: // turn off LEDs
      neopixel.clear(); // probably eventually use case 0 and use getColor(7) for the color
      break;
    case 2: // rotate or bounce color
      break;
    case 3: // rotate a rainbow color
      break;
    case 4: // flash color
      break;
    case 5: // pulse color
      break;
    case 10: { // test communications
      Serial.println("Testing Communications...");

      int value1 = argList1 & 0x7;
      int value2 = (argList1 >> 3) & 0x1F;
      int value3 = (argList1 >> 8) & 0x3FFF;
      int value4 = (argList1 >> 22) & 0x7F;
      int value5 = ((argList1 >> 31) & 0x1) | ((argList2 & 0xFF) << 1);
      int value6 = (argList2 >> 16) & 0x3FFF;

      Serial.println("Expected Values: 5 - 12 - 12345 - 55 - 400 - 13333");
      Serial.println("Received Values: " + String(value1) + " - " + String(value2) +
                                   " - " + String(value3) + " - " + String(value4) +
                                   " - " + String(value5) + " - " + String(value6));
      Serial.println("...Communications Are Functioning Properly.\n");
      break;
    }
    case 11: { // config message printing

    }
    case 12: { // config function running

    }
    default:
      neopixel.clear();
      break;
  }
  neopixel.show();
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

  // uint32_t apiId = apiClass << 4 | index;
  // rp2040.fifo.push(apiId);

  rp2040.fifo.push(apiClass);
  rp2040.fifo.push(index);

  char data[8];
  int i = 0;
  while (mcp.available()) {
    int dataByte = mcp.read();

    data[i++] = dataByte;
  }

  uint32_t *splitData = (uint32_t *) & data;

  rp2040.fifo.push(splitData[0]);
  rp2040.fifo.push(splitData[1]);

  digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
}
