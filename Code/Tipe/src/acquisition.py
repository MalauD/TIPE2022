import serial
import matplotlib.pyplot as plt
import numpy as np

ser = serial.Serial('/dev/cu.usbserial-1420', 115200)

datas = np.zeros(5000)

data = ser.readline()

i = 0

while data:
    data = ser.readline()
    data = data.decode('utf-8')
    np.put(datas, i % 5000, int(data))
    if i % 100 == 0:
        plt.plot(datas)
        plt.draw()
        plt.pause(0.0001)
        plt.clf()
    i += 1