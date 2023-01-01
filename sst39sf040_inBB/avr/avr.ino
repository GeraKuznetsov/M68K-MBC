//PORT A = 22,23,24,25,26,27,28,29 (add l)
//PORT B = 53,52,51,50,10,11,12,13 (add h)
//PORT C =  37,36,35,34,33,32,31,30 (add hh)
//PORT F =  A0 .. ,A7 (data L)
//PORT K =  A9 .. A15 (data H)
#define AS 40
#define RD_WR 41
#define LDS 42
#define UDS 43
#define RES 46

bool debug = 1;

void setup() {
  PORTK = DDRK = PORTF = DDRF = 0; //data is input and low
  PORTK = DDRK = PORTF = DDRF = 0;
  PORTA = DDRA = PORTB = DDRB = PORTC = DDRC = 0;
  pinMode(AS, INPUT);
  pinMode(RD_WR, INPUT);
  pinMode(LDS, INPUT);
  pinMode(UDS, INPUT);
  digitalWrite(AS, LOW);
  digitalWrite(RD_WR, LOW);
  digitalWrite(LDS, LOW);
  digitalWrite(UDS, LOW);
  
  pinMode(RES, INPUT);
  digitalWrite(RES, LOW);

  Serial.begin(500000);
  Serial.println("flash");
}

const int CMD_NONE = 0;
const int CMD_IDENT = 1;
const int CMD_ES = 2;
const int CMD_WRITE = 3;
const int CMD_READ = 4;
const int CMD_CLEAR = 5;
const int CMD_DB = 6;
const int CMD_SUM = 7;

int cmd;

void readMode() {  
  PORTK = DDRK = PORTF = DDRF = 0;
  digitalWrite(RD_WR, HIGH);
}

void writeMode() {  
  digitalWrite(RD_WR, LOW);
  PORTF = PORTK = 0;
  DDRF = DDRK = 0xFF;
}

void memWrite(unsigned long add, byte dataL, byte dataH) {
  add >>= 1UL;
  PORTF = dataH;
  PORTK = dataL;
  PORTA = add & 0xFFUL;
  PORTB = (add & 0xFF00UL) >> 8;
  PORTC = (add & 0xFF0000UL) >> 16;

  digitalWrite(AS, LOW);
  digitalWrite(LDS, LOW);
  digitalWrite(UDS, LOW);
  digitalWrite(LDS, HIGH);
  digitalWrite(UDS, HIGH);
  digitalWrite(AS, HIGH);
  //digitalWrite(CE, HIGH);
}

word memRead(uint32_t add) {
  add >>= uint32_t(1);
  PORTA = add & 0xFF;
  PORTB = (add & 0xFF00) >> 8;
  PORTC = (add & 0xFF0000) >> 16;
  
  digitalWrite(AS, LOW);
  digitalWrite(LDS, LOW);
  digitalWrite(UDS, LOW);
  word out = word(PINF) | (word(PINK) << 8);
  digitalWrite(LDS, HIGH);
  digitalWrite(UDS, HIGH);
  digitalWrite(AS, HIGH);
  return out;
}

void readChar(char *c) {
  *c = -1;
  while (*c == -1)
    *c = Serial.read();
}

long parseHex(char *c) {
  long i = 0;
  while (true) {
    readChar(c);
    if (*c >= '0' && *c <= '9') {
      i *= 16; i += *c - '0';
    } else if (*c >= 'a' && *c <= 'f') {
      i *= 16; i += *c - 'a' + 10;
    } else if (*c >= 'A' && *c <= 'F') {
      i *= 16; i += *c - 'A' + 10;
    } else {
      return i;
    }
  }
}

void parseHexBuff(char *c, int length, byte* buff) {
  for (int n = 0; n < length * 2; n++) {
    int i;
    readChar(c);
    if (*c >= '0' && *c <= '9') {
      i = *c - '0';
    } else if (*c >= 'a' && *c <= 'f') {
      i = *c - 'a' + 10;
    } else if (*c >= 'A' && *c <= 'F') {
      i = *c - 'A' + 10;
    } else {
      return;
    }
    if (n % 2 == 0) {
      buff[n / 2] = i * 16;
    } else {
      buff[n / 2] |= i;
    }
  }
}

byte data[256];
long addressA, addressB;
#define TESTC(x) if(c != x){Serial.println("Parse error"); cmd = CMD_NONE; goto endParse;}

const unsigned long u5555 = 0x00005555UL << 1;
const unsigned long u2AAA = 0x00002AAAUL << 1;

void loop() {
  // put your main code here, to run repeatedly:
  cmd = CMD_NONE;
  if (debug) Serial.print("> ");

  char c;
  readChar(&c);
  if (c == 'i') {
    cmd = CMD_IDENT;
    readChar(&c);
    TESTC('\n');
  } else if (c == 's') {
    cmd = CMD_SUM;
    readChar(&c);
    TESTC('\n');
  } else if (c == 'e') {
    cmd = CMD_ES;
    readChar(&c);
    addressA = parseHex(&c);
    TESTC('\n');
  } else if (c == 'c') {
    cmd = CMD_CLEAR;
    readChar(&c);
    TESTC('\n');
  } else if (c == 'd') {
    cmd = CMD_DB;
    readChar(&c);
    TESTC(' ');
    debug = parseHex(&c);
    TESTC('\n');
  } else if (c == 'r') {
    cmd = CMD_READ;
    readChar(&c);
    TESTC(' ');
    addressA = parseHex(&c);
    TESTC(' ');
    addressB = parseHex(&c);
    TESTC('\n');
  } else if (c == 'w') {
    cmd = CMD_WRITE;
    readChar(&c);
    TESTC(' ');
    addressA = parseHex(&c);
    TESTC(' ');
    addressB = parseHex(&c);
    TESTC(' ');
    parseHexBuff(&c, addressB, data);
    readChar(&c);
    TESTC('\n');
  } else {
    Serial.println("Parse error");
    cmd = CMD_NONE;
  }

endParse:

  pinMode(RES, OUTPUT);
  digitalWrite(RES, LOW); 
  delay(50);
  
    digitalWrite(AS, HIGH);
 digitalWrite(RD_WR, HIGH);
  digitalWrite(LDS, HIGH); 
  digitalWrite(UDS, HIGH);
   pinMode(AS, OUTPUT);
  pinMode(RD_WR, OUTPUT);
  pinMode(LDS, OUTPUT);
  pinMode(UDS, OUTPUT);


  DDRA = DDRB = DDRC = 0xFF; // address output
  PORTA = PORTB = PORTC = 0; //address low
  
  if (cmd == 0) {
    Serial.println("CMD_NONE");
  } else if (cmd == CMD_SUM) {
    readMode();
    unsigned int sum = 0;
    if (debug) Serial.println("Check sum");
    for (long a = 0; a < (long) 524288; a++) {
      sum = (sum + memRead(a)) % 0xFF;
    }
    Serial.print(sum);
  } else if (cmd == CMD_DB) {
    Serial.print("DB: ");
    Serial.println(debug ? '1' : '0');
  } else if (cmd == CMD_ES) {
    if (debug) Serial.print("Clear sector ");
    if (debug)  Serial.println(addressA, HEX);
    writeMode();
    memWrite(u5555, 0xAA, 0xAA);
    memWrite(u2AAA, 0x55, 0x55);
    memWrite(u5555, 0x80, 0x80);
    memWrite(u5555, 0xAA, 0xAA);
    memWrite(u2AAA, 0x55, 0x55);
    memWrite(addressA, 0x30, 0x30);
    readMode();
    delay(100);
    Serial.println("DONE");
  } else if (cmd == CMD_CLEAR) {
    if (debug)  Serial.println("Erase IC");
    writeMode();
    memWrite(u5555, 0xAA, 0xAA);
    memWrite(u2AAA, 0x55, 0x55);
    memWrite(u5555, 0x80, 0x80);
    memWrite(u5555, 0xAA, 0xAA);
    memWrite(u2AAA, 0x55, 0x55);
    memWrite(u5555, 0x10, 0x10);
    readMode();
    delay(100);
    Serial.println("DONE");
  } else if (cmd == CMD_IDENT) {
    if (debug)   Serial.println("Identify IC");
    writeMode();
    memWrite(u5555, 0xAA, 0xAA);
    memWrite(u2AAA, 0x55, 0x55);
    memWrite(u5555, 0x90, 0x90);
    readMode();
    Serial.println("Man ID: ");
    word w = memRead(uint32_t(0x00000000UL));
    Serial.print((w >> 12) & 0x0F, HEX);
    Serial.print((w >> 8) & 0x0F, HEX);
    Serial.print((w >> 4) & 0x0F, HEX);
    Serial.println(w & 0x0F, HEX);
    Serial.println("Dev ID: ");
    w = memRead(uint32_t(0x00000002UL));
    Serial.print((w >> 12) & 0x0F, HEX);
    Serial.print((w >> 8) & 0x0F, HEX);
    Serial.print((w >> 4) & 0x0F, HEX);
    Serial.println(w & 0x0F, HEX);
    writeMode();
    memWrite(u5555, 0xAA, 0xAA);
    memWrite(u2AAA, 0x55, 0x55);
    memWrite(u5555, 0xF0, 0xF0);
    readMode();
  } else if (cmd == CMD_WRITE) {
    if (debug) Serial.println("Write Data");
    if (debug) Serial.print("Location: ");
    if (debug) Serial.println(addressA, HEX);
    if (debug) Serial.print("Length: ");
    if (debug) Serial.println(addressB, HEX);

    for (uint32_t add = 0; add < addressB; add += 2) {
      byte valL = data[add];
      byte valH = data[add + 1];
      //if (valL != 0xFF || valH != 0xFF) {
      writeMode();
      memWrite(u5555, 0xAA, 0xAA);
      memWrite(u2AAA, 0x55, 0x55);
      memWrite(u5555, 0xA0, 0xA0);
      memWrite(add + addressA, valL, valH);
      readMode();
      delayMicroseconds(25);
      //}
    }
    Serial.println("DONE");
  } else if (cmd == CMD_READ) {
    if (debug)  Serial.println("Read Data");
    if (debug)  Serial.print("Location: ");
    if (debug)  Serial.println(addressA, HEX); 
    if (debug)  Serial.print("Length: ");
    if (debug)   Serial.println(addressB, HEX);
    readMode();
    for (long add = 0; add < addressB; add += 2) {
      //byte val = memRead(add + addressA);
      word w = memRead(add + addressA);
      if (add % 64 == 0) Serial.println();
      Serial.print((w >> 12) & 0x0F, HEX);
      Serial.print((w >> 8) & 0x0F, HEX);
      Serial.print((w >> 4) & 0x0F, HEX);
      Serial.print(w & 0x0F, HEX);
    }
    Serial.println();
    readMode();
  }

  PORTK = DDRK = PORTF = DDRF = 0;
  PORTA = DDRA = PORTB = DDRB = PORTC = DDRC = 0;
  pinMode(AS, INPUT);
  pinMode(RD_WR, INPUT);
  pinMode(LDS, INPUT);
  pinMode(UDS, INPUT);
  digitalWrite(AS, LOW);
  digitalWrite(RD_WR, LOW);
  digitalWrite(LDS, LOW);
  digitalWrite(UDS, LOW);
  
  pinMode(RES, INPUT);
  delayMicroseconds(250);

 }
