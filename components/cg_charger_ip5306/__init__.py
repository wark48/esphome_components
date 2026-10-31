import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import CONF_ID

DEPENDENCIES = ['i2c']
MULTI_CONF = False

cg_charger_ip5306_ns = cg.esphome_ns.namespace('cg_charger_ip5306')
CGChargerIP5306Component = cg_charger_ip5306_ns.class_('CGChargerIP5306Component', cg.Component, i2c.I2CDevice)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(CGChargerIP5306Component),
}).extend(cv.COMPONENT_SCHEMA).extend(i2c.i2c_device_schema(0x75))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
