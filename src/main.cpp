#include <Arduino.h>
#include <math.h>

using Pin = int;

int numLeds = 10;
Pin ledPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
Pin lowRefPin = A0;
Pin highRefPin = A1;
Pin signalPin = A2;
Pin modePin = 12;


int measureReference(Pin lowRefPin, Pin highRefPin)
{
    // Differenz aus Spannugen am R_HIGH und R_LOW Pin bilden
    return analogRead(highRefPin) - analogRead(lowRefPin);
}


int measureSignal(Pin signalPin)
{
    return analogRead(signalPin);
}


bool readMode(Pin modePin)
{
    return digitalRead(modePin);
}


int calculateLedIndex(int signal, int reference, int numLeds)
{
    // Quotient berechnen und runden
    int ledIndex = lrint(static_cast<float>(signal) * numLeds / reference);
    
    // 1 abziehen, weil das Array bei 0 beginnt
    ledIndex--;

    // ledIndex darf nicht größer oder gleich der Anzahl der LEDs werden
    if (ledIndex >= numLeds)
        ledIndex = numLeds - 1;

    return ledIndex;
}


void displaySignal(int ledIndex, bool isbarMode, Pin ledPins[], int numLeds)
{
    // alle LEDs ausschalten
    for(int i = 0; i < numLeds; i++)
    {
        pinMode(ledPins[i], INPUT);
    }

    // Wenn keine LED leuchten soll Funktion beenden
    if(ledIndex < 0)
        return;

    if(isbarMode)
    {
        // Bar Mode
        for(int i = 0; i < ledIndex + 1; i++)
        {
            pinMode(ledPins[i], OUTPUT);
            digitalWrite(ledPins[i], LOW);
        }
    }
    else
    {
        // Dot Mode
        pinMode(ledPins[ledIndex], OUTPUT);
        digitalWrite(ledPins[ledIndex], LOW);
    }
}


void setup()
{}


void loop()
{
    int signal = measureSignal(signalPin);
    int reference = measureReference(lowRefPin, highRefPin);
    bool isBarMode = readMode(modePin);
    int ledIndex = calculateLedIndex(signal, reference, numLeds);
    displaySignal(ledIndex, isBarMode, ledPins, numLeds);
}