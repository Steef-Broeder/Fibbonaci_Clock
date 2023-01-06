
//----------LIBRARIES----------//
#include <FastLED.h>    //LEDSTRIPS
#include <TM1637.h>     //4-DIGIT DISPLAY
#include <Wire.h>       //RTC
#include <RtcDS3231.h>  //RTC
RtcDS3231<TwoWire> Rtc(Wire);

//----------SETUP----------//
//LEDSTRIPS
#define LED_PIN1     7
#define LED_PIN2     8
#define LED_PIN3     9

#define NUM_LEDS1    54
#define NUM_LEDS2    52
#define NUM_LEDS3    17

CRGB leds1[NUM_LEDS1];
CRGB leds2[NUM_LEDS2];
CRGB leds3[NUM_LEDS3];

//colors
CRGB LEDUIT = { 255, 255, 255};
CRGB LEDMINUUT = { 0, 0, 255};
CRGB LEDUUR = { 255, 100, 100};
CRGB LEDBEIDE = { 0, 255, 0};
int LedColor[4] = {LEDUIT, LEDMINUUT, LEDUUR, LEDBEIDE};

//4-DIGIT DISPLAY
int CLK = 2;
int DIO = 3;

TM1637 tm(CLK,DIO);

//CLOCK LOGIC
int MinCombi = 0;
int UurCombi = 0;

//PANELS
int PanelMinute = 0;
int Panel1 = 0;
int Panel2 = 0;
int Panel3 = 0;
int Panel5 = 0;

//------------FUNCTIONS------------//
//LEDSTRIPS
void CalcPaneColors(RtcDateTime now) {

  int remainder = now.Minute() % 5;
  MinCombi = (now.Minute() - remainder) / 5;
  UurCombi = now.Hour();

  PanelMinute = remainder;

  int Div1 = 0;
  int Div2 = 0;
  int Div3 = 0;
  int Div4 = 0;

  int DivU1 = 0;
  int DivU2 = 0;
  int DivU3 = 0;
  int DivU4 = 0;

  if(MinCombi >= 5)
  {
    Panel5 = Panel5++;
    Div1 = MinCombi - 5;
  }
  else
  {
    Div1 = MinCombi;
  }
  if(Div1 >= 3)
  {
    Panel3 = Panel3++;
    Div2 = Div1 - 3;
  }
  else
  {
    Div2 = Div1;
  }
  if(Div2 >= 2)
  {
    Panel2 = Panel2++;
    Div3 = Div2 - 2;
  }
  else
  {
    Div3 = Div2;
  }
  if(Div3 >= 1)
  {
    Panel1 = Panel1++;
    Div4 = Div3 - 1;
  }
  else
  {
    Div4 = Div3;
  }
  //----------HOUR----------
  if(now.Hour() >= 5)
  {
    Panel5 = Panel5 + 2;
    DivU1 = UurCombi - 5;
  }
  else
  {
    DivU1 = UurCombi;
  }
  if(now.Hour() >= 3)
  {
    Panel3 = Panel3 + 2;
    DivU2 = DivU1 - 3;
  }
  else
  {
    DivU2 = DivU1;
  }
  if(now.Hour() >= 2)
  {
    Panel2 = Panel2 + 2;
    DivU3 = DivU2 - 2;
  }
  else
  {
    DivU3 = DivU2;
  }
  if(now.Hour() >= 1)
  {
    Panel1 = Panel1 + 2;
    DivU4 = DivU3 - 5;
  }
  else
  {
    DivU4 = DivU3;
  }
  
  if(now.Second() >= 59)
  {
    delay(59.999 - now.Second());
    Panel1, Panel2, Panel3, Panel5 = 0, 0, 0, 0;
  }
}

void SetLedstrips() {
  //set panel5
  for(int i = 0; i<=NUM_LEDS1; i++){
    leds1[i] = LedColor[Panel5];
  }
  for(int i = 0; i<=27; i++){
    leds2[i] = LedColor[Panel5];
  }
  //set panel3
  for(int i = 27; i<=52; i++){
    leds2[i] = LedColor[Panel3];
  }
  //set panel2
  for(int i = 0; i<=9; i++){
    leds3[i] = LedColor[Panel2];
  }
  //set panel1
  for(int i = 9; i<=13; i++){
    leds3[i] = LedColor[Panel1];
  }
  //set minutepanel
  if(PanelMinute > 0) {
    int panel = 13 + PanelMinute;
    leds3[panel] = LedColor[1];
  }

}

//RTC
void StartupRtc() {
  Serial.begin(57600);

  Serial.print("compiled: ");
  Serial.print(__DATE__);
  Serial.println(__TIME__);

  Rtc.Begin();
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  printDateTime(compiled);
  Serial.println();

  if (!Rtc.IsDateTimeValid()) { DateTimeNotValidSetup(compiled); }
  if (!Rtc.GetIsRunning()) { RtcNotRunning(); }

  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled) {
    Serial.println("RTC is older than compile time!  (Updating DateTime)");
    Rtc.SetDateTime(compiled);
  } else if (now > compiled) {
    Serial.println("RTC is newer than compile time. (this is expected)");
  } else if (now == compiled) {
    Serial.println("RTC is the same as compile time! (not expected but all is fine)");
  }

  // never assume the Rtc was last configured by you, so
  // just clear them to your needed state
  Rtc.Enable32kHzPin(false);
  Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone); 
}

RtcDateTime GetRtcTime() {
  RtcDateTime now = Rtc.GetDateTime();
  printDateTime(now);
  Serial.println();
  return now;
}

//4-DIGIT Display
void display4DigitTime(RtcDateTime time){
  int hour = time.Hour();
  int minute = time.Minute();
  int displayTime = hour * 100 + minute;

  tm.display(3, displayTime % 10);   
  tm.display(2, displayTime / 10 % 10);   
  tm.display(1, displayTime / 100 % 10);   
  tm.display(0, displayTime / 1000 % 10);
}

//----------DEBUG----------//
//RTC
#define countof(a) (sizeof(a) / sizeof(a[0]))
void printDateTime(const RtcDateTime& dt) {
  char datestring[20];

  snprintf_P(datestring, countof(datestring), PSTR("%02u/%02u/%04u %02u:%02u:%02u"), dt.Month(), dt.Day(), dt.Year(), dt.Hour(), dt.Minute(), dt.Second() );
  Serial.print(datestring);
}

void DateTimeNotValidSetup(RtcDateTime compiled) {
  if (Rtc.LastError() != 0) {
      // we have a communications error
      // see https://www.arduino.cc/en/Reference/WireEndTransmission for 
      // what the number means
      Serial.print("RTC communications error = ");
      Serial.println(Rtc.LastError());
    } else {
      // Common Causes:
      //    1) first time you ran and the device wasn't running yet
      //    2) the battery on the device is low or even missing

      Serial.println("RTC lost confidence in the DateTime!");

      // following line sets the RTC to the date & time this sketch was compiled
      // it will also reset the valid flag internally unless the Rtc device is
      // having an issue

      Rtc.SetDateTime(compiled);
    }
}

void DateTimeNotValidLoop() {
  if (Rtc.LastError() != 0) {
    // we have a communications error
    Serial.print("RTC communications error = ");
    Serial.println(Rtc.LastError());
  } else {
    // Common Causes:
    //    1) the battery on the device is low or even missing and the power line was disconnected
    Serial.println("RTC lost confidence in the DateTime!");
  }
}

void RtcNotRunning() {
  Serial.println("RTC was not actively running, starting now");
  Rtc.SetIsRunning(true);
}

//----------SETUP----------//
void setup() {
  //LEDSTRIPS
  FastLED.addLeds<WS2812, LED_PIN1, GRB>(leds1, NUM_LEDS1);
  FastLED.addLeds<WS2812, LED_PIN2, GRB>(leds2, NUM_LEDS2);
  FastLED.addLeds<WS2812, LED_PIN2, GRB>(leds3, NUM_LEDS3);

  //4-DIGIT DISPLAY
  tm.init();
  tm.set(1);

  //RTC
  StartupRtc();
}

//----------LOOP----------//
void loop() {
  if (!Rtc.IsDateTimeValid()) { DateTimeNotValidLoop(); }

  RtcDateTime now = GetRtcTime(); //Get the time from the rtc
  display4DigitTime(now); //Displays the time on the 4-digit display

  CalcPaneColors(now); //Calculates what colors the different panels should have

  SetLedstrips(); //Sets the panels to correct colors

  delay(1000); // 1 sec
}





