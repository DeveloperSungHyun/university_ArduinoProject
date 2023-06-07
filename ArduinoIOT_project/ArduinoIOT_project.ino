#include <virtuabotixRTC.h>
#include <string.h>

virtuabotixRTC myRTC(5, 6, 7);

#define latchPin 3
#define clockPin 2
#define dataPin 4

#define Button_alarmsetting 10 //알람 설정버튼
#define Button_setting 11 //시간 설정버튼
#define Button_Up 12 //설정버튼
#define Button_Down 13 //설정버튼

#define lm35Pin A0

#define SP 10

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
long distance_UpDataTIme;

long buttton_CheckTime;

long alarm_delay;

bool TimeToTemp_change = false;
int change_delay = 5000;
long change_time = 0;

bool seconds = true;

int alarm_h = 15, alarm_m = 30;



void setup() {

  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  pinMode(13, OUTPUT);

  pinMode(Button_alarmsetting, INPUT);
  pinMode(Button_setting, INPUT);
  pinMode(Button_Up, INPUT);
  pinMode(Button_Down, INPUT);

  myRTC.setDS1302Time(00, 23, 10, 6, 6, 7, 2023);

  Serial.begin(9600);

  distance_UpDataTIme = time_UpDataTime = temp_UpDataTime = millis();
  change_time = millis();
  alarm_delay = millis();

}

float test = 0;
bool button_check_1 = false;
bool button_check_2 = false;

void loop() {

  if (myRTC.hours == alarm_h && myRTC.minutes == alarm_m) {
    tone(SP, 800);
  }

if(digitalRead(Button_Up) == true){
  noTone(SP);
}
  //Button_alarmsetting
  if (digitalRead(Button_setting) == false) {
    button_check_1 = true;
  }

  if (digitalRead(Button_alarmsetting) == false) {
    button_check_2 = true;
  }

  if (digitalRead(Button_setting) == true && button_check_1 == true) {
    if (millis() - buttton_CheckTime >= 1000) {
      tone(SP, 800);
      delay(80);
      noTone(SP);
      button_check_1 = false;
      SettingMode();
    }
  } else {
    buttton_CheckTime = millis();
  }




  if (millis() - change_time >= change_delay) {
    change_time = millis();
    TimeToTemp_change = !TimeToTemp_change;
  }


  if (TimeToTemp_change) {//=================시간

    if (millis() - time_UpDataTime >= 500) {
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

bool check_sw = false;
void SettingMode() {
  myRTC.updateTime();//시갑 업데이트
  int time_h = myRTC.hours, time_m = myRTC.minutes;
  long delay_time = millis();

  while (true) {
    TimeDisplay(time_h, time_m, true);

    if (digitalRead(Button_setting) == false) {
      check_sw = true;
    }
    if (digitalRead(Button_setting) == true && check_sw == true) {
      check_sw = false;
      myRTC.setDS1302Time(00, time_m, time_h, 6, 6, 7, 2023);
      tone(SP, 800);
      delay(80);
      noTone(SP);
      break;

    }

    if (digitalRead(Button_Down) == true) {
      if (millis() - delay_time >= 200) {
        delay_time = millis();
        time_m++;
        if (time_m >= 60)time_m = 0;

        tone(SP, 500);
        delay(50);
        noTone(SP);
      }

    }

    if (digitalRead(Button_Up) == true) {
      digitalWrite(13, true);
      if (millis() - delay_time >= 200) {
        delay_time = millis();
        time_h++;
        if (time_h >= 24)time_h = 0;

        tone(SP, 500);
        delay(50);
        noTone(SP);
      }

    }
  }
}


void alarm_SettingMode() {
  long delay_time = millis();
  while (true) {
    TimeDisplay(alarm_h, alarm_m, true);

    if (digitalRead(Button_Down) == true) {
      if (millis() - delay_time >= 200) {
        delay_time = millis();
        alarm_h++;
        if (alarm_h >= 60)alarm_h = 0;

        tone(SP, 500);
        delay(50);
        noTone(SP);
      }

    }

    if (digitalRead(Button_Up) == true) {
      digitalWrite(13, true);
      if (millis() - delay_time >= 200) {
        delay_time = millis();
        alarm_m++;
        if (alarm_m >= 24)alarm_m = 0;

        tone(SP, 500);
        delay(50);
        noTone(SP);
      }

    }
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

  if (seconds) seconds_dot = 0B00010000;
  else seconds_dot = 0;

  if (tamp) tamp_dot = 0B00100000;
  else tamp_dot = 0;

  digitalWrite(latchPin, false);

  shiftOut(dataPin, clockPin, LSBFIRST, sgmt_num[test[ch_num]] - dot_value); // 숫자
  shiftOut(dataPin, clockPin, MSBFIRST, sgmt_ch[ch_num] + seconds_dot + tamp_dot); // 자리수

  digitalWrite(latchPin, true);
}
