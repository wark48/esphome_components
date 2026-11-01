import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import CONF_ID
from . import cg_charger_ip5306_ns, CGChargerIP5306Component

DEPENDENCIES = ['cg_charger_ip5306']

CONF_CG_CHARGER_IP5306_ID = 'cg_charger_ip5306_id'
CONF_SENSOR_TYPE = 'sensor_type'

CGChargerIP5306BinarySensor = cg_charger_ip5306_ns.class_(
    'CGChargerIP5306BinarySensor', binary_sensor.BinarySensor, cg.PollingComponent
)

SENSOR_TYPES = {
    'charging': 'Charging Status',
    'charge_complete': 'Charge Complete',
    'light_load': 'Light Load',
    'short_press': 'Short Press',
    'long_press': 'Long Press',
    'double_press': 'Double Press',
}

CONFIG_SCHEMA = binary_sensor.binary_sensor_schema(CGChargerIP5306BinarySensor).extend({
    cv.GenerateID(CONF_CG_CHARGER_IP5306_ID): cv.use_id(CGChargerIP5306Component),
    cv.Required(CONF_SENSOR_TYPE): cv.one_of(*SENSOR_TYPES, lower=True),
}).extend(cv.polling_component_schema('60s'))

async def to_code(config):
    var = await binary_sensor.new_binary_sensor(config)
    await cg.register_component(var, config)
    parent = await cg.get_variable(config[CONF_CG_CHARGER_IP5306_ID])
    cg.add(var.set_parent(parent))
    cg.add(var.set_sensor_type(config[CONF_SENSOR_TYPE]))
