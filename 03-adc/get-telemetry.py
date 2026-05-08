import time
import serial
import matplotlib.pyplot as plt


def read_value(ser):
    """Чтение двух float значений из последовательного порта (напряжение и температура в одной строке)"""
    while True:
        try:
            line = ser.readline().decode("ascii")
            v, t = map(float, line.split())
            return v, t
        except ValueError:
            continue


def main():
    ser = serial.Serial(port="COM6", baudrate=115200, timeout=0.5)

    if ser.is_open:
        print(f"Port {ser.name} opened")
    else:
        print(f"Port {ser.name} closed")
        return

    measure_temperature_C = []
    measure_voltage_V = []
    measure_ts = []

    start_ts = time.time()

    # Пункт 3: Отправляем команду старта телеметрии
    ser.write("tm_start\n".encode("ascii"))
    print("Telemetry started")

    try:
        while True:
            ts = time.time() - start_ts

            # Пункт 4: Читаем напряжение и температуру одной строкой из телеметрии
            voltage_V, temp_C = read_value(ser)

            measure_ts.append(ts)
            measure_voltage_V.append(voltage_V)
            measure_temperature_C.append(temp_C)

            print(f"{voltage_V:.3f} V - {temp_C:.1f}°C - {ts:.2f}s")

            time.sleep(0.1)

    finally:
        # Пункт 5: Отправляем команду остановки телеметрии
        ser.write("tm_stop\n".encode("ascii"))
        print("Telemetry stopped")

        ser.close()
        print("Port closed")

        plt.subplot(2, 1, 1)
        plt.plot(measure_ts, measure_voltage_V)
        plt.title("График зависимости напряжения от времени")
        plt.xlabel("время, с")
        plt.ylabel("напряжение, В")

        plt.subplot(2, 1, 2)
        plt.plot(measure_ts, measure_temperature_C)
        plt.title("График зависимости температуры от времени")
        plt.xlabel("время, с")
        plt.ylabel("температура, C")

        plt.tight_layout()
        plt.show()


if __name__ == "__main__":
    main()
