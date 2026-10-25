#include <LiquidCrystal.h>
#include <stdio.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(8, 9);
LiquidCrystal lcd(6, 7, 5, 4, 3, 2);
int buzzer = 13;
int button = 10;
char pirs = 'x';
int hbtc = 0, hbtc1 = 0, rtrl = 0;
int sti = 0;
unsigned char rcv, count, gchr = 'x', gchr1 = 'x', robos = 's';
char rcvmsg[10], pastnumber[11];
// char pastnumber1[11],pastnumber2[11];//pastnumber3[11];
char gpsval[50];
// char dataread[100] = "";
// char lt[15],ln[15];
int i = 0, k = 0, lop = 0;
int gps_status = 0;
float latitude = 0;
float logitude = 0;
String Speed = "";
String gpsString = "";
char *test = "$GPRMC";
// int hbtc=0,hbtc1=0,rtrl=0;
unsigned char gv = 0, msg1[10], msg2[11];
float lati = 0, longi = 0;
unsigned int lati1 = 0, longi1 = 0;
unsigned char flat[5], flong[5];
unsigned char finallat[8], finallong[9];
int ii = 0, rchkr = 0;
float tempc = 0, weight = 0;
float vout = 0;

String inputString = "";        // a string to hold incoming data
boolean stringComplete = false; // whether the string is complete
int itk = 0;
char gsm_msg[10];
void okcheck()
{
    unsigned char rcr;
    do
    {
        rcr = Serial.read();
    } while (rcr != 'K');
}
void send_link()
{
    delay(4000);
    delay(4000);
    delay(4000);
    delay(4000);
    Serial.write("AT+CMGS=\"");
    Serial.write(pastnumber);
    Serial.write("\"\r\n");
    delay(3500);
Serial.write("smart Location
https://www.google.co.in/search?client=opera&q=");
for(ii=0;ii<=6;ii++){
        Serial.write(finallat[ii]);}
Serial.write("%2C");
for(ii=0;ii<=7;ii++){
        Serial.write(finallong[ii]);}
Serial.write(0x1A);delay(4000);delay(4000);delay(4000);delay(4000);
}
void beep()
{
    digitalWrite(buzzer, LOW);
    delay(1000);
    delay(1000);
    digitalWrite(buzzer, HIGH);
}
void setup()
{
    Serial.begin(9600); // serialEvent();
    mySerial.begin(9600);

    pinMode(buzzer, OUTPUT);
    pinMode(button, INPUT_PULLUP);
    digitalWrite(buzzer, HIGH);
    lcd.begin(16, 2);
    lcd.cursor();
    lcd.print("smart Safety Alert");
    lcd.setCursor(0, 1);
    lcd.print("Wearable GPS-GSM");
    delay(2000);
    get_gps();
    gps_convert();
    lcd.clear();
    lcd.setCursor(0, 0);
    for (ii = 0; ii <= 6; ii++)
        lcd.write(finallat[ii]);
    lcd.setCursor(0, 1);
    for (ii = 0; ii <= 7; ii++)
        lcd.write(finallong[ii]);
    delay(1500);
    gsminit();
    lcd.clear();
    itk = 0;
    // serialEvent();
}
void loop()
{
    if (digitalRead(button) == LOW)
    {
        lcd.setCursor(0, 0);
        lcd.print("smart Emergency");
        lcd.setCursor(0, 1);
        lcd.print(" Alert");
        beep();
        // mySerial.write("*Vehicle_ON_Theft#");
        gsminit1();
        delay(4000);
        delay(4000);
        delay(4000);
        delay(4000);
        Serial.write("AT+CMGS=\"");

        Serial.write(pastnumber);
        Serial.write("\"\r\n");
        delay(3500);
        Serial.write("smart Emergency Alert__");
        Serial.write("https://www.google.co.in/search?client=opera&q=");
        for (ii = 0; ii <= 6; ii++)
        {
            Serial.write(finallat[ii]);
        }
        Serial.write("%2C");
        for (ii = 0; ii <= 7; ii++)
        {
            Serial.write(finallong[ii]);
        }
        Serial.write(0x1A);
        delay(4000);
        delay(4000);
        delay(4000);
        delay(4000);
        lcd.clear();
    }
    while (Serial.available())
    {
        char inChar = (char)Serial.read();
        if (inChar == '*')
        {
            sti = 1;
            itk = 0;
            memset(gsm_msg, '\0', strlen(gsm_msg));
        }
        if (sti == 1)
        {
            gsm_msg[itk] = inChar;
            itk++;
        }
        if (inChar == '#')
        {
            // gsm_msg[itk] = inChar;
            sti = 0;
            itk = 0;
            stringComplete = true;
        }
    }
    if (stringComplete)
    {
        if (gsm_msg[1] == 't' || gsm_msg[1] == 's')
        {
            gchr = 'x';
            send_link();
        }
        memset(gsm_msg, '\0', strlen(gsm_msg));
        inputString = "";
        stringComplete = false;
    }
}

/*
void serialEvent()
{
while (Serial.available())
{
char inChar = (char)Serial.read();
if(inChar == '*')
{sti=1;
}
if(sti == 1)
{
inputString += inChar;
}
if(inChar == '#')
{sti=0;
stringComplete = true;
}
}
}
void serialEvent()
{
while (Serial.available())
{
char inChar = (char)Serial.read();
if(inChar == '*')
{
gchr = Serial.read();
}
if(inChar == '#')
{
gchr1 = Serial.read();
}
}
}*/
int readSerial(char result[])
{
    int i = 0;
    while (1)
    {
        while (Serial.available() > 0)
        {
            char inChar = Serial.read();
            if (inChar == '\n')
            {
                result[i] = '\0';

                Serial.flush();
                return 0;
            }
            if (inChar != '\r')
            {
                result[i] = inChar;
                i++;
            }
        }
    }
}
int readSerial1(char result[])
{
    int i = 0;
    while (1)
    {
        while (Serial.available() > 0)
        {
            char inChar = Serial.read();
            if (inChar == '*')
            {
                result[i] = '\0';
                Serial.flush();
                return 0;
            }
            if (inChar != '*')
            {
                result[i] = inChar;
                i++;
            }
        }
    }
}
void gpsEvent()
{
    gpsString = "";
    while (1)
    {
        // while (gps.available()>0) //Serial incoming data from GPS
        while (mySerial.available() > 0)
        {
            // char inChar = (char)gps.read();
            char inChar = (char)mySerial.read();
            gpsString += inChar; // store incoming data from GPS to temparary
            string str[] i++;
            // Serial.print(inChar);
            if (i < 7)
            {
                if (gpsString[i - 1] != test[i - 1]) // check for right string
                {
                    i = 0;
                    gpsString = "";
                }
            }
            if (inChar == '\r')
            {
                if (i > 60)
                {
                    gps_status = 1;
                    break;
                }
                else
                {
                    i = 0;
                }
            }
        }
        if (gps_status)
            break;
    }
}
void get_gps()
{
    lcd.clear();
    lcd.print("Getting GPS Data");
    lcd.setCursor(0, 1);
    lcd.print("Please Wait.....");
    gps_status = 0;
    int x = 0;
    while (gps_status == 0)
    {
        gpsEvent();
        int str_lenth = i;
        coordinate2dec();
        i = 0;
        x = 0;
        str_lenth = 0;
    }
}
void coordinate2dec()
{
    String lat_degree = "";
    for (i = 19; i <= 20; i++)
        lat_degree += gpsString[i];
    String lat_minut = "";
    for (i = 20; i <= 21; i++)
        lat_minut += gpsString[i];
    for (i = 23; i <= 24; i++)
        lat_minut += gpsString[i];
    String log_degree = "";

    for (i = 31; i <= 33; i++)
        log_degree += gpsString[i];
    String log_minut = "";
    for (i = 34; i <= 35; i++)
        log_minut += gpsString[i];
    for (i = 37; i <= 38; i++)
        log_minut += gpsString[i];
    Speed = "";
    for (i = 42; i < 45; i++) // extract longitude from string
        Speed += gpsString[i];
    float minut = lat_minut.toFloat();
    minut = minut / 60;
    float degree = lat_degree.toFloat();
    latitude = degree + minut;
    minut = log_minut.toFloat();
    minut = minut / 60;
    degree = log_degree.toFloat();
    logitude = degree + minut;
}
void coordinate2dec()
{
    String lat_degree = "";
    for (i = 19; i <= 20; i++)
        lat_degree += gpsString[i];
    String lat_minut = "";
    for (i = 21; i <= 22; i++)
        lat_minut += gpsString[i];
    for (i = 24; i <= 25; i++)
        lat_minut += gpsString[i];
    String log_degree = "";
    for (i = 32; i <= 34; i++)
        log_degree += gpsString[i];
    String log_minut = "";
    for (i = 35; i <= 36; i++)
        log_minut += gpsString[i];
    for (i = 38; i <= 39; i++)
        log_minut += gpsString[i];
    Speed = "";
    for (i = 45; i < 48; i++) // extract longitude from string
        Speed += gpsString[i];

    float minut = lat_minut.toFloat();
    minut = minut / 60;
    float degree = lat_degree.toFloat();
    latitude = degree + minut;
    minut = log_minut.toFloat();
    minut = minut / 60;
    degree = log_degree.toFloat();
    logitude = degree + minut;
}
void gps_convert()
{
    if (gps_status)
    {
        // Serial.println(gpsString);
        if (gpsString[0] == '$' && gpsString[1] == 'G' && gpsString[2] == 'P' && gpsString[3] == 'R' && gpsString[4] == 'M' &&
            gpsString[5] == 'C')
        {
            //
            Serial.println("Don11111111111111111111111111111111111111111111111111111\r\n");
            //
            Serial.write(gpsString[18]);
            Serial.write(gpsString[19]);
            Serial.write(gpsString[20]);
            Seri
                al.write(gpsString[21]);
            Serial.write(gpsString[22]);
            // lcd.setCursor(0,0);
            for (ii = 0; ii < 9; ii++)
            {
                // lcd.write(gpsString[19+ii]);
                msg1[ii] = gpsString[19 + ii];
                // Serial.write(msg1[ii]);
            }
            // Serial.println("\r\n");
            // lcd.setCursor(0,1);
            for (ii = 0; ii < 10; ii++)
            {
                // lcd.write(gpsString[32+ii]);
                msg2[ii] = gpsString[32 + ii];
                // Serial.write(msg2[ii]);
            }
            // Serial.println(msg1);
            // Serial.println(msg2);
            // lati = (((msg1[2]-48)*100000) +((msg1[3]-48)*10000) + ((msg1[5]-48)*1000) +
((msg1[6]-48)*100) + ((msg1[7]-48)*10) + (msg1[8]-48));
// longi = (((msg2[3]-48)*100000) + ((msg2[4]-48)*10000) + ((msg2[6]-48)*1000)
+ ((msg2[7]-48)*100) + ((msg2[8]-48)*10) + (msg2[9]-48));

lati = (((msg1[2] - 48) * 1000) + ((msg1[3] - 48) * 100) + ((msg1[5] - 48) * 10) + (msg1[6] - 48));
longi = (((msg2[3] - 48) * 1000) + ((msg2[4] - 48) * 100) + ((msg2[6] - 48) * 10) + (msg2[7] - 48));
// converts(lati);Serial.write("-");
// converts(longi);Serial.write("\r\n");
lati = (lati / 60);
longi = (longi / 60);
lati = (lati * 100);
longi = (longi * 100);
lati1 = lati;
longi1 = longi;
// Serial.write("After ");
// converts(lati1);Serial.write("-");
// converts(longi1);Serial.write("\r\n");
convlat(lati);
convlong(longi);
finallat[0] = msg1[0];
finallat[1] = msg1[1];
finallat[2] = '.';
finallat[3] = flat[0];
finallat[4] = flat[1];
finallat[5] = flat[2];
finallat[6] =
    flat[3];
finallat[7] = '\0';
finallong[0] = msg2[0];
finallong[1] = msg2[1];
finallong[2] = msg2[2];
finallong[3] = '.';
finallong[4] = flong[0];
finallong[5] = flong[1];
finallong[6] = flong[2];
finallong[7] =
    flong[3];
finallong[8] = '\0';
        }
    }
}
void convlat(unsigned int value)
{

    unsigned int a, b, c, d, e, f, g, h;
    a = value / 10000;
    b = value % 10000;
    c = b / 1000;
    d = b % 1000;
    e = d / 100;
    f = d % 100;
    g = f / 10;
    h = f % 10;
    a = a | 0x30;
    c = c | 0x30;
    e = e | 0x30;
    g = g | 0x30;
    h = h | 0x30;
    // dlcd(a);
    // dlcd(c);dlcd(e); dlcd(g);dlcd(h);//lcddata('A');//lcddata(' ');lcddata(' ');
    flat[0] = c;
    flat[1] = e;
    flat[2] = g;
    flat[3] = h;
}
void convlong(unsigned int value)
{
    unsigned int a, b, c, d, e, f, g, h;
    a = value / 10000;
    b = value % 10000;
    c = b / 1000;
    d = b % 1000;
    e = d / 100;
    f = d % 100;
    g = f / 10;
    h = f % 10;
    a = a | 0x30;
    c = c | 0x30;
    e = e | 0x30;
    g = g | 0x30;
    h = h | 0x30;
    // dlcd(a);
    // dlcd(c);dlcd(e); dlcd(g);dlcd(h);//lcddata('A');//lcddata(' ');lcddata(' ');
    flong[0] = c;

    flong[1] = e;
    flong[2] = g;
    flong[3] = h;
}
void coordinate2dec()
{
    String lat_degree = "";
    for (i = 20; i <= 21; i++)
        lat_degree += gpsString[i];
    String lat_minut = "";
    for (i = 22; i <= 28; i++)
        lat_minut += gpsString[i];
    String log_degree = "";
    for (i = 32; i <= 34; i++)
        log_degree += gpsString[i];
    String log_minut = "";
    for (i = 35; i <= 41; i++)
        log_minut += gpsString[i];
    Speed = "";
    for (i = 45; i < 48; i++) // extract longitude from string
        Speed += gpsString[i];
    float minut = lat_minut.toFloat();
    minut = minut / 60;
    float degree = lat_degree.toFloat();
    latitude = degree + minut;
    minut = log_minut.toFloat();
    minut = minut / 60;
    degree = log_degree.toFloat();
    logitude = degree + minut;
}
void gsminit1()
{
    Serial.write("AT\r\n");
    delay(7000); // okcheck();
    Serial.write("ATE0\r\n");
    delay(7000); // okcheck();
    Serial.write("AT+CMGF=1\r\n");
    delay(7000); // okcheck();
    Serial.write("AT+CNMI=1,2,0,0\r\n");
    delay(7000); // okcheck();
    Serial.write("AT+CSMP=17,167,0,0\r\n");
    delay(7000); // okcheck();
}
void gsminit()
{
    Serial.write("AT\r\n");
    okcheck();
    Serial.write("ATE0\r\n");
    okcheck();
    Serial.write("AT+CMGF=1\r\n");
    okcheck();
    Serial.write("AT+CNMI=1,2,0,0\r\n");
    okcheck();
    Serial.write("AT+CSMP=17,167,0,0\r\n");
    okcheck();
    lcd.clear();
    lcd.print("SEND MSG STORE");
    lcd.setCursor(0, 1);
    lcd.print("MOBILE NUMBER");
    do
    {
        rcv = Serial.read();
    } while (rcv != '*');
    readSerial(pastnumber);
    pastnumber[10] = '\0';
    pastnumber1[0] = pastnumber[0];
    pastnumber1[1] = pastnumber[1];
    pastnumber1[2] = pastnumber[2];
    pastnumber1[3] = pastnumber[3];
    pastnumber1[4] =
        pastnumber[4];
    pastnumber1[5] = pastnumber[5];
    pastnumber1[6] = pastnumber[6];
    pastnumber1[7] = pastnumber[7];
    pastnumber1[8] = pastnumber[8];
    pastnumber1[9] = pastnumber[9];
    pastnumber1[10] = '\0';
    pastnumber3[0] = pastnumber[20];
    pastnumber3[1] =
        pastnumber[21];
    pastnumber3[2] = pastnumber[22];
    pastnumber3[3] =
        pastnumber[23];
    pastnumber3[4] = pastnumber[24];
    pastnumber3[5] =
        pastnumber[25];
    pastnumber3[6] = pastnumber[26];
    pastnumber3[7] =

        pastnumber[27];
    pastnumber3[8] = pastnumber[28];
    pastnumber3[9] =
        pastnumber[29];
    pastnumber3[10] = '\0';
    lcd.clear();
    lcd.print(pastnumber);
    Serial.write("AT+CMGS=\"");
    Serial.write(pastnumber);
    Serial.write("\"\r\n");
    delay(2500);
    Serial.write("Reg\r\n");
    Serial.write(0x1A);
    // pastnumber[10]='\0';
    delay(4000);
    delay(4000);
}
int gpsgain(char result[])
{
    int i = 0;
    char rcvv;
    while (1)
    {
        while (Serial.available() > 0)
        {
        lp:
            char inChar = Serial.read();
            result[i] = inChar;
            if (result[0] == '$')
            {
                i++;
                // result[i] = inChar;
            }
            if (result[0] != '$')
            {
                i = 0;
            }
            if (i == 5)
            {
                if (result[0] == '$' && result[1] == 'G' && result[2] == 'P' && result[3] == 'R' && result[4] == 'M' && result[5] == 'C')
                {
                    goto lp;
                }
                else
                {
                    i = 0;
                }
            }
            if (i == 46)
            {
                result[47] = '\0';
                Serial.flush();

                lt[0] = result[21];
                lt[1] = result[22];
                lt[2] = result[23];
                lt[3] = result[24];
                lt[4] = result[25];
                lt[5] = result
                    [26];
                lt[6] = result[27];
                lt[7] = result[28];
                lt[8] = result[29];
                lt[9] = result[30];
                lt[10] = result[31];
                lt[11] = '\0';
                ln[0] = result[33];
                ln[1] = result[34];
                ln[2] = result[35];
                ln[3] = result[36];
                ln[4] = result[37];
                ln[5] = r
                    esult[38];
                ln[6] = result[39];
                ln[7] = result[40];
                ln[8] = result[41];
                ln[9] = result[42];
                ln[10] = result[43];
                ln[11] = result[44];
                ln[12] = '\0';
                return 0;
            }
        }
    }
}
void keypad()
{
    char kn = 0, valk = 0;
    lcd.setCursor(0, 1);
    while (1)
    {
        if (digitalRead(swi) == LOW)
        {
            delay(1000);
            while (digitalRead(swi) == LOW)
                ;
            valk++;

            if (valk >= 9)
            {
                valk = 9;
            }
            lcd.setCursor(kn, 1);
            convertk(valk);
        }
        if (digitalRead(swd) == LOW)
        {
            delay(1000);
            while (digitalRead(swd) == LOW)
                ;
            valk--;
            if (valk <= 0)
            {
                valk = 0;
            }
            lcd.setCursor(kn, 1);
            convertk(valk);
        }
        if (digitalRead(swe) == LOW)
        {
            delay(1000);
            while (digitalRead(swe) == LOW)
                ;
            password[kn] = (valk + 48);
            kn++;
            lcd.setCursor(kn, 1);
            valk = 0;
            if (kn == 4)
            {
                kn = 0;
                break;
            }
        }
    }
}
void converts(unsigned int value)
{
    unsigned int a, b, c, d, e, f, g, h;
    a = value / 10000;
    b = value % 10000;
    c = b / 1000;
    d = b % 1000;
    e = d / 100;
    f = d % 100;
    g = f / 10;
    h = f % 10;

    a = a | 0x30;
    c = c | 0x30;
    e = e | 0x30;
    g = g | 0x30;
    h = h | 0x30;
    Serial.write(a);
    Serial.write(c);
    Serial.write(e);
    Serial.write(g);
    Serial.write(h);
}
void convertl(unsigned int value)
{
    unsigned int a, b, c, d, e, f, g, h;
    a = value / 10000;
    b = value % 10000;
    c = b / 1000;
    d = b % 1000;
    e = d / 100;
    f = d % 100;
    g = f / 10;
    h = f % 10;
    a = a | 0x30;
    c = c | 0x30;
    e = e | 0x30;
    g = g | 0x30;
    h = h | 0x30;
    // lcd.write(a);
    // lcd.write(c);
    lcd.write(e);
    lcd.write(g);
    lcd.write(h);
}
void convertk(unsigned int value)
{
    unsigned int a, b, c, d, e, f, g, h;
    a = value / 10000;
    b = value % 10000;
    c = b / 1000;
    d = b % 1000;
    e = d / 100;
    f = d % 100;

    g = f / 10;
    h = f % 10;
    a = a | 0x30;
    c = c | 0x30;
    e = e | 0x30;
    g = g | 0x30;
    h = h | 0x30;
    // lcd.write(a);
    // lcd.write(c);
    // lcd.write(e);
    // lcd.write(g);
    lcd.write(h);
}
sensorValue = analogRead(analogInPin);
sensorValue = (sensorValue / 9.31);
lcd.setCursor(1, 1); // rc
lcd.print(sensorValue);
Serial.print(sensorValue);