# IOT sensor value parser
from math import pow
from terminaltables import SingleTable
import os

temperature = 0
humidity = 0
visible = 0
ir = 0
hp206c_temperature = 0
pressure = 0
height = 0


def parse(received_bytes):
    sensor = int(received_bytes[0])
    sensor_value_16 = int.from_bytes(received_bytes[1:3], byteorder='big')
    sensor_value_24 = int.from_bytes(received_bytes[1:4], byteorder='big')
    if sensor == 1:
        calc_humidity(sensor_value_16)
    elif sensor == 2:
        calc_temperature(sensor_value_16)
    elif sensor == 3:
        calc_visible(sensor_value_16)
    elif sensor == 4:
        calc_ir(sensor_value_16)
    elif sensor == 5:
        calc_hp206c_temperature(sensor_value_24)
    elif sensor == 6:
        calc_hp206c_pressure(sensor_value_24)
    elif sensor == 7:
        calc_hp206c_height(sensor_value_24)
    draw_table()


def calc_humidity(sensor_value):
    humidity_raw = (sensor_value / 16) - 24

    # linearization:
    a0 = -4.7844
    a1 = 0.4008
    a2 = -0.00393
    humidity_linear = humidity_raw - (pow(humidity_raw, 2) * a2 + humidity_raw * a1 + a0)

    # temperature compensation
    q0 = 0.1973
    q1 = 0.00237
    humidity_compensated = humidity_linear + ((temperature - 30)) * (humidity_linear * q1 + q0)
    global humidity
    humidity = humidity_compensated


def calc_temperature(sensor_value):
    global temperature
    temperature = (sensor_value / 32) - 50


def calc_visible(sensor_value):
    global visible
    visible = sensor_value
    visible /= 0.282


def calc_ir(sensor_value):
    global ir
    ir = sensor_value
    ir /= 2.44


def calc_hp206c_temperature(sensor_value):
    global hp206c_temperature
    hp206c_temperature = sensor_value / 100


def calc_hp206c_pressure(sensor_value):
    global pressure
    pressure = sensor_value / 100


def calc_hp206c_height(sensor_value):
    global height
    height = sensor_value / 100


def draw_table():
    a = str(round(temperature, 2))
    b = str(round(humidity, 2))
    c = str(round(visible))
    d = str(round(ir))
    e = str(round(hp206c_temperature, 2))
    f = str(round(pressure))
    g = str(round(height, 2))

    TABLE_DATA = (
        ('Sensor', 'Wert', 'Einheit'),
        ('Temperatur', a, '°C'),
        ('Feuchtigkeit', b, '%'),
        ('Sichtbares Licht', c, 'lx'),
        ('Infrarot', d, 'lx'),
        ('Temperatur hp206c', e, '°C'),
        ('Luftdruck', f, 'mbar'),
        ('Höhe', g, 'm'),
    )

    table_instance = SingleTable(TABLE_DATA, "IoT Sensors")
    table_instance.justify_columns[3] = 'right'
    os.system('cls' if os.name == 'nt' else 'clear')
    print(table_instance.table, end='\r')

    #os.system('clear')

