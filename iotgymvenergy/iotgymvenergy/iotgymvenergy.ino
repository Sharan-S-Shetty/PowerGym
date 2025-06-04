#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <NewPing.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// Initialize the LCD with the I2C address of 0x27, 16 characters, and 2 lines
LiquidCrystal_I2C lcd(0x27, 16, 2); //0x27
// Replace with your network credentials
// const char* ssid = "sharan";
// const char* password = "87654321";

const char* ssid = "sharan";
const char* password = "87654321";

const char* serverIP = "91.108.107.159";
WiFiClient client;

const int piezoPin = A0;
float piezoValue=0;
float matv=0;
const int cycPin = 14;
float cycv =0;
// const int wgtPin = 13;
float weight=0;
const int irsen = 13;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);          // Initialize serial communication
  Serial.println("Initializing slr...");

  // Initialize the LCD
  lcd.begin();
  lcd.backlight();
  lcd.print("Initializing LCD...");
  delay(2000);
  lcd.clear();

  WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

  Serial.println("\nWiFi connected");

  pinMode(piezoPin, INPUT);
  pinMode(cycPin, INPUT_PULLUP);
  piezoValue=0;
  cycv =0;
  // weight=0;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (analogRead(piezoPin) > 0.5 )
  {
    piezoValue = analogRead(piezoPin);    
    matv = (piezoValue * 5.0) / 1023.0;
    // Clear LCD and display real-time data
    Serial.println();
    Serial.println("Mat: ");
    Serial.println(matv);
    // Clear LCD and display real-time data
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Yoga Mat: ");
    lcd.print(matv);
    lcd.print(" mV");
    weight = matv*3.6;
    lcd.print(weight);
  }
  if (digitalRead(cycPin) == 0) { // If state has changed
            // Turn on the LED
      Serial.println("Cycle volt +1."); // Print message to serial monitor
      cycv = cycv+1;
      Serial.println("Cycle");
      Serial.println(cycv);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Cycling: ");
      lcd.print(cycv);
      lcd.print(" mV"); 

      // Clear LCD and display real-time data
    } else {                           // If the reed switch is deactivated
             // Turn off the LED
      Serial.println("Cycle volt 0."); // Print message to serial monitor      
    }
  // if (digitalRead(wgtPin) > 0) { // If state has changed
  //           // Turn on the LED
  //     Serial.println("Weight volt +1."); // Print message to serial monitor
  //     weight = weight+1;

  //     lcd.clear();
  //     lcd.setCursor(0, 0);
  //     lcd.print("Weight Lifting: ");
  //     lcd.print(weight);
  //     lcd.print(" mV");
  //   } else {                           // If the reed switch is deactivated
  //          // Turn off the LED
  //     Serial.println("Weight volt 0."); // Print message to serial monitor      
  //   }
    if (digitalRead(irsen) == 0)
  {
// Send the sensor data to the server
    if (client.connect(serverIP, 80)) {
        // Prepare the data to be sent
        String postData = "sensor1=" + String(cycv) +
                          "&sensor2=" + String(weight) +
                          "&sensor3=" + String(matv);
        Serial.print(postData);
        // Send the HTTP POST request to the server-side script
        client.println("POST https://thantrajna.in/works/IOT_GYM/save_data.php HTTP/1.1");
        client.println("Host: " + String(serverIP));
        client.println("Connection: close");
        client.println("Content-Type: application/x-www-form-urlencoded");
        client.print("Content-Length: ");
        client.println(postData.length());
        client.println();
        client.println(postData);
        Serial.println(postData);
    }
    client.stop();
  }       
  delay(1000);
}
