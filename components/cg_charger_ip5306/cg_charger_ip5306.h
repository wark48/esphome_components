#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

namespace esphome {
namespace cg_charger_ip5306 {

// Регистры IP5306 (согласно даташиту CG Charger IP)
static const uint8_t CG_IP5306_REG_SYS_CTL0 = 0x00;
static const uint8_t CG_IP5306_REG_SYS_CTL1 = 0x01;
static const uint8_t CG_IP5306_REG_SYS_CTL2 = 0x02;
static const uint8_t CG_IP5306_REG_CHARGER_CTL0 = 0x20;
static const uint8_t CG_IP5306_REG_CHARGER_CTL1 = 0x21;
static const uint8_t CG_IP5306_REG_CHARGER_CTL2 = 0x22;
static const uint8_t CG_IP5306_REG_CHARGER_CTL3 = 0x23;
static const uint8_t CG_IP5306_REG_CHG_DIG_CTL0 = 0x24;
static const uint8_t CG_IP5306_REG_READ0 = 0x70;
static const uint8_t CG_IP5306_REG_READ1 = 0x71;
static const uint8_t CG_IP5306_REG_READ2 = 0x72;
static const uint8_t CG_IP5306_REG_READ3 = 0x77;
static const uint8_t CG_IP5306_REG_BAT_LEVEL = 0x78;

class CGChargerIP5306Component : public Component, public i2c::I2CDevice {
 public:
  void setup() override;
  void dump_config() override;
  // Приоритет 990: сразу после I2C (1000), до остальных компонентов DATA (600)
  float get_setup_priority() const override { return 990.0f; }

  void set_charging_current_config(float current) { this->configured_current_ = current; }

  // SYS_CTL0
  void boost_mode(bool enable);
  void charger_mode(bool enable);
  void power_on_load(bool enable);
  void boost_always_on(bool enable);
  void button_shutdown_enable(bool enable);

  // SYS_CTL1
  void set_boost_shutdown_signal(bool long_press);
  void set_led_signal(bool double_press);
  void short_press_boost_enable(bool enable);
  void boost_after_vin(bool enable);
  void low_battery_shutdown_enable(bool enable);

  // SYS_CTL2
  void set_long_press_time(bool three_seconds);
  void set_light_load_shutdown_time(uint8_t time);

  // CHARGER_CTL0
  void set_charging_stop_voltage(uint8_t level);

  // CHARGER_CTL1
  void set_end_charge_current(uint8_t level);
  void set_charger_under_voltage(uint8_t level);

  // CHARGER_CTL2
  void set_battery_voltage(uint8_t level);
  void set_voltage_pressure(uint8_t level);

  // CHARGER_CTL3
  void set_cc_loop(bool vin_loop);

  // CHG_DIG_CTL0
  void set_charging_current(float current);

  // Статусы
  bool is_charging();
  bool is_charge_complete();
  bool is_light_load();
  bool is_short_press();
  bool is_long_press();
  bool is_double_press();
  uint8_t get_battery_level();

 protected:
  float configured_current_{0.6f};
  bool read_register_(uint8_t reg, uint8_t *value);
  bool write_register_(uint8_t reg, uint8_t value);
  bool update_register_bits_(uint8_t reg, uint8_t mask, uint8_t value);
  bool update_register_bit_(uint8_t reg, uint8_t bit, bool value);
};

class CGChargerIP5306BinarySensor : public binary_sensor::BinarySensor, public PollingComponent {
 public:
  void set_parent(CGChargerIP5306Component *parent) { this->parent_ = parent; }
  void set_sensor_type(const std::string &type) { this->sensor_type_ = type; }
  void update() override;

 protected:
  CGChargerIP5306Component *parent_{nullptr};
  std::string sensor_type_;
};

}  // namespace cg_charger_ip5306
}  // namespace esphome
