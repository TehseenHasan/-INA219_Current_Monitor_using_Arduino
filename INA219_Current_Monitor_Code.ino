#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_INA219.h>

//Declaration for INA-219 Sensor
Adafruit_INA219 ina219;

//Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup()
{
  Serial.begin(115200);

  // Initialize the INA219.
  if (! ina219.begin())
  {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range.
  // To use a slightly lower range (32V, 1A ) (higher precision on amps), uncomment the below line
  ina219.setCalibration_32V_1A();
  // Or to use a lower range (16V, 400mA) (higher precision on volts and amps), uncomment the below line
  //ina219.setCalibration_16V_400mA();

  // Initialize the OLED Display.
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) //Address 0x3C for 128x64 OLED Display
  {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  //Displaying the boot message on OLED Display
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(20,10);
  display.println("INA-219");
  display.setTextSize(1);
  display.setCursor(20,40);
  display.println("Current Monitor");
  display.drawRoundRect(0, 00, 128, 64, 8, WHITE);
  display.display();
  delay(5000);
  display.clearDisplay();
  display.display();
}

void loop()
{
  // Initilizing variables
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;
  
  // Reading Values from INA219 Current Sensor
  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);

  //Displaying data to Serial Monitor
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
  Serial.println("");

  //Displaying data to OLED Display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Bus Volt:   "); display.print(busvoltage); display.println(" V"); display.setCursor(0,13);
  display.print("Shunt Volt: "); display.print(shuntvoltage); display.println(" mV"); display.setCursor(0,26);
  display.print("Load Volt:  "); display.print(loadvoltage); display.println(" V"); display.setCursor(0,39);
  display.print("Current:    "); display.print(current_mA); display.println(" mA"); display.setCursor(0,52);
  display.print("Power:      "); display.print(power_mW); display.println(" mW");
  display.display();

  delay(500);  
}
