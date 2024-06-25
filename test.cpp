#include "OneButton.h"
#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

Servo myservo;  // create servo object to control a servo

int nutnhan = 4;
bool isOpen = false; // Track the state of the servo
bool rfidUnlock = false;
bool espData = false;
unsigned long rfidUnlockTime = 0;
unsigned long espUnLockTime = 0;

#define SS_PIN 10
#define RST_PIN 5

OneButton button(nutnhan, true);

MFRC522 rfid(SS_PIN, RST_PIN);
byte keyTagUID[4] = {0xE2, 0x09, 0xD6, 0x1A};

void setup()
{
    Serial.begin(9600);

    pinMode(3, INPUT);  // data from esp32_cam

    myservo.attach(2);  // attaches the servo on pin 2 to the servo object
    myservo.write(0);   // Initial position
    button.attachClick(handleClick);

    SPI.begin(); // init SPI bus
    rfid.PCD_Init(); // init MFRC522

    Serial.println("Tap RFID/NFC Tag on reader");
}

void loop()
{
    // switch code
    button.tick(); // Check button state
    delay(10);     // Small delay to debounce

    // Check RFID tag
    if (rfid.PICC_IsNewCardPresent())
    {
        if (rfid.PICC_ReadCardSerial())
        {
            if (rfid.uid.uidByte[0] == keyTagUID[0] &&
                rfid.uid.uidByte[1] == keyTagUID[1] &&
                rfid.uid.uidByte[2] == keyTagUID[2] &&
                rfid.uid.uidByte[3] == keyTagUID[3])
            {
                Serial.println("Access is granted");
                rfidUnlock = true;
                rfidUnlockTime = millis();
                myservo.write(180); // unlock the door
            }
            else
            {
                Serial.print("Access denied for user with UID:");
                for (int i = 0; i < rfid.uid.size; i++)
                {
                    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
                    Serial.print(rfid.uid.uidByte[i], HEX);
                }
                Serial.println();
            }

            rfid.PICC_HaltA();       // halt PICC
            rfid.PCD_StopCrypto1();  // stop encryption on PCD
        }
    }

    // Check if we need to lock the door after 3 seconds
    

    // for esp32_cam
    int esp=digitalRead(3);
    if (esp==1)
    {
      Serial.println("Unlock door");
      espData = true;
      espUnLockTime = millis();
      myservo.write(180); // unlock the door
    }

    if ((rfidUnlock && (millis() - rfidUnlockTime > 3000)) || (espData && (millis() - espUnLockTime > 3000)))
    {
      myservo.write(0); // lock the door
      rfidUnlock = false;
      espData = false ;
    }

}


//switch code
void handleClick()
{
    if (isOpen)
    {
        myservo.write(0);   // Close the servo
        isOpen = false;     // Update the state
    }
    else
    {
        myservo.write(180); // Open the servo
        isOpen = true;      // Update the state
    }
}
