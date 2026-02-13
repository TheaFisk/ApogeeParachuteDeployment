
 Rocket Flight Computer
 Target Board Adafruit Feather M0 Adalogger
 Sensor BMP280 or BMP390 via I2C
 Features
   - Real-time altitude reading
   - Apogee detection
   - Parachute deployment trigger
   - microSD flight logging
   - LED + buzzer ready indicator


#include Wire.h
#include SPI.h
#include SD.h

#include Adafruit_BMP280.h
#include Adafruit_BMP3XX.h

 ---------------- Hardware Configuration ---------------- 

#define LED_PIN        13
#define BUZZER_PIN     12
#define DEPLOY_PIN     11
#define SD_CS_PIN      4    Feather M0 Adalogger uses pin 4

#define SEA_LEVEL_HPA  1013.25

 ---------------- Sensor Objects ---------------- 

Adafruit_BMP280 bmp280;    BMP280
Adafruit_BMP3XX bmp390;    BMP390

bool usingBMP390 = false;

 ---------------- Flight Variables ---------------- 

float previousAltitude = 0.0;
float currentAltitude = 0.0;
float maxAltitude = 0.0;

bool apogeeDetected = false;
bool flightStarted = false;

unsigned long launchTime = 0;

 ---------------- SD Logging ---------------- 

File logFile;

 ---------------- Configuration ---------------- 

 Altitude threshold to assume launch (meters)
#define LAUNCH_THRESHOLD 5.0

 Drop amount to detect apogee (meters)
#define APOGEE_DROP_THRESHOLD 1.5

 ========================================================= 

void logEvent(const char eventName, float altitude)
{
    if (logFile)
    {
        unsigned long timestamp = millis() - launchTime;

        logFile.print(timestamp);
        logFile.print(,);
        logFile.print(eventName);
        logFile.print(,);
        logFile.println(altitude);

        logFile.flush();
    }
}

 ========================================================= 

float readAltitude()
{
    if (usingBMP390)
    {
        if (!bmp390.performReading()) return currentAltitude;
        return bmp390.readAltitude(SEA_LEVEL_HPA);
    }
    else
    {
        return bmp280.readAltitude(SEA_LEVEL_HPA);
    }
}

 ========================================================= 

void deployParachute()
{
    digitalWrite(DEPLOY_PIN, HIGH);
    delay(1500);
    digitalWrite(DEPLOY_PIN, LOW);

    logEvent(PARACHUTE_DEPLOYED, currentAltitude);
}

 ========================================================= 

void readySignal()
{
    for (int i = 0; i  3; i++)
    {
        digitalWrite(LED_PIN, HIGH);
        digitalWrite(BUZZER_PIN, HIGH);
        delay(200);

        digitalWrite(LED_PIN, LOW);
        digitalWrite(BUZZER_PIN, LOW);
        delay(200);
    }
}

 ========================================================= 

void setup()
{
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(DEPLOY_PIN, OUTPUT);

    digitalWrite(DEPLOY_PIN, LOW);

    Serial.begin(115200);
    Wire.begin();

     ---------- Initialize SD Card ---------- 
    if (!SD.begin(SD_CS_PIN))
    {
        while (1)
        {
            digitalWrite(LED_PIN, HIGH);
            delay(100);
            digitalWrite(LED_PIN, LOW);
            delay(100);
        }
    }

    logFile = SD.open(flight.csv, FILE_WRITE);

    if (logFile)
    {
        logFile.println(Time(ms),Event,Altitude(m));
        logFile.flush();
    }

     ---------- Initialize Sensors ---------- 

    if (bmp390.begin_I2C())
    {
        usingBMP390 = true;
        bmp390.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
        bmp390.setPressureOversampling(BMP3_OVERSAMPLING_4X);
        bmp390.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
    }
    else if (bmp280.begin())
    {
        usingBMP390 = false;
    }
    else
    {
        while (1);  Sensor failure halt
    }

    readySignal();
}

 ========================================================= 

void loop()
{
    currentAltitude = readAltitude();

     ---------- Detect Launch ---------- 
    if (!flightStarted && currentAltitude  LAUNCH_THRESHOLD)
    {
        flightStarted = true;
        launchTime = millis();
        logEvent(LAUNCH_DETECTED, currentAltitude);
    }

    if (!flightStarted)
    {
        delay(50);
        return;
    }

     ---------- Track Maximum Altitude ---------- 
    if (currentAltitude  maxAltitude)
    {
        maxAltitude = currentAltitude;
    }

     ---------- Apogee Detection ---------- 
    if (!apogeeDetected)
    {
        if ((maxAltitude - currentAltitude)  APOGEE_DROP_THRESHOLD)
        {
            apogeeDetected = true;

            logEvent(APOGEE_DETECTED, maxAltitude);
            deployParachute();
        }
    }

     ---------- Log Altitude ---------- 
    logEvent(ALTITUDE, currentAltitude);

    previousAltitude = currentAltitude;

    delay(50);  ~20 Hz logging
}
