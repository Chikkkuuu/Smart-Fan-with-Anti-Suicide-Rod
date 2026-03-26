 # Anti-Suicide Smart Fan Architecture: IoT-Integrated Safety Ecosystem
The **Anti-Suicide Smart Fan Architecture** is a specialized life-safety solution engineered to transform standard ceiling fans into intelligent, responsive devices. By combining real-time vertical displacement sensing with a cloud-synchronized emergency protocol, the system provides an immediate mechanical and digital intervention during high-risk events.

## Detailed System Architecture
The architecture is a hybrid of real-time embedded control and asynchronous cloud monitoring, ensuring that safety-critical interrupts are prioritized while maintaining remote manageability.
### 1. Hardware Sensing & Actuation Layer
* **Ultrasonic Proximity Monitoring:** The system uses an **HC-SR04** sensor to establish a 1D spatial baseline from the fan to the ceiling. 
* **Vertical Displacement Interrupt:** The firmware continuously compares the current distance against a calibrated baseline. If the distance increases by **>5.0 cm**, it indicates a mechanical decoupling event (suicide attempt), triggering the **Panic Mode** interrupt.
* **Precision Stepper Drive:** Fan rotation is handled by a **NEMA 17** stepper motor driven by an **A4988/DRV8825** driver. This allows for exact speed control and an instantaneous "Hard Kill" of rotation during an emergency.
* **Auditory & Visual Indicators:** A high-intensity LED and an active buzzer provide immediate localized feedback during a lockout or panic state.
### 2. State Management & Control Flow
* **Calibration State:** On boot, if the system is not in a "Locked" cloud state, it calibrates the `baselineDistance` to adapt to the specific room environment.
* **Normal Operational Loop:** The ESP32-C3 polls Firebase for user-defined speed settings (0–5) and maps them to hardware timer intervals for smooth motor pulsing.
* **Locked/Recovery State:** Once panic mode is triggered, the system enters a blocking loop. It will only resume operation if an authorized administrator updates the Firebase status to "Normal," at which point the system recalibrates and unlocks.
### 3. Cloud Synchronization Layer
* **RESTful Data Exchange:** The system uses **HTTP/SSL** to communicate with a **Firebase Real-time Database**.
* **Telemetry & SOS:** During a panic event, the `Suicide` flag is pushed to `true`, and the `Status` is set to `"Locked"`, facilitating remote alerting of authorities or medical teams.

## Hardware Wiring Specification
The system is built on the **ESP32-C3** platform. Below are the pin assignments and wiring connections as defined in the configuration:
| Component | Pin (ESP32-C3) | Connection Type | Description |
| :--- | :--- | :--- | :--- |
| **HC-SR04 (TRIG)** | GPIO 4 | Digital Output | Triggers ultrasonic pulse |
| **HC-SR04 (ECHO)** | GPIO 5 | Digital Input | Receives pulse echo |
| **Red LED** | GPIO 6 | Digital Output | Safety/Panic indicator |
| **A4988 (DIR)** | GPIO 7 | Digital Output | Stepper direction control |
| **A4988 (STEP)** | GPIO 10 | Digital Output | Pulse-width speed control |

## Repository Structure
The codebase is organized to separate hardware abstraction, global state variables, and cloud-management logic.
```text
Anti_Suicide_Fan/
├── Firmware/                 # Embedded C++ (Arduino/ESP32)
│   ├── sketch.ino            # Main state machine, calibration, and safety loops
│   ├── Sensor.cpp / .h       # Proximity sensing logic and hardware setup
│   ├── CloudManager.cpp / .h # Firebase REST API and WiFi orchestration
│   ├── Config.h              # Credential management and pin definitions
│   └── Globals.h             # Shared volatile system states
├── Web App/                  # Management Dashboard (HTML/JS)
│   ├── index.html            # Main status and monitoring view
│   └── control.html          # Remote speed and status override
├── vercel.json               # Web dashboard deployment configuration
└── LICENSE                   # MIT Open Source License
```

## Professional Profile
* **Developer**: Ritul Raj Bhakat (Firmware Developer)
* **Core Expertise**: Embedded Systems, IoT Architecture, and Rapid Prototyping
* **Contact**: [ritulraj384@gmail.com](mailto:ritulraj384@gmail.com)
* **Links**: [LinkedIn](https://www.linkedin.com/in/ritul-raj-bhakat-521202277/) | [GitHub Portfolio](https://github.com/Chikkkuuu)
