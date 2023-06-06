#include <virtuabotixRTC.h>

virtuabotixRTC myRTC(5, 6, 7);

int latchPin = 3;
int clockPin = 2;
int dataPin = 4;

int lm35Pin = A0;

int sgmt_ch[4] = {0B00000001, 0B00000010, 0B00000100, 0B00001000};//7세그먼트 자리수
int sgmt_num[11] = {0B00000011, 0B10011111, 0B00100101, 0B00001101, 0B10011001,
                    0B01001001, 0B01000001, 0B00011011, 0B00000001, 0B00001001,
                    0B01100011
                   };

int ch_num = 0;

int reading;
float temp_value;
long average_count;
float temperatureC;

long temp_UpDataTime;
long time_UpDataTime;


bool TimeToTemp_change = false;
int change_delay = 5000;
long change_time = 0;

bool seconds = true;

void setup() {

  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  myRTC.setDS1302Time(00, 00, 10, 6, 6, 7, 2023);

  time_UpDataTime = temp_UpDataTime = millis();
  change_time = millis();
}

float test = 0;
void loop() {

  if (millis() - change_time >= change_delay) {
    change_time = millis();
    TimeToTemp_change = !TimeToTemp_change;
  }

  
  if (TimeToTemp_change) {//=================시간
    
    if(millis() - time_UpDataTime >= 500){
      time_UpDataTime = millis();
      myRTC.updateTime();//시갑 업데이트
      seconds = !seconds;
    }
   
    TimeDisplay(myRTC.hours, myRTC.minutes, seconds);//시간 출력

  } else {//=================================온도

    reading = analogRead(lm35Pin);

    if (millis() - temp_UpDataTime <= 500) {

      temp_value = reading * 5.0 / 1024.0;
      temperatureC += temp_value * 100;
      average_count++;
    } else {
      temp_UpDataTime = millis();
      test = temperatureC / average_count;
      average_count = 0;
      temperatureC = 0;
    }

    TemperatureDisplay(test);//온도 출력

  }

}
void TimeDisplay(int h, int m, bool seconds) {
  segment_display(h / 10, h % 10, m / 10, m % 10, -1, seconds, false);
}
void TemperatureDisplay(float num) {
  segment_display((int)num / 10, (int)num % 10, (int)(num * 10) % 10, 10, 1, false, true);
}

void segment_display(int num1, int num2, int num3, int num4, int dot, bool seconds, bool tamp) {
  int test[4] = {num1, num2, num3, num4};
  int dot_value = 0;
  int seconds_dot = 0;
  int tamp_dot = 0;
  ch_num++;
  if (ch_num >= 4) ch_num = 0;

  if (dot == ch_num) dot_value = 1;
  else dot_value = 0;

  if(seconds) seconds_dot = 0B00010000;
  else seconds_dot = 0;

  if(tamp) tamp_dot = 0B00100000;
  else tamp_dot = 0;

  digitalWrite(latchPin, false);

  shiftOut(dataPin, clockPin, LSBFIRST, sgmt_num[test[ch_num]] - dot_value); // 숫자
  shiftOut(dataPin, clockPin, MSBFIRST, sgmt_ch[ch_num] + seconds_dot + tamp_dot); // 자리수

  digitalWrite(latchPin, true);
}
