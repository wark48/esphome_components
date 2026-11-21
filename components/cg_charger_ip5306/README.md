> [!IMPORTANT]
> CURRENTLY IN THE TESTING PHASE

# IP5306 battery charger 

This component provides support for the Climateguard IP5306 battery charger I2C.

[Manufacturer's Website](https://climateguard.ru/cg_charger_ip/)

[DataSheet (RU)](https://github.com/climateguard/CG_charger_IP)

[IP5306 i2C Libs (Arduino)](https://github.com/AvinasheeTech/IP5306_I2C)

You need an `i2c:` component configured.  All the sensors are optional.

Example:
```yaml
external_components:
  - source:
      type: git
      url: https://github.com/wark48/esphome_components
    components: [cg_charger_ip5306]

i2c:
  sda: GPIO21
  scl: GPIO22
  scan: true
  frequency: 400kHz

cg_charger_ip5306:
  id: cg_charger_ip
  address: 0x75   # 7-bit adress 0xEA
  charging_current: 0.6 # Ampere

binary_sensor: # Be sure to add this; the values can be left empty.
  - platform: cg_charger_ip5306
    cg_charger_ip5306_id: cg_charger_ip
    sensor_type: charge_enabled
    name: "IP5306 Charge Enabled"

  - platform: cg_charger_ip5306
    cg_charger_ip5306_id: cg_charger_ip
    sensor_type: charge_complete
    name: "IP5306 Charge Complete"

  - platform: cg_charger_ip5306
    cg_charger_ip5306_id: cg_charger_ip
    sensor_type: light_load
    name: "IP5306 Light Load"

  - platform: cg_charger_ip5306
    cg_charger_ip5306_id: cg_charger_ip
    sensor_type: short_press
    name: "IP5306 Short Press"

  - platform: cg_charger_ip5306
    cg_charger_ip5306_id: cg_charger_ip
    sensor_type: long_press
    name: "IP5306 Long Press"

  - platform: cg_charger_ip5306
    cg_charger_ip5306_id: cg_charger_ip
    sensor_type: double_press
    name: "IP5306 Double Press"

sensor:
  - platform: template
    name: "IP5306 Battery Level"
    unit_of_measurement: "%"
    accuracy_decimals: 0
    device_class: battery
    state_class: measurement
    update_interval: 60s
    lambda: |-
      uint8_t level = id(cg_charger_ip).get_battery_level();
      if (level == 255) return NAN;
      return (float) level;
```
