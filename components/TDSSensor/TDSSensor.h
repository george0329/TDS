#include "esphome.h"
using namespace esphome;

class TDSSensorComponent : public Component, public uart::UARTDevice {
 public:
  TDSSensorComponent(UARTComponent *parent) : UARTDevice(parent) {}
  Sensor *tds1 = new Sensor();
  Sensor *tds2 = new Sensor();
  Sensor *temperature1 = new Sensor();
  Sensor *temperature2 = new Sensor();

  void loop() override {
    if (available()) {
      unsigned char data[11];
      if (read_array(data, sizeof(data)) && data[0] == 0x55 && data[1] == 0x0A && data[2] == 0x85) {
        int channel = data[3];
        int tds = (data[4] * 255 + data[5]) / 20;
        float temperature = (data[6] * 255 + data[7]) / 10.0;

        if (channel == 1) {
          tds1->publish_state(tds);
          temperature1->publish_state(temperature);
        } else if (channel == 2) {
          tds2->publish_state(tds);
          temperature2->publish_state(temperature);
        }
      }
    }
  }
};
