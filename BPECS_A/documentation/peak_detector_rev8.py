import numpy as np
import matplotlib.pyplot as plt
import pandas as pd


class Peak:
    NEG = -1
    NONE = 0
    POS = 1

class Status:
    INVALID = 0
    VALID = 1


class GradDetectorNP:
    def __init__(self, NSAMP, KSAMP, neg_thres, pos_thres):
        self.NSAMP = NSAMP
        self.KSAMP = KSAMP
        self.BUF = NSAMP + KSAMP

        self.data_arr = np.zeros(self.BUF, dtype=np.float64)
        self.index = 0

        self.curr_sum = 0
        self.last_sum = 0
        self.delta = 0

        self.neg_thres = neg_thres
        self.pos_thres = pos_thres

        self.state = Status.INVALID
        self.p = Peak.NONE

    def task(self, value):
        # Roll-over
        if self.index >= self.BUF:
            self.index = 0
            self.state = Status.VALID

        # Write new sample
        self.data_arr[self.index] = value

        # ---- Current window ----
        start = self.index - self.NSAMP + 1
        if start < 0:
            start += self.BUF

        idxs_curr = (start + np.arange(self.NSAMP)) % self.BUF
        curr_data = self.data_arr[idxs_curr]
        self.curr_sum = curr_data.sum()

        # ---- Last window ----
        start = self.index - self.NSAMP - self.KSAMP + 1
        if start < 0:
            start += self.BUF

        idxs_last = (start + np.arange(self.NSAMP)) % self.BUF
        last_data = self.data_arr[idxs_last]
        self.last_sum = last_data.sum()

        # ---- Delta ----
        self.delta = self.curr_sum - self.last_sum

        # ---- Peak classification ----
        if self.delta <= self.neg_thres:
            self.p = Peak.NEG
        elif self.delta >= self.pos_thres:
            self.p = Peak.POS
        else:
            self.p = Peak.NONE

        # Advance write index
        self.index += 1

        return curr_data, last_data, self.delta, self.p


def visualize_detector(detector, t, raw_data):
    t_list = []
    curr_sum_list = []
    last_sum_list = []
    delta_list = []
    peak_list = []
    state_list = []
    raw_list = []

    # Run detector over all samples
    for x in raw_data:
        curr, last, delta, p = detector.task(x)
        curr_sum_list.append(detector.curr_sum)
        last_sum_list.append(detector.last_sum)
        delta_list.append(delta)
        peak_list.append(p)
        state_list.append(detector.state)
        raw_list.append(x)

    # Convert to arrays
    curr_sum_list = np.array(curr_sum_list)
    last_sum_list = np.array(last_sum_list)
    delta_list = np.array(delta_list)
    peak_list = np.array(peak_list)
    state_list = np.array(state_list)
    raw_list = np.array(raw_list)
    t_list = np.array(t)

    # ---------------------------------------------------------
    # Filter out INVALID samples
    # ---------------------------------------------------------
    valid_mask = (state_list == Status.VALID)

    if not valid_mask.any():
        print("No valid samples available yet.")
        return

    t_valid = t_list[valid_mask]
    raw_valid = raw_list[valid_mask]
    curr_valid = curr_sum_list[valid_mask]
    last_valid = last_sum_list[valid_mask]
    delta_valid = delta_list[valid_mask]
    peak_valid = peak_list[valid_mask]

    # ---------------------------------------------------------
    # Plotting
    # ---------------------------------------------------------
    fig, axs = plt.subplots(4, 1, figsize=(14, 12), sharex=True)

    # Raw data (VALID only)
    axs[0].plot(t_valid, raw_valid, color="black")
    axs[0].set_title("Raw Input Signal (VALID samples only)")
    axs[0].grid(True)

    # Current sum
    axs[1].plot(t_valid, curr_valid, label="Current Sum", color="blue")
    axs[1].set_title("Current Window Sum (curr_sum)")
    axs[1].grid(True)

    # Last sum
    axs[2].plot(t_valid, last_valid, label="Last Sum", color="orange")
    axs[2].set_title("Last Window Sum (last_sum)")
    axs[2].grid(True)

    # Delta + thresholds + peak output
    axs[3].plot(t_valid, delta_valid, label="Delta", color="purple")
    axs[3].axhline(detector.pos_thres, color="green", linestyle="--", label="pos_thres")
    axs[3].axhline(detector.neg_thres, color="red", linestyle="--", label="neg_thres")

    # Peak markers
    axs[3].scatter(
        t_valid[np.where(peak_valid == Peak.POS)],
        delta_valid[peak_valid == Peak.POS],
        color="green", label="POS"
    )
    axs[3].scatter(
        t_valid[np.where(peak_valid == Peak.NEG)],
        delta_valid[peak_valid == Peak.NEG],
        color="red", label="NEG"
    )

    axs[3].set_title("Delta, Thresholds, and Peak Output (VALID samples only)")
    axs[3].grid(True)
    axs[3].legend()

    plt.tight_layout()
    plt.show()

det = GradDetectorNP(NSAMP=16, KSAMP=4, neg_thres=-20, pos_thres=20)

# ---------------------------------------------------------
# Generate test signal
# ---------------------------------------------------------

file_name = "20260211_PIC12F1572_EXT_16MHZ_4_PRE_T1_6CM_TO_1CM"
file_ending = ".csv"
file_path = "C:/""


file_complete = file_path+file_name+file_ending
df = pd.read_csv(file_complete, sep=';', header=0)

df['Start Time'] = df['Start Time'].str.replace(',', '.').astype(float)
df['End Time'] = df['End Time'].str.replace(',', '.').astype(float)

counts_ = df["Data"]
time_ = df["Start Time"]-min(df["Start Time"])

signal = counts_.to_numpy()
time_arr = time_.to_numpy()

visualize_detector(det, time_arr, signal)
