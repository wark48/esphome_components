> [!IMPORTANT]
> CURRENTLY IN THE TESTING PHASE

# IP5306 battery charger 

This component provides support for the Climateguard IP5306 battery charger I2C.

DataSheet: 
[Github](https://github.com/climateguard/CG_charger_IP)
[Manufacturer's Website](https://climateguard.ru/cg_charger_ip/)

You need an `i2c:` component configured.  All the sensors are optional.

Example:
```yaml
external_components:
  - source: github://wark48/esphome_components
    components: [cg_charger_ip5306]
    refresh: 600s

i2c:
  sda: GPIO21
  scl: GPIO22
  scan: true

cg_charger_ip5306:
  id: cg_charger_ip
  address: 0x75
  charging_current: 0.6 # Set current Ampere of battery charging

binary_sensor:
  - platform: cg_charger_ip5306
    cg_charger_ip5306_id: cg_charger_ip
    sensor_type: charging
    name: "Charging Status"

  - platform: cg_charger_ip5306
    cg_charger_ip5306_id: cg_charger_ip
    sensor_type: charge_complete
    name: "Charge Complete"

sensor:
  - platform: template
    name: "Battery Level"
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
