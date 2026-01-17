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
#define TOUCH_CS 5 // Touch Chip Select

TFT_eSPI tft = TFT_eSPI();

// Screen states
enum Screen
{
  SCREEN_1,
  SCREEN_2,
  SCREEN_3
};

Screen currentScreen = SCREEN_1;
bool screenChanged = true;

// Struktur untuk button
struct Button
{
  int x, y, w, h;
  String label;
  uint16_t color;
};

// Fungsi untuk menggambar button
void drawButton(Button btn)
{
  tft.fillRoundRect(btn.x, btn.y, btn.w, btn.h, 8, btn.color);
  tft.drawRoundRect(btn.x, btn.y, btn.w, btn.h, 8, TFT_WHITE);

  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);

  // Center text
  int textWidth = btn.label.length() * 12;
  int textX = btn.x + (btn.w - textWidth) / 2;
  int textY = btn.y + (btn.h - 16) / 2;

  tft.setCursor(textX, textY);
  tft.print(btn.label);
}

// Fungsi untuk cek apakah button ditekan
bool isButtonPressed(Button btn, uint16_t x, uint16_t y)
{
  return (x >= btn.x && x <= btn.x + btn.w && y >= btn.y && y <= btn.y + btn.h);
}

// Gambar Screen 1
void drawScreen1()
{
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_CYAN);
  tft.setTextSize(3);
  tft.setCursor(20, 10);
  tft.println("SCREEN 1");

  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(20, 150);
  tft.println("Halaman Pertama");

  // Button untuk ke Screen 2 - posisi disesuaikan dengan touch
  Button btn = {60, 50, 200, 60, "Ke Screen 2", TFT_BLUE};
  drawButton(btn);
}

// Gambar Screen 2
void drawScreen2()
{
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_GREEN);
  tft.setTextSize(3);
  tft.setCursor(20, 10);
  tft.println("SCREEN 2");

  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(20, 150);
  tft.println("Halaman Kedua");

  // Button untuk ke Screen 1
  Button btn1 = {20, 50, 130, 50, "< Screen 1", TFT_ORANGE};
  drawButton(btn1);

  // Button untuk ke Screen 3
  Button btn2 = {170, 50, 130, 50, "Screen 3 >", TFT_PURPLE};
  drawButton(btn2);
}

// Gambar Screen 3
void drawScreen3()
{
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_MAGENTA);
  tft.setTextSize(3);
  tft.setCursor(20, 10);
  tft.println("SCREEN 3");

  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(20, 150);
  tft.println("Halaman Ketiga");

  // Button untuk ke Screen 2
  Button btn = {60, 50, 200, 60, "Ke Screen 2", TFT_DARKGREEN};
  drawButton(btn);
}

void setup()
{
  Serial.begin(115200);
  Serial.println("ESP32 TFT Touch Screen Starting...");

  tft.init();
  tft.setRotation(1); // Landscape mode
  
  // Hapus kalibrasi dulu untuk testing
  // uint16_t calData[5] = {275, 3620, 264, 3532, 1};
  // tft.setTouch(calData);

  Serial.println("Screen initialized");
  Serial.println("Touch the screen to see raw coordinates");
  drawScreen1();
}

void loop()
{
  // Jika screen berubah, gambar ulang
  if (screenChanged)
  {
    switch (currentScreen)
    {
    case SCREEN_1:
      drawScreen1();
      break;
    case SCREEN_2:
      drawScreen2();
      break;
    case SCREEN_3:
      drawScreen3();
      break;
    }
    screenChanged = false;
  }

  // Handle touch
  uint16_t x, y;
  if (tft.getTouch(&x, &y))
  {
    // Inverse koordinat Y karena terbalik (landscape mode 320x240)
    y = 240 - y;
    
    Serial.print("Touch detected: x=");
    Serial.print(x);
    Serial.print(", y=");
    Serial.print(y);
    Serial.print(" | Current Screen: ");
    Serial.println(currentScreen);
    
    // Tampilkan titik sentuh di layar untuk debugging
    tft.fillCircle(x, y, 3, TFT_RED);
    
    // Screen 1 - Button ke Screen 2
    if (currentScreen == SCREEN_1)
    {
      Button btn = {60, 50, 200, 60, "", TFT_BLUE};
      Serial.print("Checking button area: x[");
      Serial.print(btn.x);
      Serial.print("-");
      Serial.print(btn.x + btn.w);
      Serial.print("], y[");
      Serial.print(btn.y);
      Serial.print("-");
      Serial.print(btn.y + btn.h);
      Serial.println("]");
      
      if (isButtonPressed(btn, x, y))
      {
        Serial.println(">>> Button pressed! Pindah ke Screen 2");
        currentScreen = SCREEN_2;
        screenChanged = true;
        delay(300); // Debounce
      }
      else
      {
        Serial.println("Touch outside button area");
      }
    }
    // Screen 2 - 2 Buttons
    else if (currentScreen == SCREEN_2)
    {
      Button btn1 = {20, 50, 130, 50, "", TFT_ORANGE}; // ke Screen 1
      Button btn2 = {170, 50, 130, 50, "", TFT_PURPLE}; // ke Screen 3

      if (isButtonPressed(btn1, x, y))
      {
        Serial.println("Pindah ke Screen 1");
        currentScreen = SCREEN_1;
        screenChanged = true;
        delay(200);
      }
      else if (isButtonPressed(btn2, x, y))
      {
        Serial.println("Pindah ke Screen 3");
        currentScreen = SCREEN_3;
        screenChanged = true;
        delay(200);
      }
    }
    // Screen 3 - Button ke Screen 2
    else if (currentScreen == SCREEN_3)
    {
      Button btn = {60, 50, 200, 60, "", TFT_DARKGREEN};
      if (isButtonPressed(btn, x, y))
      {
        Serial.println("Pindah ke Screen 2");
        currentScreen = SCREEN_2;
        screenChanged = true;
        delay(200);
      }
    }
  }
}
