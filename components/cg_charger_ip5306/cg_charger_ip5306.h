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
static const uint8_t CG_IP5306_REG_BAT_LEVEL = 0x78;  // недокументирован, но рабочий

class CGChargerIP5306Component : public Component, public i2c::I2CDevice {
 public:
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override { return 990.0f; }

  // Конфигурация из YAML
  void set_charging_current_config(float current) { this->configured_current_ = current; }
  void set_boost_mode_config(bool v) { this->cfg_boost_mode_ = v; this->has_boost_mode_ = true; }
  void set_charger_mode_config(bool v) { this->cfg_charger_mode_ = v; this->has_charger_mode_ = true; }
  void set_power_on_load_config(bool v) { this->cfg_power_on_load_ = v; this->has_power_on_load_ = true; }
  void set_boost_always_on_config(bool v) { this->cfg_boost_always_on_ = v; this->has_boost_always_on_ = true; }
  void set_button_shutdown_config(bool v) { this->cfg_button_shutdown_ = v; this->has_button_shutdown_ = true; }

  void set_boost_shutdown_long_press_config(bool v) { this->cfg_boost_shutdown_long_press_ = v; this->has_boost_shutdown_long_press_ = true; }
  void set_led_double_press_config(bool v) { this->cfg_led_double_press_ = v; this->has_led_double_press_ = true; }
  void set_short_press_boost_config(bool v) { this->cfg_short_press_boost_ = v; this->has_short_press_boost_ = true; }
  void set_boost_after_vin_config(bool v) { this->cfg_boost_after_vin_ = v; this->has_boost_after_vin_ = true; }
  void set_low_battery_shutdown_config(bool v) { this->cfg_low_battery_shutdown_ = v; this->has_low_battery_shutdown_ = true; }

  void set_long_press_time_config(bool v) { this->cfg_long_press_time_ = v; this->has_long_press_time_ = true; }
  void set_light_load_shutdown_time_config(uint8_t v) { this->cfg_light_load_shutdown_time_ = v; this->has_light_load_shutdown_time_ = true; }

  void set_charging_stop_voltage_config(uint8_t v) { this->cfg_charging_stop_voltage_ = v; this->has_charging_stop_voltage_ = true; }
  void set_end_charge_current_config(uint8_t v) { this->cfg_end_charge_current_ = v; this->has_end_charge_current_ = true; }
  void set_charger_under_voltage_config(uint8_t v) { this->cfg_charger_under_voltage_ = v; this->has_charger_under_voltage_ = true; }

  void set_battery_voltage_config(uint8_t v) { this->cfg_battery_voltage_ = v; this->has_battery_voltage_ = true; }
  void set_voltage_pressure_config(uint8_t v) { this->cfg_voltage_pressure_ = v; this->has_voltage_pressure_ = true; }

  void set_cc_loop_vin_config(bool v) { this->cfg_cc_loop_vin_ = v; this->has_cc_loop_vin_ = true; }

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
  // time: 0=8s, 1=32s, 2=16s, 3=64s (нелинейно по даташиту)
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
  bool is_charge_enabled();      // 0x70 bit3 (бывший is_charging)
  bool is_charge_complete();     // 0x71 bit3
  bool is_light_load();          // 0x72 bit2
  bool is_short_press();         // 0x77 bit0
  bool is_long_press();          // 0x77 bit1
  bool is_double_press();        // 0x77 bit2
  uint8_t get_battery_level();   // 0x78 (недокументирован)

  void clear_press_flag_(uint8_t mask);

 protected:
  float configured_current_{0.6f};

  bool has_boost_mode_{false};        bool cfg_boost_mode_{false};
  bool has_charger_mode_{false};      bool cfg_charger_mode_{false};
  bool has_power_on_load_{false};     bool cfg_power_on_load_{false};
  bool has_boost_always_on_{false};   bool cfg_boost_always_on_{false};
  bool has_button_shutdown_{false};   bool cfg_button_shutdown_{false};

  bool has_boost_shutdown_long_press_{false}; bool cfg_boost_shutdown_long_press_{false};
  bool has_led_double_press_{false};          bool cfg_led_double_press_{false};
  bool has_short_press_boost_{false};         bool cfg_short_press_boost_{false};
  bool has_boost_after_vin_{false};           bool cfg_boost_after_vin_{false};
  bool has_low_battery_shutdown_{false};      bool cfg_low_battery_shutdown_{false};

  bool has_long_press_time_{false};           bool cfg_long_press_time_{false};
  bool has_light_load_shutdown_time_{false};  uint8_t cfg_light_load_shutdown_time_{0};

  bool has_charging_stop_voltage_{false};     uint8_t cfg_charging_stop_voltage_{0};
  bool has_end_charge_current_{false};        uint8_t cfg_end_charge_current_{0};
  bool has_charger_under_voltage_{false};     uint8_t cfg_charger_under_voltage_{0};

  bool has_battery_voltage_{false};           uint8_t cfg_battery_voltage_{0};
  bool has_voltage_pressure_{false};          uint8_t cfg_voltage_pressure_{0};

  bool has_cc_loop_vin_{false};               bool cfg_cc_loop_vin_{false};

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
