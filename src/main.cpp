#include <Arduino.h>
#include <math.h>

using Pin = uint8_t;

constexpr uint8_t numLeds = 10;
constexpr Pin ledPins[numLeds] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
constexpr Pin lowRefPin = A0;
constexpr Pin highRefPin = A1;
constexpr Pin signalPin = A2;
constexpr Pin modePin = 12;


uint16_t measureReference(Pin lowRefPin, Pin highRefPin)
{
    uint16_t reference = analogRead(highRefPin) - analogRead(lowRefPin);
    if(reference < 0)
        reference = 0;
    return reference;
}


uint16_t measureSignal(Pin signalPin)
{
    return analogRead(signalPin);
}


void initializeDisplay(const Pin* ledPins, uint8_t numLeds)
{
    for(uint8_t i = 0; i < numLeds; i++)
    {
        pinMode(ledPins[i], OUTPUT);
    }
}


bool readMode(Pin modePin)
{
    return digitalRead(modePin);
}


int16_t calculateLedIndex(uint16_t signal, uint16_t reference, uint8_t numLeds)
{
    return lrint(static_cast<float>(signal) * numLeds / reference ) - 1;
}


void displaySignal(int16_t ledIndex, bool barMode, const Pin* ledPins, uint8_t numLeds)
{
    for(uint8_t i = 0; i < numLeds; i++)
    {
        pinMode(ledPins[i], INPUT);
    }

    if(ledIndex < 0)
        return;

    if(barMode)
    {
        for(uint8_t i = 0; i < ledIndex + 1; i++)
        {
            pinMode(ledPins[i], OUTPUT);
            digitalWrite(ledPins[i], LOW);
        }
    }
    else
    {
        pinMode(ledPins[ledIndex], OUTPUT);
        digitalWrite(ledPins[ledIndex], LOW);
    }
}


void setup()
{}


void loop()
{
    displaySignal(
        calculateLedIndex(
            measureSignal(signalPin), 
            measureReference(lowRefPin, highRefPin),
            numLeds
        ),
        readMode(modePin),
        ledPins,
        numLeds
    );
}