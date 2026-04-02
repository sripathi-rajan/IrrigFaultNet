IrrigFaultNet 🌱
Cumulative Irrigation Fault Detection & Field Irrigation Health Scoring
A minimal low-cost sensor system for detecting chronic irrigation mismanagement in smallholder agriculture — 10-14 days before visible crop stress.

🚀 Overview
IrrigFaultNet is a hardware-software co-designed system that:

Detects and classifies irrigation faults in real-time (5 fault types)
Accumulates faults over time using a 4-state finite-state machine
Computes a Field Irrigation Health Score (FIHS) — a 0-100 scalar metric
Alerts farmers in plain language before irreversible crop damage
Designed specifically for manual and semi-manual irrigation used by 80%+ of smallholder farmers globally.

🔧 Hardware (Minimal — < ₹4,500/acre)
Component	Spec	Cost
Capacitive Soil Moisture Sensor x3	±3% accuracy, 10/30/60 cm depths	~₹400 each
DHT22 Temperature/Humidity Sensor	±0.5°C, ±2% RH	~₹150
ESP32 / Arduino UNO	Edge processing, deep-sleep	~₹400
Solar + Li-ion Battery	5W panel + 3000mAh	~₹800
No irrigation control hardware (valves/pumps) required.

📁 Repository Structure

IrrigFaultNet/
├── sketch.ino              # Arduino/ESP32 firmware — sensor reading + fault classification
├── diagram.json            # Wokwi circuit diagram
├── DesignThinking.ipynb    # Full ML pipeline — FIHS computation, RF classifier, dashboard
└── README.md               # This file
⚙️ How It Works

Soil Sensors (3 depths)
        ↓
  Edge Node (ESP32)
  - 5-point median filter
  - 15-min interval logging
        ↓
  Event Segmentation
  - PELT changepoint detection
  - ET0-adjusted reference target
        ↓
  Fault Classifier (5 classes)
  - UNDER_IRRIGATION  (Wi = 1.4)
  - OVER_IRRIGATION   (Wi = 1.2)
  - DISTRIBUTION_FAULT(Wi = 1.1)
  - TIMING_FAULT      (Wi = 1.0)
  - NORMAL            (Wi = 0.0)
        ↓
  Accumulation Engine
  - 4-state FSM: Normal → Isolated → Recurring → Chronic
  - Exponential temporal decay: exp(-λ × Δt), λ = 0.05/day
        ↓
  FIHS Computation
  FIHS(t) = 100 × (1 - Σ(Si × Wi × e^(-λΔti)) / N_max)
        ↓
  Farmer Alert
  🔴 CHRONIC if FIHS < 70
📊 Results
Metric	Value
ML Classifier Accuracy	100% (all 5 classes)
Fault Detection Lead Time	10-14 days before visible stress
Hardware Cost	< ₹4,500/acre
Power Consumption	< 50 mW average
Days in Danger Zone (simulation)	9 / 60
Minimum FIHS recorded	52.2 / 100
🧪 Running the Simulation
Wokwi (Hardware Simulation)
Go to wokwi.com → New Project → Arduino UNO
Paste diagram.json and sketch.ino
Hit ▶ — Serial Monitor opens automatically
Twist potentiometers to simulate different moisture conditions
Google Colab (ML Pipeline)
Open DesignThinking.ipynb in Google Colab
Run all cells in order
Outputs: FIHS score, fault classification, dashboard PNG, CSV exports
🔬 Fault Classification Rules
Fault	Condition	Severity Weight
UNDER_IRRIGATION	avg(M) < Target × 0.6	1.4
OVER_IRRIGATION	avg(M) > Field_Cap × 0.95	1.2
TIMING_FAULT	M10 > Target×0.85 AND avg < Target×0.5	1.0
DISTRIBUTION_FAULT	M10 > M60 × 2.0	1.1
NORMAL	None of the above	0.0
📄 Patent Status
Provisional Patent Application — Indian Patent Office

Title: A System and Method for Cumulative Irrigation Fault Detection, Fault Accumulation, and Field Irrigation Health Scoring Using Minimal Low-Cost Sensor Hardware
Applicant: Thiagarajar College of Engineering, Madurai
Inventor: Sripathi Rajan
Status: In preparation for filing (Form 2 Complete Specification ready)
🎯 Target Applications
Smallholder paddy, cotton, and millet farms in Tamil Nadu
Semi-arid irrigation management (Madurai / Virudhunagar districts)
Research: IEEE ICIOT, ACM COMPASS, Computers and Electronics in Agriculture
👨‍💻 Author
Sripathi Rajan B.Tech Computer Science and Business Systems (CSBS) Thiagarajar College of Engineering, Madurai — 625 015

📜 License
This project is submitted for patent protection. All rights reserved.
© 2026 Thiagarajar College of Engineering
