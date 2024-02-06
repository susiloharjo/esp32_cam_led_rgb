#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <FastLED.h>

const char* ssid = "WIFIAP";
const char* password = "WIFIPASS";

#define NUM_LEDS 16        // Number of leds
#define DATA_PIN 12        // Data pin

CRGB leds[NUM_LEDS];      // Led init

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 25200); // Get Jakarta Time


void setup() {
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);  // config led 
  Serial.begin(115200);

   // Menghubungkan ke jaringan WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("Wifi Connected");
  
  // NTP init
  timeClient.begin();
  timeClient.setTimeOffset(25200); // NTP GMT +7 (Jakarta Time)

  
}

void loop() {

  // get time from NTP
  timeClient.update();

  delay(1000);

  // Membaca waktu saat ini
  int currentHour = timeClient.getHours();

  Serial.println(currentHour);
  Serial.println("lampu hidup");

if (currentHour >= 21 || currentHour < 5) {
    // Turn off led between 9pm to 5am
    FastLED.clear();
    FastLED.show();
  } else {
    

  
  // Randomly generate RGB values for the fade color
  uint8_t r = random(256);
  uint8_t g = random(256);
  uint8_t b = random(256);
  
  // Fade to the random color for 30 seconds
  fadeToColor(r, g, b);
  
  // Wait for 5 minutes before returning to the original mode
  delay(300000); // 5 minutes (5 * 60 * 1000 milliseconds)
  
  // Return to the original mode (display color based on predefined values)
  // You can adjust these values as needed
  int brightness = map(550, 400, 700, 0, 255); // Example lux value of 550
  leds[0] = CRGB(brightness, brightness, brightness);
  FastLED.show();
}

  }

// Function to fade to a specific color
void fadeToColor(uint8_t targetR, uint8_t targetG, uint8_t targetB) {
  static uint8_t currentR = 0;
  static uint8_t currentG = 0;
  static uint8_t currentB = 0;
  
  // Calculate step size for each color component
  int stepR = (targetR - currentR) / 30; // 30 seconds (30 * 2 steps)
  int stepG = (targetG - currentG) / 30;
  int stepB = (targetB - currentB) / 30;
  
  // Fade to the target color over 30 seconds
  for (int i = 0; i < 30; i++) { // 30 seconds (30 * 2 steps)
    currentR += stepR;
    currentG += stepG;
    currentB += stepB;
    
    // Set all LEDs to the current color
    for (int j = 0; j < NUM_LEDS; j++) {
      leds[j] = CRGB(currentR, currentG, currentB);
    }
    FastLED.show();
    
    delay(500); // Adjust delay as needed for desired fade effect
  }
}
