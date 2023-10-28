import serial
import time
import random
import re
import matplotlib.pyplot as plt
from matplotlib import style
import matplotlib.cm as cm
import numpy as np
from threading import Thread

# Change as needed
simulation = True; real = not simulation
logging = True
full_path = True
fixed_graph = False
sleep_time = 0.2

# Match arduino port & baudrate
port = "/dev/cu.usbmodem144201"
baudrate = 115200
timeout = 1
ser = None


lat_0 = 32.88013 # Temporary
lon_0 = -117.23314 # Tempory
r_earth = 6371000 # Radius of earth in meters


n = 10 # Number of points for vanishing path
x_data = []
y_data = []
intensity = np.linspace(0.4,1,n)

style.use("seaborn-whitegrid")
fig, ax = plt.subplots()
x_lim = 30; y_lim = 30  # Limits for fixed axis graph



if (real):
	# Serial port connection
	ser = serial.Serial(port, baudrate, timeout)

def simulate_reading():
	# Example GPS Reading:
	# 32.880132529690826, -117.23314437853034
	simulated_lat = 32.88013 + ((random.random() * 2) - 1) / 100000 
	simulated_long = -117.23314 + ((random.random() * 2) - 1) / 100000
	return "Location: " + str(simulated_lat) + "," + str(simulated_long)

# Parse data string from serial
def process_data(data):
	result = re.search(r"Location: ([-]*\d+.\d+),([-]*\d+.\d+)", data)
	latitude = result.group(1)
	longitude = result.group(2)
	return float(latitude), float(longitude)


def deg_to_m(angle):
	return angle / 360 * 2 * np.pi * r_earth

def latlon_to_meters(latitude, longitude):
	x = deg_to_m(latitude - lat_0)
	y = deg_to_m(longitude - lon_0)
	return x, y

def update_position_graph(x, y):
	if (not full_path and len(x_data) >= n):
		x_data.pop(0)
		y_data.pop(0)
	
	x_data.append(x)
	y_data.append(y)
	
	ax.clear()
	if (fixed_graph):
		ax.set(xlim=(-x_lim, x_lim), ylim=(-y_lim, y_lim))
	ax.set_title("GPS Position")
	ax.set_xlabel("x position (m)")
	ax.set_ylabel("y position (m)")
	
	if (full_path):
		colors = cm.plasma(np.linspace(0, 1, len(x_data)))
		ax.scatter(x_data, y_data, c=colors)
	else:
		scatter = ax.scatter(x_data, y_data)
		scatter.set_array(intensity)
	
	plt.pause(0.001)

def get_serial_line():
	return ser.readline().decode().strip()

last_received = ""

# Get the most recent measurements from serial port
# Runs on a seperate thread
def receive_serial(ser):
	global last_received

	buffer_string = ""

	while True:
		buffer_string = buffer_string + ser.read(ser.inWaiting()).decode()
		if "\n" in buffer_string:
			lines = buffer_string.split("\n")
			last_received = lines[-2]
			buffer_string = lines[-1]

def initialize_zero_reference():
	initialized = False
	while (not initialized):
		try:
			global lat_0
			global lon_0
			if (real):
				lat_0, lon_0 = process_data(last_received)
			initialized = True;
			print("Initialized lat/lon zero reference")
			break
		except:
			print(f"Could not initialize lat/lon from line: {data}")
		time.sleep(1)

def main():
	initialize_zero_reference()
	while True:
		data = ""
		if (simulation):
			data = simulate_reading()
		else:
			data = last_received
		
		try:
			latitude, longitude = process_data(data)
		except:
			print(f"Could not read lat/lon from line: {data}")
			continue
		
		x, y = latlon_to_meters(latitude, longitude)

		if (logging):
			print("Latitude: {:<20} Longitude: {:<20} x: {:8.2f} y: {:8.2f}"\
			.format(latitude, longitude, x, y), end="\r")
		
		update_position_graph(x, y)
		time.sleep(sleep_time)

if __name__ == "__main__":
	if (real):
		Thread(target=receive_serial, args=(ser,)).start()
	main()

