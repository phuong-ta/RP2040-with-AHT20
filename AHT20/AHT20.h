#ifndef AHT20_H
#define AHT20_H

class AHT20 {
private:
    float tempReading = 0;
    float humReading = 0;
    const static int addr = 0x38;

    bool checkCalibration();
    void sensorInit();

public:
    AHT20();

    void triggerMeasurement();
    float getTemp();
    float getTemp_f();
    float getHumidity();
    void init(int sdaPort, int sclPort);
};


#endif //_AHT20_H