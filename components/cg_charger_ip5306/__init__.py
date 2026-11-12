import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import CONF_ID

DEPENDENCIES = ['i2c', 'binary_sensor']
MULTI_CONF = False

CONF_CHARGING_CURRENT = 'charging_current'

# SYS_CTL0 (0x00)
CONF_BOOST_MODE = 'boost_mode'
CONF_CHARGER_MODE = 'charger_mode'
CONF_POWER_ON_LOAD = 'power_on_load'
CONF_BOOST_ALWAYS_ON = 'boost_always_on'
CONF_BUTTON_SHUTDOWN_ENABLE = 'button_shutdown_enable'

# SYS_CTL1 (0x01)
CONF_BOOST_SHUTDOWN_LONG_PRESS = 'boost_shutdown_long_press'
CONF_LED_DOUBLE_PRESS = 'led_double_press'
CONF_SHORT_PRESS_BOOST = 'short_press_boost'
CONF_BOOST_AFTER_VIN = 'boost_after_vin'
CONF_LOW_BATTERY_SHUTDOWN = 'low_battery_shutdown'

# SYS_CTL2 (0x02)
CONF_LONG_PRESS_TIME_3S = 'long_press_time_3s'
CONF_LIGHT_LOAD_SHUTDOWN_TIME = 'light_load_shutdown_time'

# CHARGER_CTL0 (0x20)
CONF_CHARGING_STOP_VOLTAGE = 'charging_stop_voltage'

# CHARGER_CTL1 (0x21)
CONF_END_CHARGE_CURRENT = 'end_charge_current'
CONF_CHARGER_UNDER_VOLTAGE = 'charger_under_voltage'

# CHARGER_CTL2 (0x22)
CONF_BATTERY_VOLTAGE = 'battery_voltage'
CONF_VOLTAGE_PRESSURE = 'voltage_pressure'

# CHARGER_CTL3 (0x23)
CONF_CC_LOOP_VIN = 'cc_loop_vin'

cg_charger_ip5306_ns = cg.esphome_ns.namespace('cg_charger_ip5306')
CGChargerIP5306Component = cg_charger_ip5306_ns.class_(
    'CGChargerIP5306Component', cg.Component, i2c.I2CDevice
)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(CGChargerIP5306Component),
    cv.Optional(CONF_CHARGING_CURRENT, default=0.6): cv.float_range(min=0.05, max=2.1),

    # SYS_CTL0
    cv.Optional(CONF_BOOST_MODE): cv.boolean,
    cv.Optional(CONF_CHARGER_MODE): cv.boolean,
    cv.Optional(CONF_POWER_ON_LOAD): cv.boolean,
    cv.Optional(CONF_BOOST_ALWAYS_ON): cv.boolean,
    cv.Optional(CONF_BUTTON_SHUTDOWN_ENABLE): cv.boolean,

    # SYS_CTL1
    cv.Optional(CONF_BOOST_SHUTDOWN_LONG_PRESS): cv.boolean,
    cv.Optional(CONF_LED_DOUBLE_PRESS): cv.boolean,
    cv.Optional(CONF_SHORT_PRESS_BOOST): cv.boolean,
    cv.Optional(CONF_BOOST_AFTER_VIN): cv.boolean,
    cv.Optional(CONF_LOW_BATTERY_SHUTDOWN): cv.boolean,

    # SYS_CTL2
    cv.Optional(CONF_LONG_PRESS_TIME_3S): cv.boolean,
    # 0=8s, 1=32s, 2=16s, 3=64s (нелинейно по даташиту)
    cv.Optional(CONF_LIGHT_LOAD_SHUTDOWN_TIME): cv.int_range(min=0, max=3),

    # CHARGER_CTL0 (0..3)
    cv.Optional(CONF_CHARGING_STOP_VOLTAGE): cv.int_range(min=0, max=3),

    # CHARGER_CTL1
    cv.Optional(CONF_END_CHARGE_CURRENT): cv.int_range(min=0, max=3),
    cv.Optional(CONF_CHARGER_UNDER_VOLTAGE): cv.int_range(min=0, max=7),

    # CHARGER_CTL2
    cv.Optional(CONF_BATTERY_VOLTAGE): cv.int_range(min=0, max=3),
    cv.Optional(CONF_VOLTAGE_PRESSURE): cv.int_range(min=0, max=3),

    # CHARGER_CTL3
    cv.Optional(CONF_CC_LOOP_VIN): cv.boolean,
}).extend(cv.COMPONENT_SCHEMA).extend(i2c.i2c_device_schema(0x75))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    cg.add(var.set_charging_current_config(config[CONF_CHARGING_CURRENT]))

    if CONF_BOOST_MODE in config:
        cg.add(var.set_boost_mode_config(config[CONF_BOOST_MODE]))
    if CONF_CHARGER_MODE in config:
        cg.add(var.set_charger_mode_config(config[CONF_CHARGER_MODE]))
    if CONF_POWER_ON_LOAD in config:
        cg.add(var.set_power_on_load_config(config[CONF_POWER_ON_LOAD]))
    if CONF_BOOST_ALWAYS_ON in config:
        cg.add(var.set_boost_always_on_config(config[CONF_BOOST_ALWAYS_ON]))
    if CONF_BUTTON_SHUTDOWN_ENABLE in config:
        cg.add(var.set_button_shutdown_config(config[CONF_BUTTON_SHUTDOWN_ENABLE]))

    if CONF_BOOST_SHUTDOWN_LONG_PRESS in config:
        cg.add(var.set_boost_shutdown_long_press_config(config[CONF_BOOST_SHUTDOWN_LONG_PRESS]))
    if CONF_LED_DOUBLE_PRESS in config:
        cg.add(var.set_led_double_press_config(config[CONF_LED_DOUBLE_PRESS]))
    if CONF_SHORT_PRESS_BOOST in config:
        cg.add(var.set_short_press_boost_config(config[CONF_SHORT_PRESS_BOOST]))
    if CONF_BOOST_AFTER_VIN in config:
        cg.add(var.set_boost_after_vin_config(config[CONF_BOOST_AFTER_VIN]))
    if CONF_LOW_BATTERY_SHUTDOWN in config:
        cg.add(var.set_low_battery_shutdown_config(config[CONF_LOW_BATTERY_SHUTDOWN]))

    if CONF_LONG_PRESS_TIME_3S in config:
        cg.add(var.set_long_press_time_config(config[CONF_LONG_PRESS_TIME_3S]))
    if CONF_LIGHT_LOAD_SHUTDOWN_TIME in config:
        cg.add(var.set_light_load_shutdown_time_config(config[CONF_LIGHT_LOAD_SHUTDOWN_TIME]))

    if CONF_CHARGING_STOP_VOLTAGE in config:
        cg.add(var.set_charging_stop_voltage_config(config[CONF_CHARGING_STOP_VOLTAGE]))
    if CONF_END_CHARGE_CURRENT in config:
        cg.add(var.set_end_charge_current_config(config[CONF_END_CHARGE_CURRENT]))
    if CONF_CHARGER_UNDER_VOLTAGE in config:
        cg.add(var.set_charger_under_voltage_config(config[CONF_CHARGER_UNDER_VOLTAGE]))

    if CONF_BATTERY_VOLTAGE in config:
        cg.add(var.set_battery_voltage_config(config[CONF_BATTERY_VOLTAGE]))
    if CONF_VOLTAGE_PRESSURE in config:
        cg.add(var.set_voltage_pressure_config(config[CONF_VOLTAGE_PRESSURE]))

    if CONF_CC_LOOP_VIN in config:
        cg.add(var.set_cc_loop_vin_config(config[CONF_CC_LOOP_VIN]))
        cg.add(var.set_voltage_pressure_config(config[CONF_VOLTAGE_PRESSURE]))

    if CONF_CC_LOOP_VIN in config:
        cg.add(var.set_cc_loop_vin_config(config[CONF_CC_LOOP_VIN]))
