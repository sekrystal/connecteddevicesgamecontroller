//Adapted from Tom Igoe's BallDropMKR1000JoystickClient.ino file accessible with this link:
//https://github.com/tigoe/BallDropGame/blob/master/BallDropMKR1000JoystickClient/BallDropMKR1000JoystickClient.ino

#include<SPI.h>
#include<WiFiNINA.h>
#include "arduino_secrets.h" 

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int keyIndex = 0;

WiFiClient client;
int status = WL_IDLE_STATUS;
IPAddress server(10,23,11,167);

// connecting/disconnecting pushbutton
const int connectButton = 6;
int buttonStateConnect = HIGH;
const int connectionLED = 7;
int LEDStateConnect = HIGH;

// 4 controller buttons
const int buttonPinUp = 2;
int buttonStateUp = HIGH; 
const int buttonPinDown = 3;
int buttonStateDown = HIGH;  
const int buttonPinLeft = 4;
int buttonStateLeft = HIGH;  
const int buttonPinRight = 5;
int buttonStateRight = HIGH;  

const int sendInterval = 100;
const int debounceInterval = 15;
int prevButtonState = 0;
long lastTimeSent = 0;

void setup() {
  Serial.begin(9600);

  pinMode(connectionLED, OUTPUT);
  pinMode(connectButton, INPUT_PULLUP);

  while (status !=WL_CONNECTED) {
    Serial.print("Attempting to Connect to SSID:");
    Serial.println(ssid);

    status = WiFi.begin(ssid, pass);

    delay(3000);
  }

  printWiFiStatus();
  
  pinMode(buttonPinUp, INPUT_PULLUP);
  pinMode(buttonPinDown, INPUT_PULLUP);
  pinMode(buttonPinLeft, INPUT_PULLUP);
  pinMode(buttonPinRight, INPUT_PULLUP);
   
}

void loop() {
  char message = 0;
  long now = millis();
  boolean buttonPushed = buttonRead(connectButton);

  if (buttonPushed) {
      if (client.connected()) {
        Serial.println("disconnecting");
        client.print("x");
        client.stop();
      }
    else {
      Serial.println("connecting");
      client.connect(server, 8080);
    }
  }
  
  buttonStateUp = digitalRead(buttonPinUp);
  buttonStateDown = digitalRead(buttonPinDown);
  buttonStateLeft = digitalRead(buttonPinLeft);
  buttonStateRight = digitalRead(buttonPinRight);

  switch (buttonStateUp) {
   case LOW:
   message = 'u';
   break;
  }
  switch (buttonStateDown) {
   case LOW:
   message = 'd';
   break;
  }
  switch (buttonStateLeft) {
   case LOW:
   message = 'l';
   break;
  }
  switch (buttonStateRight) {
   case LOW:
   message = 'r';
   break;
  }

//LED CONNECTION STATE
  if (client.connected()
    && now - lastTimeSent > sendInterval
    && message !=0) {
      client.print(message);
      Serial.print(message);
      lastTimeSent = now;
    }
  
    digitalWrite(connectionLED, client.connected());
    if (client.available()){
      char c = client.read();
      Serial.write(c);
    }
}

    boolean buttonRead(int thisButton) {
      boolean result = false;
      int currentState = digitalRead (thisButton);

      if (currentState != prevButtonState
        && currentState == LOW) {
          result = true;
        }

     delay(debounceInterval);
     prevButtonState = currentState;
     return result;
    }

 
 void printWiFiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  long rssi = WiFi.RSSI ();
  Serial.print (rssi);
  Serial.println(" dBm");
 }
