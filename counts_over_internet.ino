#include <SPI.h>
#include <Ethernet.h>

const int pulseInputPin = 2; // Use D2 (Digital Pin 2) for pulse detection
volatile unsigned long totalPulses = 0; // Declare totalPulses as volatile
volatile unsigned long uploadPulses = 0; // Declare uploadPulses as volatile

// Require a unique counter for the upload values

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(10, 66, 192, 46); // Establish IP

EthernetServer server(80); // Default server for HTTP

void setup() {
  pinMode(pulseInputPin, INPUT); // Set D2 (Digital Pin 2) to input

  Serial.begin(9600); // Set the baud rate to 9600
  while (!Serial) {
    ; // Wait for Serial Monitor to open
  }
  Serial.println("Serial Monitor is ready.");

  Ethernet.begin(mac, ip);  // Establish ethernet with previous specifications

  Serial.println("Ethernet initialized.");
  Serial.print("Server is at ");
  Serial.println(Ethernet.localIP());

  // Attach an interrupt to detect rising edges on the pulseInputPin
  attachInterrupt(digitalPinToInterrupt(pulseInputPin), pulseRisingInterrupt, RISING);
}

void pulseRisingInterrupt() {
  totalPulses++;
  uploadPulses++; // Increment the total and upload pulse counts when a rising edge is detected
}

void loop() {
  static unsigned long previousMillis = 0;
  static unsigned long uploadMillis = 0;

  // Measure and display the pulses per second every second
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;

    float frequencykHz = (float)totalPulses
    // Print to the Serial Monitor with units
    Serial.print("Frequency (kHz): ");
    Serial.println(frequencykHz);

    // totalPulses = 0; // Reset pulse count for the next measurement interval
  }

  // Upload to the client over Ethernet every 5 seconds
  if (currentMillis - uploadMillis >= 5000) {
    uploadMillis = currentMillis;

    float frequencykHz = (float)uploadPulses;

    // uploadPulses = 0; // Reset upload pulse count

    EthernetClient client = server.available();
    if (client) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("Connection: close");
      client.println("Refresh: 5"); // Refresh the page every 5 seconds
      client.println();
      client.print(frequencykHz); // Send the frequency with units (kHz)
      client.stop();
    }
  }
}
