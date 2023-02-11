#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2, LCD_5x8DOTS);
int state = 0;
int lastState = -1;
bool lastButtonState = false;
bool buttonState = false;
bool comunicationStarted = false;

void setup()
{
    pinMode(11, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(13, OUTPUT);
    pinMode(2, INPUT);
    Serial.begin(9600);
    // initialize the LCD
    lcd.begin();

    // Turn on the blacklight and print a message.
    lcd.setBacklight(255);
    lcd.print("connecting...");

    Serial.println("k");
}

void loop()
{
    buttonState = digitalRead(2);

    if (Serial.available() > 0)
    {
        String incomingString = Serial.readString();
        if (incomingString == "start")
        {
            comunicationStarted = true;
            Serial.println("ok");
        }
        if (incomingString == "get")
        {
            Serial.println(state);
        }

        if (incomingString == "0" || incomingString == "1" || incomingString == "2")
        {
            state = incomingString.toInt();
            Serial.println("Done: " + incomingString);
            return;
        }
        Serial.println(incomingString);
    }

    if (buttonState == HIGH && buttonState != lastButtonState)
    {
        state = (state + 1) % 3;
    }
    if (state != lastState && comunicationStarted)
    {
        digitalWrite(11, LOW);
        digitalWrite(12, LOW);
        digitalWrite(13, LOW);

        lcd.clear();
        if (state == 0)
        {
            lcd.print("BLUE is showing");
            digitalWrite(11, HIGH);
        }
        else if (state == 1)
        {
            lcd.print("GREEN is showing");
            digitalWrite(12, HIGH);
        }
        else if (state == 2)
        {
            lcd.print("RED is showing");
            digitalWrite(13, HIGH);
        }
        Serial.println("changed");
        Serial.println(state);
    }

    lastButtonState = buttonState;
    lastState = state;
}