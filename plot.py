import sys
import os
import pandas as pd
import matplotlib.pyplot as plt

def main():
    # Determine CSV file path
    csv_file = sys.argv[1] if len(sys.argv) > 1 else "output.csv"

    if not os.path.exists(csv_file):
        print(f"[plot.py Error]: File '{csv_file}' does not exist.")
        sys.exit(1)

    # Load simulation data
    data = pd.read_csv(csv_file)

    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8), sharex=True)

    # Subplot 1: Motor Speed Response
    ax1.plot(data['time'], data['targetSpeed'], label='Target Speed (rad/s)', linestyle='--', color='red', linewidth=1.5)
    ax1.plot(data['time'], data['trueSpeed'], label='True Speed (rad/s)', color='blue', linewidth=1.5)
    if 'measuredSpeed' in data.columns and (data['measuredSpeed'] != data['trueSpeed']).any():
        ax1.plot(data['time'], data['measuredSpeed'], label='Measured Speed (Sensor)', color='gray', alpha=0.5, linewidth=0.8)

    ax1.set_ylabel('Speed (rad/s)')
    ax1.set_title('DC Motor PID Simulation Response')
    ax1.grid(True, linestyle=':', alpha=0.6)
    ax1.legend(loc='lower right')

    # Subplot 2: Control Voltage and Error
    ax2.plot(data['time'], data['controlVoltage'], label='Control Voltage (V)', color='green', linewidth=1.2)
    ax2.plot(data['time'], data['error'], label='Error (rad/s)', color='orange', linestyle=':', linewidth=1.2)

    ax2.set_xlabel('Time (seconds)')
    ax2.set_ylabel('Voltage (V) / Error')
    ax2.set_title('Controller Effort and System Error')
    ax2.grid(True, linestyle=':', alpha=0.6)
    ax2.legend(loc='upper right')

    plt.tight_layout()

    output_image = "simulation_plot.png"
    plt.savefig(output_image, dpi=300)
    print(f"[plot.py]: Plot successfully saved to '{output_image}'")

if __name__ == "__main__":
    main()
