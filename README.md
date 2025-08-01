# Smart-Pet-Feeder
A smart IoT-based pet feeding system that lets you feed and hydrate your furry friend remotely using Google Assistant, with real-time monitoring and automated scheduling

## 📌 Project Overview

The **Automated Pet Feeder** is designed to make pet care easy and worry-free for busy pet parents. Using a NodeMCU (ESP8266) microcontroller, it allows users to control feeding times, portion sizes, and water levels — all through a mobile app or voice commands via Google Assistant.

Real-time camera monitoring and sensors ensure your pet is fed and hydrated, while alerts notify you if anything goes wrong.

---

## 🧠 Core Features

- **Remote Feeding & Hydration** via Google Assistant
- **Scheduled Feeding** using time-based triggers
- **Live Monitoring** with ESP32-CAM to check bowl status
- **Ultrasonic Sensor** to measure food level
- **Soil Moisture Sensor** to check water availability
- **Telegram Alerts** for low food or water levels
- **LCD Display** showing current time & feed schedule
- **Servo Motor** for precise food dispensing
- **Water Pump** for auto-refill of water bowl

---

## 🛠️ Tech Stack

| Component           | Purpose                           |
|---------------------|-----------------------------------|
| NodeMCU (ESP8266)   | Main microcontroller              |
| ESP32-CAM           | Capturing pet bowl images         |
| Servo Motor         | Dispensing dry food               |
| Mini Water Pump     | Dispensing water                  |
| Ultrasonic Sensor   | Monitoring food level             |
| Soil Moisture Sensor| Monitoring water presence         |
| LCD Display         | Displaying time & system status   |
| Telegram Bot API    | Sending alerts & notifications    |
| Google Assistant    | Voice command integration         |

---

## 📱 User Interface

- **Google Assistant**: Use voice commands like  
  `"Hey Google, feed my dog"` or `"Refill water bowl"`
  
- **Mobile App/Web UI** *(Optional)*: Control feeding schedules and view ESP32-CAM images

---

## 🚨 Notifications

Low food or water level? The system sends real-time alerts to your **Telegram** with bowl status images and a refill reminder.

---

