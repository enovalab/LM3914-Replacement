#include <Arduino.h>
#include <math.h>

using Pin = uint8_t;

enum class DisplayMode : uint8_t
{
    Dot,
    Bar
};

constexpr Pin ledPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
constexpr Pin lowRefPin = A0;
constexpr Pin highRefPin = A1;
constexpr Pin signalPin = A2;
constexpr Pin modePin = 12;



uint16_t measureReference(Pin lowRefPin, Pin highRefPin)
{
    return analogRead(highRefPin) - analogRead(lowRefPin);
}


uint16_t measureSignal(Pin signalPin)
{
    return analogRead(signalPin);
}


void initializeDisplay(const Pin* ledPins, uint8_t numLeds = 10)
{
    for(uint8_t i = 0; i < numLeds; i++)
    {
        pinMode(ledPins[i], OUTPUT);
    }
}

void displaySignal(uint16_t signal, uint16_t reference, DisplayMode mode, const Pin* ledPins, uint8_t numLeds = 10)
{
    uint16_t ledIndex = lrint(static_cast<double>(signal) / static_cast<double>(reference));

    for(uint8_t i = 0; i < numLeds; i++)
    {
        digitalWrite(ledPins[i], HIGH);
    }

    switch(mode)
    {
        case DisplayMode::Bar:
            for(uint8_t i = 0; i < ledIndex; i++)
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
    uint16_t signal = measureSignal(signalPin);
    uint16_t reference = measureReference(lowRefPin, highRefPin);
    DisplayMode mode = static_cast<DisplayMode>(digitalRead(modePin));
    displaySignal(signal, reference, mode, ledPins);
}