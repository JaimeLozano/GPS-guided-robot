import io
import time
import pynmea2
import serial
import utm
import math

ser = serial.Serial('/dev/ttyS0', 9600, timeout=1.0)
sio = io.TextIOWrapper(io.BufferedRWPair(ser, ser))

ser2 = serial.Serial('/dev/ttyACM0', 9600, timeout=1.0)

while 1:
    try:
        line = sio.readline()
        msg = pynmea2.parse(line)
        if (msg.sentence_type == 'RMC'):
                print(msg.timestamp)
                input_lat = msg.latitude
                input_lon = msg.longitude
                x, y, zone_number, zone_letter  = utm.from_latlon(input_lat,input_lon)
                # Calcular error
                input_lat0 = 37.18817 # Granada
                input_lon0 = -3.60667 # Granada
#                input_lat0 = 48.85341 # Paris
#                input_lon0 = 2.3488   # Paris
#                input_lat0 = 40.4165 # Madrid
#                input_lon0 = -3.70256 # Madrid
                x0, y0, zone_number0, zone_letter0 = utm.from_latlon(input_lat0,input_lon0)
                xerror = x0 - x
                yerror = y0 - y
                
                distance = math.sqrt(xerror*xerror + yerror*yerror)
                print(str('Distance: ') + str(int(distance)))

                angulo_a_objetivo = math.atan2(xerror,yerror)*180/3.1415
                print(str('Angle: ') + str(int(angulo_a_objetivo)))

                # Pasar angulo_a_objetivo
                command = str(angulo_a_objetivo) + str('\n')
                ser2.write(bytes(command, encoding='utf8'))

    except serial.SerialException as e:
        print('Device error: {}'.format(e))
        break
    except pynmea2.ParseError as e:
        print('Parse error: {}'.format(e))
        continue
    except AttributeError as e:
        print('Atrribute error: {}'.format(e))
        continue
