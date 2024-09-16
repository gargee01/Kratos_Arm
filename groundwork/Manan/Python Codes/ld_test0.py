import serial
import matplotlib.pyplot as plt

# Set up the serial connection (replace '/dev/ttyUSB0' with your ESP32's serial port)
ser = serial.Serial('/dev/ttyUSB0', 115200)  # Make sure to change this to your correct serial port

# Define the function to process the incoming data
def process_data(data):
    # Split the incoming data string into individual wavelength-absorbance pairs
    pairs = data.split(";")
    
    wavelengths = []
    absorbances = []
    
    # Split each pair into wavelength and absorbance values
    for pair in pairs:
        wavelength, absorbance = pair.split(",")
        wavelengths.append(float(wavelength))
        absorbances.append(float(absorbance))
    
    return wavelengths, absorbances

# Create a plot that will be updated in real-time
plt.ion()  # Interactive mode for live updates
fig, ax = plt.subplots()
line, = ax.plot([], [], 'o-')  # Initialize an empty line object
ax.set_xlabel('Wavelength (nm)')
ax.set_ylabel('Absorbance')
ax.set_title('Absorbance vs Wavelength')
ax.set_xlim(400, 950)  # The expected wavelength range from the sensor
ax.set_ylim(0, 2)  # Adjust limits based on expected absorbance values

# Continuously read and plot data from the serial port
while True:
    # Read a line of data from the serial port (assuming ESP32 is sending it as a string)
    line_data = ser.readline().decode('utf-8').strip()  # Read and clean the serial data

    # Process the incoming data into wavelength and absorbance arrays
    wavelengths, absorbances = process_data(line_data)

    # Update the plot with new data
    line.set_xdata(wavelengths)
    line.set_ydata(absorbances)
    
    ax.relim()  # Recompute the limits based on the new data
    ax.autoscale_view(True, True, True)  # Automatically scale the axes

    plt.draw()  # Redraw the plot
    plt.pause(0.1)  # Pause briefly to allow for the plot to update
