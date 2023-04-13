#include <Arduino.h>
#include <math.h>

using Pin = uint8_t;

enum class DisplayMode : uint8_t
{
    Dot,
    Bar
};

constexpr uint8_t numLeds = 10;
constexpr Pin ledPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
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

void displaySignal(uint16_t signal, uint16_t reference, DisplayMode mode, const Pin* ledPins, uint8_t numLeds)
{
    float percentage = signal * 100.0 / reference;

    Serial.print("percentage: ");
    Serial.print(percentage);
    Serial.print("   ");

    int16_t ledIndex = lrint(percentage * 0.1) - 1;

    Serial.print("ledIndex: ");
    Serial.print(ledIndex);
    Serial.print("   ");

    for(uint8_t i = 0; i < numLeds; i++)
    {
        pinMode(ledPins[i], INPUT);
    }

    switch(mode)
    {
        case DisplayMode::Bar:
            for(uint8_t i = 0; i < ledIndex + 1; i++)
            {
                pinMode(ledPins[i], OUTPUT);
                digitalWrite(ledPins[i], LOW);
            }
            break;
        case DisplayMode::Dot:
            pinMode(ledPins[ledIndex], OUTPUT);
            digitalWrite(ledPins[ledIndex], LOW);
            break;

    }
}


void setup()
{
    Serial.begin(115200);
    initializeDisplay(ledPins, numLeds);
}

void loop()
{
    uint16_t signal = measureSignal(signalPin);
    uint16_t reference = measureReference(lowRefPin, highRefPin);

    Serial.print("signal: ");
    Serial.print(signal);
    Serial.print("   ");

    Serial.print("reference: ");
    Serial.print(reference);
    Serial.print("   ");

    DisplayMode mode = static_cast<DisplayMode>(digitalRead(modePin));

    Serial.print("mode: ");
    Serial.print(static_cast<int>(mode));
    Serial.print("   ");

    displaySignal(signal, reference, mode, ledPins, numLeds);

    Serial.println();
}