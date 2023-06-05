int latchPin = 3;
int clockPin = 2;
int dataPin = 4;

int sgmt_ch[4] = {0B00000001, 0B00000010, 0B00000100, 0B00001000};//7세그먼트 자리수
int sgmt_num[11] = {0B00000011, 0B10011111, 0B00100101, 0B00001101, 0B10011001,
                    0B01001001, 0B01000001, 0B00011011, 0B00000001, 0B00001001,
                    0B01100011
                   };

int ch_num = 0;

void setup() {

  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);


}

void loop() {

  //TimeDisplay(22, 30);
  TemperatureDisplay(39.4);
}
void TimeDisplay(int h, int m) {
  segment_display(h / 10, h % 10, m / 10, m % 10, -1);
}
void TemperatureDisplay(float num) {
  segment_display((int)num / 10, (int)num % 10, (int)(num * 10) % 10, 10, 1);
}

void segment_display(int num1, int num2, int num3, int num4, int dot) {
  int test[4] = {num1, num2, num3, num4};
  int dot_value = 0;

  ch_num++;
  if (ch_num >= 4) ch_num = 0;

  if (dot == ch_num) dot_value = 1;
  else dot_value = 0;


  digitalWrite(latchPin, false);

  shiftOut(dataPin, clockPin, LSBFIRST, sgmt_num[test[ch_num]] - dot_value); // 숫자
  shiftOut(dataPin, clockPin, MSBFIRST, sgmt_ch[ch_num]); // 자리수

  digitalWrite(latchPin, true);
}
