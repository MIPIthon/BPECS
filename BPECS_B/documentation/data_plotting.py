import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

# ---------------------------------------------------------
# Load data (PICOSCOPE-UART-DECODER)
# ---------------------------------------------------------

file_name = "20260406_BPECS_B_EXT_16MHZ_T3_6CM_TO_1CM"
file_ending = ".csv"
file_path = "C:/"


file_complete = file_path+file_name+file_ending
df = pd.read_csv(file_complete, sep=';', header=0)

df['Start Time'] = df['Start Time'].str.replace(',', '.').astype(float)
df['End Time'] = df['End Time'].str.replace(',', '.').astype(float)

counts_ = df["Data"]
time_ = df["Start Time"]-min(df["Start Time"])

signal = counts_.to_numpy()
time_arr = time_.to_numpy()


# ---------------------------------------------------------
# Plot data
# ---------------------------------------------------------
fig, ax = plt.subplots()

# Raw data
ax.plot(time_arr, signal, color="black")
ax.set_title(file_name)
ax.grid(True)
ax.set_xlabel("Time [s]")
ax.set_ylabel("Raw signal [-]")
ax.set_xlim(0, max(time_arr))
ax.set_ylim(min(signal), max(signal))

plt.tight_layout()
plt.show()
