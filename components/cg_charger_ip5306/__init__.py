import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import CONF_ID

DEPENDENCIES = ['i2c']
MULTI_CONF = False

CONF_CHARGING_CURRENT = 'charging_current'

cg_charger_ip5306_ns = cg.esphome_ns.namespace('cg_charger_ip5306')
CGChargerIP5306Component = cg_charger_ip5306_ns.class_(
    'CGChargerIP5306Component', cg.Component, i2c.I2CDevice
)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(CGChargerIP5306Component),
    cv.Optional(CONF_CHARGING_CURRENT, default=0.6): cv.float_range(min=0.05, max=2.1),
}).extend(cv.COMPONENT_SCHEMA).extend(i2c.i2c_device_schema(0x75))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
    cg.add(var.set_charging_current_config(config[CONF_CHARGING_CURRENT]))
