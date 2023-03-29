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

def inter_SOS (SOS_list: list) -> list:
    bool_SOS = []
    for element in SOS_list:
        if (element == True):
            bool_SOS.append(1)
        else:
            bool_SOS.append(0)

    return bool_SOS

def timescale_SOS(SOS_list) -> list:
    """
    generate timescale for SOS plot
    """
    length = len(SOS_list)
    scale = np.linspace(0,length-1, length)
    return scale

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
    fig1 = plt.figure("Figure 1")
    plt.plot(humid_list)
    plt.ylabel(y_label)
    plt.xlabel(x_label)
    plt.show()
    return

def plot_pressure (pressure_list):
    """
    y axis auto scaling
    """
    y_label = "Pressure (Pa)"
    x_label = "Time (s)"
    fig2 = plt.figure("Figure 2")
    plt.plot(pressure_list)
    plt.ylabel(y_label)
    plt.xlabel(x_label)
    plt.show()

    return

def plot_CO2 (CO2_list):
    """
    y axis auto scale
    """

    return

def plot_all (SOS_list, temp_list, humid_list, pressure_list, CO2_list):
    """
    to all plots on one subplot
    """
    fig_temp = plt.figure("SOS Status")
    SOS_y_label = "SOS (On / Off)"
    SOS_x_label = "Time (s)"
    SOS_scale = timescale_SOS(SOS_list)
    plt.step(SOS_scale, SOS_list, color="red")
    plt.ylabel(SOS_y_label)
    plt.xlabel(SOS_x_label)

    fig_temp = plt.figure("Temperature")
    temp_y_label = "Temperature (C)"
    temp_x_label = "Time (s)"
    plt.plot(temp_list)
    plt.ylabel(temp_y_label)
    plt.xlabel(temp_x_label)

    fig_humid = plt.figure("Humidity")
    humid_y_label = "Humidity (%)"
    humid_x_label = "Time (s)"
    plt.plot(humid_list)
    plt.ylabel(humid_y_label)
    plt.xlabel(humid_x_label)

    fig_pressure = plt.figure("Pressure")
    pressure_y_label = "Pressure (Pa)"
    pressure_x_label = "Time (s)"
    plt.plot(pressure_list)
    plt.ylabel(pressure_y_label)
    plt.xlabel(pressure_x_label)

    fig_temp = plt.figure("CO2")
    CO2_y_label = "CO2 Concentration (ppm)"
    CO2_x_label = "Time (s)"
    plt.plot(CO2_list)
    plt.ylabel(CO2_y_label)
    plt.xlabel(CO2_x_label)

    plt.show()
    return

def auto_plot_all (): 
    """
    plots all graph atributes
    """

    SOS_data = data.get("SOS")
    SOS_data.pop(0)
    SOS_inter = inter_SOS(SOS_data)

    temp_data = data.get("temp")
    temp_data.pop(0)

    humid_data = data.get("humid")
    humid_data.pop(0)

    pressure_data = data.get("pressure")
    pressure_data.pop(0)

    CO2_data = data.get("CO2")
    CO2_data.pop(0)

    plot_all(SOS_inter, temp_data, humid_data, pressure_data, CO2_data)
    return

auto_plot_all()

#trim_data = data.get("humid")
#test = trim_data.pop(0)
#plot_pressure(trim_data)


