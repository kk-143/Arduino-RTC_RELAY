#include <ThreeWire.h> 
#include <RtcDS1302.h>

int Relay1 = 12;
int Relay2 = 11; 
int Relay3 = 10;
int Relay4 = 9;
int Relay5 = 8;
int Relay6 = 7;
int Relay7 = 6;
int Relay8 = 5;
ThreeWire myWire(3,2,4); // DAT, CLK, RST
RtcDS1302<ThreeWire> Rtc(myWire); 
#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime& dt)
{
    char datestring[20];

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Month(),
            dt.Day(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    Serial.print(datestring);
    Serial.println();
}

void RelayTest(int a){
  digitalWrite(a, LOW);
  delay(1000);
  digitalWrite(a, HIGH);
  delay(1000);
  
}

void setup() 
{ Serial.begin(57600);

    Serial.print("compiled: ");
    Serial.print(__DATE__);
    Serial.println(__TIME__);

    Rtc.Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    printDateTime(compiled);
    Serial.println();

    if (!Rtc.IsDateTimeValid()) 
    {
        // Common Causes:
        //    1) first time you ran and the device wasn't running yet
        //    2) the battery on the device is low or even missing

        Serial.println("RTC lost confidence in the DateTime!");
        Rtc.SetDateTime(compiled);
    }

    if (Rtc.GetIsWriteProtected())
    {
        Serial.println("RTC was write protected, enabling writing now");
        Rtc.SetIsWriteProtected(false);
    }

    if (!Rtc.GetIsRunning())
    {
        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }

    RtcDateTime now = Rtc.GetDateTime();
    if (now < compiled) 
    {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        Rtc.SetDateTime(compiled);
    }
    else if (now > compiled) 
    {
        Serial.println("RTC is newer than compile time. (this is expected)");
    }
    else if (now == compiled) 
    {
        Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    }


    //Relay setup
    for (int thisPin = Relay1; thisPin >= Relay8; thisPin--){
        pinMode(thisPin, OUTPUT);    
   }

    for (int thisPin = Relay1; thisPin >= Relay8; thisPin--){
         RelayTest(thisPin);
    }
 
} 
void StartEndRelay(int Start, int End, int x, int Relay){
  if (End >= 24){
    End = End - 24;
  }
  
  if (Start < End){
    if(x >= Start && x < End){
        digitalWrite(Relay,LOW);}
      else{
        digitalWrite(Relay,HIGH); }}
  if(Start > End){
    if(x >= End && x < Start){
      digitalWrite(Relay,HIGH);
    }
    else{
      digitalWrite(Relay,LOW);
    }
    }
    }
void loop() {
        RtcDateTime now = Rtc.GetDateTime();
        printDateTime(now);
        Serial.print(now.Hour());
        int Start_time = 8;
        int i = 4;
        StartEndRelay(Start_time,Start_time+i,now.Hour(),Relay1);
        StartEndRelay(Start_time,Start_time+(2*i),now.Hour(),Relay2);
        StartEndRelay(Start_time,Start_time+(3*i),now.Hour(),Relay3);
        StartEndRelay(Start_time,Start_time+(4*i),now.Hour(),Relay4);
        StartEndRelay(Start_time,Start_time+(5*i),now.Hour(),Relay5);
        //StartEndRelay(Start_time,21,now.Hour(),Relay6);
       // StartEndRelay(Start_time,0,now.Hour(),Relay7);
       // StartEndRelay(Start_time,59,now.Hour(),Relay8);
       // for (int thisPin = Relay1; thisPin >= Relay8; thisPin--){
   
         delay(120000); 
}
