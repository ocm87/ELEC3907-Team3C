import matplotlib.pyplot as plt
import numpy as np
from readSD import readSD

SD_FILE_PATH = "/Users/owenmacintosh/Documents/ELEC3907-Team3C/localGraphing/exFile.txt"

# get dictionary of data from file
data = readSD(SD_FILE_PATH)
print(data)

#TODO define possible plots: SOS (1/0), temp (C), humid (%), pressure (Pa)

#! all time intervals are approx. 1s since that is the rate it's stored on SD

#* Can I do auto y-asix scaling??? have it ignore first

def generate_timescale (input_list: list):
    length = len(input_list)
    timescale = np.linspace(0.0, length-1, length)
    
    return timescale

def plot_SOS (SOS_list):
    """
    y axis scale 0-1 FIXED
    """
    y_label = "ON/OFF"
    x_label = "Time (s)"
    return

def plot_temp (temp_list):
    """
    y axis scale -40-100 AUTO SCALE IF POSSIBLE
    """
    y_label = "Temperature (C)"
    x_label = "Time (s)"

    return

def plot_humid (humid_list: list):
    """
    y-axis scale 0-100 FIXED
    """
    y_label = "Humidity (%)"
    x_label = "Time (s)"
    return

def plot_pressure (pressure_list):
    """
    y axis scale 50000-300000 - AUTO SCALE IF POSSIBLE MAYBE
    """
    y_label = "Pressure (Pa)"
    x_label = "Time (s)"

    plt.plot(pressure_list)
    plt.ylabel(y_label)
    plt.xlabel(x_label)
    plt.show()

    return
trim_data = data.get("pressure")
test = trim_data.pop(0)
plot_pressure(trim_data)

