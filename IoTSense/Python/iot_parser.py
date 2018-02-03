# IOT sensor value parser
from math import pow

temperature = 0


def parse(received_bytes):
    sensor = int(received_bytes[0])
    sensor_value = int.from_bytes(received_bytes[1:3], byteorder='big')
    if sensor == 1:
        calc_humidity(sensor_value)
    elif sensor == 2:
        calc_temperature(sensor_value)


def calc_humidity(sensor_value):
    humidity = (sensor_value / 16) - 24

    # linearization:
    a0 = -4.7844
    a1 = 0.4008
    a2 = -0.00393
    humidity_linear = humidity - (pow(humidity, 2) * a2 + humidity * a1 + a0)

    # temperature compensation
    q0 = 0.1973
    q1 = 0.00237
    humidity_compensated = humidity_linear + ((temperature - 30)) * (humidity_linear * q1 + q0)

    print("RH:\t\t\t", round(humidity_compensated, 2), "%")


def calc_temperature(sensor_value):
    global temperature
    temperature = (sensor_value / 32) - 50
    print("Temperatur:\t", round(temperature, 2), "°C")

