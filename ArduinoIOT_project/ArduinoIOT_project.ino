//Pin connected to ST_CP of 74HC595
int latchPin = 3;
//Pin connected to SH_CP of 74HC595
int clockPin = 2;
////Pin connected to DS of 74HC595
int dataPin = 4;

byte data;

int sgmt_ch[4] = {0B00000001, 0B00000010, 0B00000100, 0B00001000};//7세그먼트 자리수
int sgmt_num[10] = {0B00000011, 0B10011111, 0B00100101, 0B00001101, 0B10011001,
                   0B01001001, 0B01000001, 0B00011011, 0B00000001, 0B00001001
};

void setup() {
  //Start Serial for debuging purposes
  Serial.begin(9600);
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

}

int test[4] = {1, 0, 2, 4};
void loop() {

  for (int i = 0; i < 4; i++) {
    digitalWrite(latchPin, 0);

    shiftOut(dataPin, clockPin, LSBFIRST, sgmt_num[test[i]]); // 숫자
    shiftOut(dataPin, clockPin, MSBFIRST, sgmt_ch[i]); // 자리수

    digitalWrite(latchPin, 1);
    //delay(1);
  }

}
