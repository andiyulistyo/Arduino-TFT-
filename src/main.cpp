#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>

// Definisi Pin Koneksi
// TFT Display Pins
#define TFT_MISO 19 // MISO (dapat diabaikan jika tidak membaca dari display)
#define TFT_MOSI 23 // MOSI / SDI / Data In
#define TFT_SCLK 18 // SCK / Clock
#define TFT_CS 15   // Chip Select
#define TFT_DC 2    // Data/Command
#define TFT_RST 4   // Reset

// Touch Screen Pins (jika menggunakan touch)
#define TOUCH_CS 5   // Touch Chip Select
#define TOUCH_IRQ 17 // Touch Interrupt (optional)

TFT_eSPI tft = TFT_eSPI();

void setup()
{
  Serial.begin(115200);

  tft.init();
  tft.setRotation(1); // Landscape mode
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("ESP32 + ILI9341");
  tft.println("TFT Display Ready!");
  tft.println("Indonesia merah putih");

  // draw a button an clicked
  tft.fillRect(50, 100, 220, 60, TFT_BLUE);
  tft.setCursor(90, 120);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.setTextSize(3);
  tft.println("CLICK ME");
}

void loop()
{
  // put your main code here, to run repeatedly:
  // action click button click me
  uint16_t x, y;
  if (tft.getTouch(&x, &y))
  {
    // Cek apakah sentuhan di area tombol (50, 100, 220x60)
    if (x >= 50 && x <= 270 && y >= 100 && y <= 160)
    {
      Serial.println("Button Clicked!");
      tft.fillRect(50, 100, 220, 60, TFT_GREEN);
      tft.setCursor(90, 120);
      tft.setTextColor(TFT_WHITE, TFT_GREEN);
      tft.setTextSize(3);
      tft.println("CLICKED!");
      delay(300);
      // tft.fillRect(50, 100, 220, 60, TFT_BLUE);
      tft.fillRect(50, 100, 220, 60, TFT_BLUE);
      tft.setCursor(90, 120);
      tft.setTextColor(TFT_WHITE, TFT_BLUE);
      tft.setTextSize(3);
      tft.println("CLICK ME");
     }
  }
}
