#include "interface.hpp"

void CalInterface::start()
{
    Serial.println("Starting calibration");
    Serial.println("Test:");
    adc.begin();
    adc.set_gain(GAIN_ONE);
    auto reading = adc.one_shot_reading(0);
    Serial.print("Adc1: ");
    Serial.print(reading.getAdcValueByAddr(AdcAddr::ADC1));
    Serial.print(" Adc2: ");
    Serial.print(reading.getAdcValueByAddr(AdcAddr::ADC2));
    Serial.print(" Adc3: ");
    Serial.print(reading.getAdcValueByAddr(AdcAddr::ADC3));
    Serial.print(" Adc4: ");
    Serial.println(reading.getAdcValueByAddr(AdcAddr::ADC4));
    Serial.println("Enter value to start calibration");
    while (!Serial.available())
        ;
    while (true)
    {
        float val = Serial.parseFloat();
        if (val == -1)
        {
            break;
        }
        if (val != 0)
        {
            Serial.print("Value: ");
            Serial.println(val);
            Serial.print("Adc: ");
            auto reading = adc.one_shot_reading(0).getAdcValueByAddrInVolts(AdcAddr::ADC1);
            Serial.println(reading);
            Serial.print("Inverse Adc: ");
            Serial.println(1 / reading);
            dataSet.appendDataPoint({1 / reading, val});
        }
    }
    auto result = linearRegression(dataSet);
    Serial.print("Slope: ");
    Serial.println(result.slope);
    Serial.print("Intercept: ");
    Serial.println(result.intercept);
    Serial.print("R: ");
    Serial.println(result.r);
}