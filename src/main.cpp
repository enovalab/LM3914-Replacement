#include <Arduino.h>
#include <math.h>

using Pin = int;

enum class DisplayMode
{
    Dot,
    Bar
};

Pin ledPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
Pin lowRefPin = A0;
Pin highRefPin = A1;
Pin signalPin = A2;
Pin modePin = 12;


int measureReference(Pin lowRefPin, Pin highRefPin)
{
    return analogRead(highRefPin) - analogRead(lowRefPin);
}


int measureSignal(Pin signalPin)
{
    return analogRead(signalPin);
}


void initializeDisplay(const Pin* ledPins, int numLeds = 10)
{
    for(int i = 0; i < numLeds; i++)
    {
        pinMode(ledPins[i], OUTPUT);
    }
}

void displaySignal(int signal, int reference, DisplayMode mode, const Pin* ledPins, int numLeds = 10)
{
    int ledIndex = lrint(static_cast<double>(signal) / static_cast<double>(reference));

    for(int i = 0; i < numLeds; i++)
    {
        digitalWrite(ledPins[i], HIGH);
    }

    switch(mode)
    {
        case DisplayMode::Bar:
            for(int i = 0; i < ledIndex; i++)
            {
                digitalWrite(ledPins[ledIndex], LOW);
            }
            break;
    
        case DisplayMode::Dot:
            digitalWrite(ledPins[ledIndex], LOW);
            break;
    }
}


void setup()
{
    Serial.begin(115200);
    initializeDisplay(ledPins);
}

void loop()
{
    int signal = measureSignal(signalPin);
    int reference = measureReference(lowRefPin, highRefPin);
    DisplayMode mode = static_cast<DisplayMode>(digitalRead(modePin));
    displaySignal(signal, reference, mode, ledPins);
}