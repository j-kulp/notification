#include <WiFi.h>
#include <HTTPClient.h>
#include <UrlEncode.h> // URL encoding library
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// Network and Telegram details
const char* ssid = "YOUR_WIFI_HERE";
const char* password = "WIFI_PW";
const char* botToken = "TELEGRAM_BOT_TOKEN";

// Chat IDs of recipients
String chatIDs[] = {"CHAT_ID_1"};
const int numChatIDs = sizeof(chatIDs) / sizeof(chatIDs[0]);

// Pin definitions
#define SDA 13
#define SCL 14
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int dayButtonPin = 4;
const int eveningButtonPin = 5;
const int nightButtonPin = 18;
const int floorswitchPin = 26;
const int bonusswitchPin = 27;

// Button states and timing
const unsigned long debounceDuration = 100;  // Debounce duration to prevent false triggers
unsigned long lastButtonPressTime = 0;       // Stores the time of the last button press
const unsigned long longPressDuration = 1000; // 1 second to register a long press
const unsigned long lcdTimeout = 5000;       // Timeout for the LCD backlight (5 seconds)

int lastButtonPin = -1;  // Tracks the last button that was pressed

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize I2C and LCD
  Wire.begin(SDA, SCL);
  if (!i2CAddrTest(0x27)) {
    lcd = LiquidCrystal_I2C(0x3F, 16, 2); // Fallback address if 0x27 is not detected
  }
  lcd.init();
  
  // Initialize button and switch pins
  pinMode(dayButtonPin, INPUT_PULLUP);
  pinMode(eveningButtonPin, INPUT_PULLUP);
  pinMode(nightButtonPin, INPUT_PULLUP);
  pinMode(floorswitchPin, INPUT);
  pinMode(bonusswitchPin, INPUT);
}

void loop() {
  // Check switch states to determine location and bonus status
  int floorState = digitalRead(floorswitchPin);
  String location = (floorState == LOW) ? "F1" : "F2";

  int bonusState = digitalRead(bonusswitchPin);
  String bonus = (bonusState == LOW) ? "No Bonus" : "Bonus";

  // Check button states to determine which shift button was pressed
  int dayButtonState = digitalRead(dayButtonPin);
  int eveningButtonState = digitalRead(eveningButtonPin);
  int nightButtonState = digitalRead(nightButtonPin);

  // Process button press for each shift
  if (dayButtonState == LOW && lastButtonPin != dayButtonPin) {
    lastButtonPin = dayButtonPin;
    lastButtonPressTime = millis(); // Record the time the button was pressed
    showDetails("6AM-2PM", location, bonus); // Display details on LCD
  } else if (eveningButtonState == LOW && lastButtonPin != eveningButtonPin) {
    lastButtonPin = eveningButtonPin;
    lastButtonPressTime = millis(); // Record the time the button was pressed
    showDetails("2PM-10PM", location, bonus); // Display details on LCD
  } else if (nightButtonState == LOW && lastButtonPin != nightButtonPin) {
    lastButtonPin = nightButtonPin;
    lastButtonPressTime = millis(); // Record the time the button was pressed
    showDetails("10PM-6AM", location, bonus); // Display details on LCD
  }

  // Handle long press detection
  if (lastButtonPin != -1) {
    handleLongPress(lastButtonPin, lastButtonPressTime, location, bonus);
  }

  // Manage LCD timeout and backlight
  if (millis() - lastButtonPressTime > lcdTimeout) {
    lcd.clear();
    lcd.noBacklight();
    lastButtonPin = -1; // Reset last button pin to avoid multiple triggers
  }

  delay(100); // Small delay to reduce button bounce issues
}

bool i2CAddrTest(uint8_t addr) {
  Wire.beginTransmission(addr);
  return (Wire.endTransmission() == 0);
}

void showDetails(String shift, String location, String bonus) {
  lcd.backlight(); // Keep the backlight on while displaying
  lcd.setCursor(0, 0); // Move the cursor to column 0, row 1
  lcd.print(shift);
  lcd.setCursor(13, 0);
  lcd.print(location);
  lcd.setCursor(0, 1);
  lcd.print(bonus);
  lcd.setCursor(11, 1);
  lcd.print("Send?");
  delay(3000); // Delay to keep the message on the screen
}

void handleLongPress(int buttonPin, unsigned long &lastButtonPressTime, String location, String bonus) {
  String shift = "";

  // Determine the shift based on which button was pressed
  if (buttonPin == dayButtonPin) {
    shift = "6AM-2PM";
  } else if (buttonPin == eveningButtonPin) {
    shift = "2PM-10PM";
  } else if (buttonPin == nightButtonPin) {
    shift = "10PM-6AM";
  }

  // Check if the button is still pressed and if the long press duration has been met
  if (digitalRead(buttonPin) == LOW) {
    if (millis() - lastButtonPressTime > longPressDuration) {
      sendShiftAlert(shift, location, bonus); // Send notification
      lastButtonPressTime = 0; // Reset to prevent multiple alerts
      while (digitalRead(buttonPin) == LOW); // Wait for button release
    }
  } else {
    lastButtonPressTime = 0; // Reset if button is released early
  }
}

void sendShiftAlert(String shift, String location, String bonus) {
  // Compose the message
  String message = "Open Shift Available:\n";
  message += "*Shift: * " + shift + "\n";
  message += "*Location: * " + location + "\n";
  message += "*Bonus:* " + bonus + "\n";
  message += "Please call if you can cover the shift, thank you";

  // URL encode the message
  String encodedMessage = urlEncode(message);

  // Send the message to each chat ID
  for (int i = 0; i < numChatIDs; i++) {
    String chatID = chatIDs[i];
    String url = "https://api.telegram.org/bot" + String(botToken) + "/sendMessage?chat_id=" + chatID + "&text=" + encodedMessage;

    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET();

    // Handle the HTTP response
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println("HTTP Response code: " + String(httpCode));
      Serial.println("HTTP payload: " + payload);
    } else {
      Serial.println("Error on HTTP request");
    }

    http.end(); // Close the connection
    delay(1500); // Small delay to avoid overloading the server
  }
}
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
