#include "cg_charger_ip5306.h"
#include "esphome/core/log.h"

namespace esphome {
namespace cg_charger_ip5306 {

static const char *const TAG = "cg_charger_ip5306";

void CGChargerIP5306Component::setup() {
  // КРИТИЧНО: немедленно ограничиваем ток заряда значением из YAML
  this->set_charging_current(this->configured_current_);

  ESP_LOGCONFIG(TAG, "Setting up CG Charger IP5306 (charging_current=%.2fA)...",
                this->configured_current_);
  uint8_t value;
  if (!this->read_register_(CG_IP5306_REG_SYS_CTL0, &value)) {
    ESP_LOGE(TAG, "Communication with CG Charger IP5306 failed!");
    this->mark_failed();  // Вызов, НЕ переопределение (breaking change 2026.3)
  }
}

void CGChargerIP5306Component::dump_config() {
  ESP_LOGCONFIG(TAG, "CG Charger IP5306:");
  LOG_I2C_DEVICE(this);
  if (this->is_failed()) {
    ESP_LOGE(TAG, "Communication with CG Charger IP5306 failed!");
  }
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
bool CGChargerIP5306Component::is_charging() {
  uint8_t value;
  if (this->read_register_(CG_IP5306_REG_READ0, &value)) {
    return (value & 0x08) != 0;
  }
  return false;
}
bool CGChargerIP5306Component::is_charge_complete() {
  uint8_t value;
  if (this->read_register_(CG_IP5306_REG_READ1, &value)) {
    return (value & 0x08) != 0;
  }
  return false;
}
bool CGChargerIP5306Component::is_light_load() {
  uint8_t value;
  if (this->read_register_(CG_IP5306_REG_READ2, &value)) {
    return (value & 0x04) != 0;
  }
  return false;
}
bool CGChargerIP5306Component::is_short_press() {
  uint8_t value;
  if (this->read_register_(CG_IP5306_REG_READ3, &value)) {
    return (value & 0x01) != 0;
  }
  return false;
}
bool CGChargerIP5306Component::is_long_press() {
  uint8_t value;
  if (this->read_register_(CG_IP5306_REG_READ3, &value)) {
    return (value & 0x02) != 0;
  }
  return false;
}
bool CGChargerIP5306Component::is_double_press() {
  uint8_t value;
  if (this->read_register_(CG_IP5306_REG_READ3, &value)) {
    return (value & 0x04) != 0;
  }
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
    default:   return 0;
  }
}

// --- Бинарный сенсор ---
void CGChargerIP5306BinarySensor::update() {
  if (this->parent_ == nullptr) return;
  bool value = false;
  if (this->sensor_type_ == "charging") {
    value = this->parent_->is_charging();
  } else if (this->sensor_type_ == "charge_complete") {
    value = this->parent_->is_charge_complete();
  } else if (this->sensor_type_ == "light_load") {
    value = this->parent_->is_light_load();
  } else if (this->sensor_type_ == "short_press") {
    value = this->parent_->is_short_press();
  } else if (this->sensor_type_ == "long_press") {
    value = this->parent_->is_long_press();
  } else if (this->sensor_type_ == "double_press") {
    value = this->parent_->is_double_press();
  }
  this->publish_state(value);
}

}  // namespace cg_charger_ip5306
}  // namespace esphome
