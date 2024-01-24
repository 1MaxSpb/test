#include <Wire.h>
#include <MCP3221.h>
const byte DEV_ADDR = 0x4C; // Адрес микросхемы с перемычкой - A0
MCP3221 mcp3221(DEV_ADDR);
bool t;



#include <Wire.h>
#include "TLC59108.h"

#define HW_RESET_PIN 0 // Только программный сброс
#define I2C_ADDR TLC59108::I2C_ADDR::BASE

TLC59108 leds(I2C_ADDR + 0);


#define I2C_HUB_ADDR 0x70  // настройки I2C для платы MGB-I2C63EN
#define EN_MASK 0x08
#define DEF_CHANNEL 0x00
#define MAX_CHANNEL 0x08

// I2C порт 0x07 - выводы gp16 (SDA), gp17 (SCL)
// I2C порт 0x06 - выводы gp4 (SDA), gp13 (SCL)
// I2C порт 0x05 - выводы gp14 (SDA), gp15 (SCL)
// I2C порт 0x04 - выводы gp5 (SDA), gp23 (SCL)
// I2C порт 0x03 - выводы gp19 (SDA), gp18 (SCL)

void setup() {
 Serial.begin(115200);
 Wire.begin();

// Инициализация модуля
  Wire.begin();
  leds.init(HW_RESET_PIN);
  leds.setLedOutputMode(TLC59108::LED_MODE::PWM_IND);

}
void loop() {

 float adc0 = mcp3221.getVoltage(); // измеряем уровень шума
 //Serial.println(adc0);
 if (adc0 > 1300) { // если уровень шума переходит за уровень 1300
 t = !t; // то включаем/выключаем свет
Serial.println(t);

setBusChannel(0x07);
delay(1000);

if (t == 1 ){
  Serial.println("on");
      leds.setBrightness(0, 0xff);
    leds.setBrightness(6, 0xff);
}
if (t == 0){
  Serial.println("off");
      leds.setBrightness(0, 0x00);
    leds.setBrightness(6, 0x00);
}

}

}
// Функция установки нужного выхода I2C


//функция смены I2C-порта
bool setBusChannel(uint8_t i2c_channel) {
  if (i2c_channel >= MAX_CHANNEL) {
    return false;
  } else {
    Wire.beginTransmission(I2C_HUB_ADDR);
    Wire.write(i2c_channel | EN_MASK);
    Wire.endTransmission();
    return true;
  }
}