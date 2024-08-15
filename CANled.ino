#include <Adafruit_MCP2515.h>
#include <Adafruit_NeoPixel.h>


#define CS_PIN PIN_CAN_CS
#define CAN_BAUDRATE (1000000)

#define CANID_TYPE(id) ((id >> 24) & 0x1f)
#define CANID_TYPE_ROBOT 1

#define CANID_MFG(id) ((id >> 16) & 0xff)
#define CANID_MFG_NI 1

#define CANID_API(id) ((id >> 10) & 0x3f)
#define CANID_INDEX(id) ((id >> 6) & 0xf)
#define CANID_DEVICE(id) (id & 0x3f)

Adafruit_MCP2515 mcp(CS_PIN);
Adafruit_NeoPixel neopixel(100, 5, NEO_RGBW + NEO_KHZ800);

void setup() {
  mcp.onReceive(PIN_CAN_INTERRUPT, rx);
}

void loop() {
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
  neopixel.clear();
  neopixel.show();
  neopixel.setBrightness(20);
}

// the loop function runs over and over again forever
static int hue = 0;

void flash() {

}

void loop1() {
  uint32_t apiClass;
  if (!rp2040.fifo.pop_nb(&apiClass)) {
    return;
  }
  uint32_t index = rp2040.fifo.pop();
  uint32_t data1 = rp2040.fifo.pop();
  uint32_t data2 = rp2040.fifo.pop();

  Serial.print(apiClass, HEX);
  Serial.print('|');
  Serial.print(index, HEX);
  Serial.print('|');
  Serial.print(data1, HEX);
  Serial.print('|');
  Serial.print(data2, HEX);
  Serial.println();
}

void rx(int available) {
  // received a packet

  int id = mcp.packetId();
  int devNum = CANID_DEVICE(id);
  int index = CANID_INDEX(id);
  int apiClass = CANID_API(id);
  int mfg = CANID_MFG(id);
  int devType = CANID_TYPE(id);

  if (devType != 10 || mfg != 8) {
    return;
  }

  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)

  if (mcp.packetRtr()) {
    // Remote transmission request, packet contains no data
    Serial.print("[R]");
  }

  // Serial.print(devType, DEC);
  // Serial.print(":");
  // Serial.print(mfg, DEC);
  // Serial.print("/");
  // Serial.print(apiClass, DEC);
  // Serial.print(":");
  // Serial.print(index, DEC);
  // Serial.print("@");
  // Serial.print(devNum, DEC);

  // Serial.print(" -> ");

  // uint32_t apiId = apiClass << 4 | index;
  // rp2040.fifo.push(apiId);

  rp2040.fifo.push(apiClass);
  rp2040.fifo.push(index);

  char data[8];
  int i = 0;
  while (mcp.available()) {
    int dataByte = mcp.read();

    // Serial.print((char)dataByte, HEX);

    data[i++] = dataByte;
  }

  uint32_t *splitData = (uint32_t *)&data;

  rp2040.fifo.push(splitData[0]);
  rp2040.fifo.push(splitData[1]);

  // Serial.println();

  digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
}
