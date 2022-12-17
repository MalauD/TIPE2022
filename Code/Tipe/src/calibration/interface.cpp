#include "interface.hpp"

template <typename T, std::size_t size>
void CalInterface<T, size>::start(Config<T, size> &config)
{
    Serial.println("Starting calibration");
    Serial.println("Test:");

    dataSet.clear();
    adc.begin();
    adc.set_gain(GAIN_ONE);
    auto test_reading = adc.one_shot_reading(0);
    test_reading.print();

    Serial.println("Extend the current config ? [y/n]");

    int choice = -1;
    while (true)
    {
        while (!Serial.available())
            ;
        choice = Serial.read();
        if (choice == 'y')
        {
            Serial.println("Extending config");
            break;
        }
        else if (choice == 'n')
        {
            Serial.println("Not extending config");
            break;
        }
    }
    Serial.println("Choose a channel to calibrate (1-4):");
    int channel = 0;
    while (channel <= 0 || channel >= 5)
    {
        while (!Serial.available())
            ;
        channel = Serial.parseInt();
    }

    Serial.println("Enter value to start calibration - channel " + String(channel) + " - (in grams):");

    while (true)
    {
        while (!Serial.available())
            ;
        float val = Serial.parseFloat();
        if (val == -1)
        {
            break;
        }
        if (val != 0)
        {
            auto adc_reading = adc.one_shot_reading(0);
            adc_reading.print();
            auto reading = adc_reading.getAdcValueByIndexInVolts(channel - 1);
            Serial.print("Value: ");
            Serial.println(val);
            Serial.print("Adc: ");
            Serial.println(reading);
            Serial.print("Inverse Adc: ");
            Serial.println(1 / reading);
            dataSet.appendDataPoint({1 / reading, val});
        }
    }

    if (choice == 'y')
    {
        config->extendDatasetAt(dataSet, channel - 1);
    }
    else
    {
        config->setDatasetAt(dataSet, channel - 1);
    }

    auto newDataset = config->getDatasetAt(channel - 1);
    auto result = linearRegression(newDataset);
    Serial.print("Slope: ");
    Serial.println(result.slope);
    Serial.print("Intercept: ");
    Serial.println(result.intercept);
    Serial.print("R: ");
    Serial.println(result.r);
    config->setFittingResultAt(result, channel - 1);
}