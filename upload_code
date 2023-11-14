#include <SPI.h>
#include <Ethernet.h>

const int pulseInputPin = 2; // Use D2 (Digital Pin 2) for pulse detection
volatile unsigned long uploadPulses = 0; // Declare uploadPulses as volatile

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(10, 66, 192, 46); // Establish IP

EthernetServer server(80); // Default server for HTTP

void setup() {
  pinMode(pulseInputPin, INPUT); // Set D2 (Digital Pin 2) to input

  Serial.begin(9600); // Set the baud rate to 9600
  while (!Serial) {
    ; // Wait for Serial Monitor to open
  }

  Ethernet.begin(mac, ip);  // Establish ethernet with previous specifications

  // Attach an interrupt to detect rising edges on the pulseInputPin
  attachInterrupt(digitalPinToInterrupt(pulseInputPin), pulseRisingInterrupt, RISING);
}

void pulseRisingInterrupt() {
  uploadPulses++; // Increment the total and upload pulse counts when a rising edge is detected
}

void loop() {
  static unsigned long uploadMillis = 0;
  
  // Upload to the client over Ethernet every 5 seconds
  if (currentMillis - uploadMillis >= 500) {
    uploadMillis = currentMillis;

    float frequencykHz = (float)uploadPulses / 50.0; // Convert to kHz * 10, minus error
    uploadPulses = 0; // Reset upload pulse count

    EthernetClient client = server.available();
    if (client) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("Connection: close");
      client.println("Refresh: 0.5");

      client.println();
      client.print("Monitor Rate is ");
      client.print(frequencykHz); // Send the frequency with units (kHz)
      client.print(" kHz");
      client.stop();
    }
  }
}
