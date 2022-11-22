

struct Led
{
    int r = 0;
    int g = 0;
    int b = 0;
};

Led LEDUIT = { 255, 255, 255};
Led LEDMINUUT = { 0, 0, 255};
Led LEDUUR = { 255, 100, 100};
Led LEDBEIDE = { 0, 255, 0};
int LedColor[4] = {LEDUIT, LEDMINUUT, LEDUUR, LEDBEIDE};

int Led1 = 0;
int Led2 = 0;
int Led3 = 0;
int Led5 = 0;

RTC_DS3231 rtc;
int MinCombi = 0;
char daysOfTheWeek[7][9] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup()
{
    Serial.begin(9600);

    //halt if the rtc didn't start correctly, waits 5 sec and tries again
    while(true)
    {
        if (!rtc.begin())
        {
            Serial.println("RTC niet verbonden/correct aangesloten.");
            sleep(5000);
        }
        else
        {
            break;
        }
    }

    // Load the current time into the RTC after a powerloss
    if (rtc.lostPower())
    {
        Serial.println("Tijd relativeren");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}

void loop()
{
    // Fetch the current time from the RTC
    DateTime now = rtc.now();

    //----------MINUTE----------
    remainder = now.minute() % 5
    MinCombi = (now.minute() - remainder) / 5

    if(MinCombi >= 5)
    {
        int Led5 = Led5++;
        int Div1 = MinCombi - 5;
    }
    else
    {
        int Div1 = MinCombi;
    }
    if(Div1 >= 3)
    {
        int Led3 = Led3++;
        int Div2 = Div1 - 3;
    }
    else
    {
        int Div2 = Div1;
    }
    if(Div2 >= 2)
    {
        int Led2 = Led2++;
        int Div3 = Div2 - 2;
    }
    else
    {
        int Div3 = Div2;
    }
    if(Div3 >= 1)
    {
        int Led1 = Led1++;
        int Div4 = Div3 - 1;
    }
    else
    {
        int Div4 = Div3;
    }

    //----------HOUR----------
    if now.hour = > 5
    {
        int Led5 = Led5 + 2;
        int DivU1 = UurCombi - 5;
    }
    else
    {
        int DivU1 = UurCombi;
    }
    if now.hour = > 3
    {
        int Led3 = Led3 + 2;
        int DivU2 = DivU1 - 3;
    }
    else
    {
        int DivU2 = DivU1;
    }
    if now.hour = > 2
    {
        int Led2 = Led2 + 2;
        int DivU3 = DivU2 - 2;
    }
    else
    {
        int DivU3 = DivU2;
    }
    if now.hour = > 1
    {
        int Led1 = Led1 + 2;
        int DivU4 = DivU3 - 5;
    }
    else
    {
        int DivU4 = DivU3;
    }

    digitalWrite(KleurLed1, k[Led1])
    digitalWrite(KleurLed2, k[Led2])
    digitalWrite(KleurLed3, k[Led3])
    digitalWrite(KleurLed5, k[Led5])

    If(now.second >= 59)
    {
        delay(59.999 - now.second);
        Led1, Led2, Led3, Led5 = 0, 0, 0, 0;
    }
}
