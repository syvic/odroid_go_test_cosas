#include <odroid_go.h>
#include <driver/adc.h>
#include <esp_adc_cal.h>

#define RESISTANCE_NUM    2
#define DEFAULT_VREF      1100

static esp_adc_cal_characteristics_t adc_chars;

void bat_init() {
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, DEFAULT_VREF, &adc_chars);
}

double bat_read() {
  return (double) esp_adc_cal_raw_to_voltage(adc1_get_raw(ADC1_CHANNEL_0), &adc_chars) * RESISTANCE_NUM / 1000;
}

void bat_show() {
  static unsigned long t_bat;

  if (millis() - t_bat > 5000) {
    t_bat = millis();
    //GO.lcd.clear();
    GO.lcd.setCursor(160, 0);

    GO.lcd.printf("Current Voltage: %1.3lf V \n", bat_read());
  }
}
