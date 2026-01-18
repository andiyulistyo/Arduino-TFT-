#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <Preferences.h>

class Calibration
{
private:
  TFT_eSPI *tft;
  Preferences preferences;
  uint16_t calData[5];

public:
  Calibration(TFT_eSPI *tftInstance)
  {
    tft = tftInstance;
  }

  // Simpan data kalibrasi ke flash memory
  void saveCalibration(uint16_t data[5])
  {
    preferences.begin("tft-config", false);
    for (int i = 0; i < 5; i++)
    {
      preferences.putUShort(("cal" + String(i)).c_str(), data[i]);
    }
    preferences.putBool("calibrated", true);
    preferences.end();
    Serial.println("✓ Kalibrasi disimpan ke flash memory");
  }

  // Load data kalibrasi dari flash memory
  bool loadCalibration(uint16_t data[5])
  {
    preferences.begin("tft-config", true);
    bool isCalibrated = preferences.getBool("calibrated", false);

    if (isCalibrated)
    {
      for (int i = 0; i < 5; i++)
      {
        data[i] = preferences.getUShort(("cal" + String(i)).c_str(), 0);
      }
      preferences.end();
      
      Serial.println("✓ Kalibrasi dimuat dari flash memory");
      Serial.print("Data: {");
      for (int i = 0; i < 5; i++)
      {
        Serial.print(data[i]);
        if (i < 4)
          Serial.print(", ");
      }
      Serial.println("}");
      
      return true;
    }
    preferences.end();
    Serial.println("⚠ Tidak ada data kalibrasi");
    return false;
  }

  // Lakukan proses kalibrasi touchscreen
  void performCalibration()
  {
    Serial.println("═══════════════════════════════════");
    Serial.println("Memulai proses kalibrasi...");
    Serial.println("═══════════════════════════════════");

    tft->fillScreen(TFT_BLACK);
    tft->setTextColor(TFT_WHITE, TFT_BLACK);
    tft->setTextSize(2);

    // Instruksi
    tft->setCursor(20, 80);
    tft->println("KALIBRASI LAYAR");
    tft->setCursor(20, 110);
    tft->setTextSize(1);
    tft->println("Sentuh tanda + yang muncul");
    tft->setCursor(20, 130);
    tft->println("pada setiap sudut layar");
    
    delay(2000);

    // Proses kalibrasi otomatis
    tft->calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

    // Tampilkan hasil
    Serial.println("✓ Kalibrasi selesai!");
    Serial.print("Data kalibrasi: {");
    for (int i = 0; i < 5; i++)
    {
      Serial.print(calData[i]);
      if (i < 4)
        Serial.print(", ");
    }
    Serial.println("}");

    // Simpan ke flash
    saveCalibration(calData);
    tft->setTouch(calData);

    // Tampilkan pesan sukses
    tft->fillScreen(TFT_BLACK);
    tft->setTextColor(TFT_GREEN, TFT_BLACK);
    tft->setTextSize(2);
    tft->setCursor(60, 100);
    tft->println("KALIBRASI");
    tft->setCursor(80, 125);
    tft->println("BERHASIL!");

    delay(1500);
    Serial.println("═══════════════════════════════════");
  }

  // Initialize - load kalibrasi atau lakukan kalibrasi baru
  bool initialize(bool forceCalibrate = false)
  {
    if (!forceCalibrate && loadCalibration(calData))
    {
      tft->setTouch(calData);
      return true;
    }
    else
    {
      Serial.println("Melakukan kalibrasi awal...");
      performCalibration();
      return false;
    }
  }

  // Hapus data kalibrasi
  void clearCalibration()
  {
    preferences.begin("tft-config", false);
    preferences.clear();
    preferences.end();
    Serial.println("✓ Data kalibrasi dihapus");
  }

  // Cek apakah sudah dikalibrasi
  bool isCalibrated()
  {
    preferences.begin("tft-config", true);
    bool calibrated = preferences.getBool("calibrated", false);
    preferences.end();
    return calibrated;
  }

  // Get resolusi layar
  String getResolution()
  {
    return String(tft->width()) + "x" + String(tft->height());
  }

  // Estimasi ukuran layar dalam inch berdasarkan resolusi
  String getScreenSize()
  {
    int w = tft->width();
    int h = tft->height();
    
    // Estimasi berdasarkan resolusi umum
    if ((w == 320 && h == 240) || (w == 240 && h == 320))
      return "2.8-3.2\"";
    else if ((w == 480 && h == 320) || (w == 320 && h == 480))
      return "3.5-4.0\"";
    else if ((w == 800 && h == 480) || (w == 480 && h == 800))
      return "7.0\"";
    else
      return "Unknown";
  }

  // Get info lengkap layar
  String getScreenInfo()
  {
    return getResolution() + " (" + getScreenSize() + ")";
  }
};

#endif
