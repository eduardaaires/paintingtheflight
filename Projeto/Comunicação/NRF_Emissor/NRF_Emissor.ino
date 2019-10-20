
/*
  Getting Started example sketch for nRF24L01+ radios
  This is a very basic example of how to send data from one node to another
  Updated: Dec 2014 by TMRh20
*/

#include <SPI.h>
#include "RF24.h"

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 0;
int var;
int pino = 4;
int change;
int ledState = 0;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7, 8);
/**********************************************************/

byte addresses[][6] = {"1Node", "2Node"};

// Used to control whether this node is sending or receiving
bool role = 1; //Escolhe o tipo: 0 -> Receptor     1 -> Emissor

void setup() {
  Serial.begin( mmç);
  Serial.println(F("RF24/examples/GettingStarted"));
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));

  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);

  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);

  // Start the radio listening for data
  radio.startListening();


  pinMode(pino, OUTPUT);
  digitalWrite(pino, LOW);
}

void loop() {


  /****************** Ping Out Role ***************************/
  if (role == 1)  {

    radio.stopListening();                                    // First, stop listening so we can talk.

    if (Serial.available())
    {
      while (Serial.available())
      {
        
        var = Serial.read();
        radio.write(var, sizeof(var));
        Serial.print(var);
      }

    }
    radio.startListening();
  }



  /****************** Pong Back Role ***************************/

  if ( role == 0 )
  {
    change = 1;
    Serial.println(radio.available());
    while(radio.available()) {
      if(ledState == 0)
        {
          ledState = 1;
          digitalWrite(pino, HIGH);
        }
        else
        {
          ledState = 0;
          digitalWrite(pino, LOW);
        }
         change=0;
      // Variable for the received timestamp                                 // While there is data ready
      radio.read(&var, sizeof(var));
      Serial.println(var);// Get the payload
    }
    delay(1000);
  }
} // Loop
