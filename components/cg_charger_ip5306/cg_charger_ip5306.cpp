#include "cg_charger_ip5306.h"
#include "esphome/core/log.h"

namespace esphome {
namespace cg_charger_ip5306 {

static const char *const TAG = "cg_charger_ip5306";

void CGChargerIP5306Component::setup() {
  uint8_t value;
  if (!this->read_register_(CG_IP5306_REG_SYS_CTL0, &value)) {
    ESP_LOGE(TAG, "Communication with CG Charger IP5306 failed!");
    this->mark_failed();
    return;
  }
  ESP_LOGCONFIG(TAG, "Setting up CG Charger IP5306 (charging_current=%.2fA)...",
                this->configured_current_);

  // Применяем конфигурацию из YAML (только заданные параметры)
  this->set_charging_current(this->configured_current_);

  if (this->has_boost_mode_) this->boost_mode(this->cfg_boost_mode_);
  if (this->has_charger_mode_) this->charger_mode(this->cfg_charger_mode_);
  if (this->has_power_on_load_) this->power_on_load(this->cfg_power_on_load_);
  if (this->has_boost_always_on_) this->boost_always_on(this->cfg_boost_always_on_);
  if (this->has_button_shutdown_) this->button_shutdown_enable(this->cfg_button_shutdown_);

  if (this->has_boost_shutdown_long_press_) this->set_boost_shutdown_signal(this->cfg_boost_shutdown_long_press_);
  if (this->has_led_double_press_) this->set_led_signal(this->cfg_led_double_press_);
  if (this->has_short_press_boost_) this->short_press_boost_enable(this->cfg_short_press_boost_);
  if (this->has_boost_after_vin_) this->boost_after_vin(this->cfg_boost_after_vin_);
  if (this->has_low_battery_shutdown_) this->low_battery_shutdown_enable(this->cfg_low_battery_shutdown_);

  if (this->has_long_press_time_) this->set_long_press_time(this->cfg_long_press_time_);
  if (this->has_light_load_shutdown_time_) this->set_light_load_shutdown_time(this->cfg_light_load_shutdown_time_);

  if (this->has_charging_stop_voltage_) this->set_charging_stop_voltage(this->cfg_charging_stop_voltage_);
  if (this->has_end_charge_current_) this->set_end_charge_current(this->cfg_end_charge_current_);
  if (this->has_charger_under_voltage_) this->set_charger_under_voltage(this->cfg_charger_under_voltage_);

  if (this->has_battery_voltage_) this->set_battery_voltage(this->cfg_battery_voltage_);
  if (this->has_voltage_pressure_) this->set_voltage_pressure(this->cfg_voltage_pressure_);

  if (this->has_cc_loop_vin_) this->set_cc_loop(this->cfg_cc_loop_vin_);
}

void CGChargerIP5306Component::dump_config() {
  ESP_LOGCONFIG(TAG, "CG Charger IP5306:");
  LOG_I2C_DEVICE(this);
  if (this->is_failed()) {
    ESP_LOGE(TAG, "Communication with CG Charger IP5306 failed!");
  }
  ESP_LOGCONFIG(TAG, "  Configured charging current: %.2fA", this->configured_current_);
}

bool CGChargerIP5306Component::read_register_(uint8_t reg, uint8_t *value) {
  return this->read_byte(reg, value);
}

bool CGChargerIP5306Component::write_register_(uint8_t reg, uint8_t value) {
  return this->write_byte(reg, value);
}

bool CGChargerIP5306Component::update_register_bits_(uint8_t reg, uint8_t mask, uint8_t value) {
  uint8_t current;
  if (!this->read_register_(reg, &current)) return false;
  current = (current & ~mask) | (value & mask);
  return this->write_register_(reg, current);
}

bool CGChargerIP5306Component::update_register_bit_(uint8_t reg, uint8_t bit, bool value) {
  return this->update_register_bits_(reg, (1 << bit), value ? (1 << bit) : 0);
}

// --- SYS_CTL0 (0x00) ---
void CGChargerIP5306Component::boost_mode(bool enable) {
  this->update_register_bit_(CG_IP5306_REG_SYS_CTL0, 5, enable);
}
void CGChargerIP5306Component::charger_mode(bool enable) {
  this->update_register_bit_(CG_IP5306_REG_SYS_CTL0, 4, enable);
}
void CGChargerIP5306Component::power_on_load(bool enable) {
  this->update_register_bit_(CG_IP5306_REG_SYS_CTL0, 2, enable);
}
void CGChargerIP5306Component::boost_always_on(bool enable) {
  this->update_register_bit_(CG_IP5306_REG_SYS_CTL0, 1, enable);
}
void CGChargerIP5306Component::button_shutdown_enable(bool enable) {
  this->update_register_bit_(CG_IP5306_REG_SYS_CTL0, 0, enable);
}

// --- SYS_CTL1 (0x01) ---
void CGChargerIP5306Component::set_boost_shutdown_signal(bool long_press) {
  this->update_register_bit_(CG_IP5306_REG_SYS_CTL1, 7, long_press);
}
void CGChargerIP5306Component::set_led_signal(bool double_press) {
  this->update_register_bit_(CG_IP5306_REG_SYS_CTL1, 6, double_press);
}
void CGChargerIP5306Component::short_press_boost_enable(bool enable) {
  this->update_register_bit_(CG_IP5306_REG_SYS_CTL1, 5, enable);
}
void CGChargerIP5306Component::boost_after_vin(bool enable) {
  this->update_register_bit_(CG_IP5306_REG_SYS_CTL1, 2, enable);
}
void CGChargerIP5306Component::low_battery_shutdown_enable(bool enable) {
  this->update_register_bit_(CG_IP5306_REG_SYS_CTL1, 0, enable);
}

// --- SYS_CTL2 (0x02) ---
void CGChargerIP5306Component::set_long_press_time(bool three_seconds) {
  this->update_register_bit_(CG_IP5306_REG_SYS_CTL2, 4, three_seconds);
}
void CGChargerIP5306Component::set_light_load_shutdown_time(uint8_t time) {
  this->update_register_bits_(CG_IP5306_REG_SYS_CTL2, 0x0C, (time & 0x03) << 2);
}

// --- CHARGER_CTL0 (0x20) ---
void CGChargerIP5306Component::set_charging_stop_voltage(uint8_t level) {
  this->update_register_bits_(CG_IP5306_REG_CHARGER_CTL0, 0x03, level & 0x03);
}

// --- CHARGER_CTL1 (0x21) ---
void CGChargerIP5306Component::set_end_charge_current(uint8_t level) {
  this->update_register_bits_(CG_IP5306_REG_CHARGER_CTL1, 0xC0, (level & 0x03) << 6);
}
void CGChargerIP5306Component::set_charger_under_voltage(uint8_t level) {
  this->update_register_bits_(CG_IP5306_REG_CHARGER_CTL1, 0x1C, (level & 0x07) << 2);
}

// --- CHARGER_CTL2 (0x22) ---
void CGChargerIP5306Component::set_battery_voltage(uint8_t level) {
  this->update_register_bits_(CG_IP5306_REG_CHARGER_CTL2, 0x0C, (level & 0x03) << 2);
}
void CGChargerIP5306Component::set_voltage_pressure(uint8_t level) {
  this->update_register_bits_(CG_IP5306_REG_CHARGER_CTL2, 0x03, level & 0x03);
}

// --- CHARGER_CTL3 (0x23) ---
void CGChargerIP5306Component::set_cc_loop(bool vin_loop) {
  this->update_register_bit_(CG_IP5306_REG_CHARGER_CTL3, 5, vin_loop);
}

// --- CHG_DIG_CTL0 (0x24) ---
void CGChargerIP5306Component::set_charging_current(float current) {
  if (current < 0.05f) current = 0.05f;
  if (current > 2.1f) current = 2.1f;
  float temp = current - 0.05f;
  uint8_t bits = 0;
  if (temp >= 0.1f) { bits |= 0x01; temp -= 0.1f; }
  if (temp >= 0.2f) { bits |= 0x02; temp -= 0.2f; }
  if (temp >= 0.4f) { bits |= 0x04; temp -= 0.4f; }
  if (temp >= 0.8f) { bits |= 0x08; temp -= 0.8f; }
  if (temp >= 1.6f) { bits |= 0x10; }
  this->update_register_bits_(CG_IP5306_REG_CHG_DIG_CTL0, 0x1F, bits);
}

// --- Статусы ---
bool CGChargerIP5306Component::is_charge_enabled() {
  uint8_t value;
  if (this->read_register_(CG_IP5306_REG_READ0, &value)) {
    return (value & 0x08) != 0;
  }
  ESP_LOGW(TAG, "Failed to read charge_en flag");
  return false;
}
bool CGChargerIP5306Component::is_charge_complete() {
  uint8_t value;
  if (this->read_register_(CG_IP5306_REG_READ1, &value)) {
    return (value & 0x08) != 0;
  }
  ESP_LOGW(TAG, "Failed to read charge_complete flag");
  return false;
}
bool CGChargerIP5306Component::is_light_load() {
  uint8_t value;
  if (this->read_register_(CG_IP5306_REG_READ2, &value)) {
    return (value & 0x04) != 0;
  }
  ESP_LOGW(TAG, "Failed to read light_load flag");
  return false;
}
bool CGChargerIP5306Component::is_short_press() {
  uint8_t value;
  if (this->read_register_(CG_IP5306_REG_READ3, &value)) {
    return (value & 0x01) != 0;
  }
  ESP_LOGW(TAG, "Failed to read short_press flag");
  return false;
}
bool CGChargerIP5306Component::is_long_press() {
  uint8_t value;
  if (this->read_register_(CG_IP5306_REG_READ3, &value)) {
    return (value & 0x02) != 0;
  }
  ESP_LOGW(TAG, "Failed to read long_press flag");
  return false;
}
bool CGChargerIP5306Component::is_double_press() {
  uint8_t value;
  if (this->read_register_(CG_IP5306_REG_READ3, &value)) {
    return (value & 0x04) != 0;
  }
  ESP_LOGW(TAG, "Failed to read double_press flag");
  return false;
}

uint8_t CGChargerIP5306Component::get_battery_level() {
  uint8_t value;
  if (!this->read_register_(CG_IP5306_REG_BAT_LEVEL, &value)) {
    return 255;
  }
  switch (value & 0xF0) {
    case 0xE0: return 25;
    case 0xC0: return 50;
    case 0x80: return 75;
    case 0x00: return 100;
    default:   return 255;  // неизвестно; README трактует 255 как NAN
  }
}

// Сброс флагов кнопок: "Write 1 Cleared" (регистр 0x77)
void CGChargerIP5306Component::clear_press_flag_(uint8_t mask) {
  this->update_register_bits_(CG_IP5306_REG_READ3, mask, mask);
}

// --- Бинарный сенсор ---
void CGChargerIP5306BinarySensor::update() {
  if (this->parent_ == nullptr) return;
  bool value = false;
  if (this->sensor_type_ == "charge_enabled") {
    value = this->parent_->is_charge_enabled();
  } else if (this->sensor_type_ == "charge_complete") {
    value = this->parent_->is_charge_complete();
  } else if (this->sensor_type_ == "light_load") {
    value = this->parent_->is_light_load();
  } else if (this->sensor_type_ == "short_press") {
    value = this->parent_->is_short_press();
    if (value) this->parent_->clear_press_flag_(0x01);
  } else if (this->sensor_type_ == "long_press") {
    value = this->parent_->is_long_press();
    if (value) this->parent_->clear_press_flag_(0x02);
  } else if (this->sensor_type_ == "double_press") {
    value = this->parent_->is_double_press();
    if (value) this->parent_->clear_press_flag_(0x04);
  }
  this->publish_state(value);
}

}  // namespace cg_charger_ip5306
}  // namespace esphome
