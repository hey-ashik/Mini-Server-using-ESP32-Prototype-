# ESP32 Portfolio Web Server with Admin WiFi Portal

This project turns an ESP32 into a personal portfolio web server. It features a clean, responsive front-end and a hidden **Admin Panel** that allows you to update WiFi credentials over-the-air (OTA) without reflashing the code.



## 🚀 Features
* **Persistent Storage:** Uses `Preferences.h` to save WiFi credentials even after power loss.
* **mDNS Support:** Access the site via `http://ashik.local` instead of typing an IP address.
* **Responsive UI:** A modern, minimalist portfolio design.
* **Admin Panel:** Secure POST-request handling to update network settings.

## 🛠️ Hardware Requirements
* **ESP32** Development Board (DevKit V1 or similar).
* Micro-USB cable for flashing.
* An existing WiFi network.

## 📁 Project Structure
* `Portfolio_Server.ino`: The main Arduino sketch containing the C++ logic and HTML/CSS.

## ⚙️ Setup Instructions

### 1. Prerequisite Libraries
Ensure you have the following libraries installed in your Arduino IDE:
* `WiFi.h`
* `WebServer.h`
* `ESPmDNS.h`
* `Preferences.h` (Built-in for ESP32)

### 2. Configuration
By default, the code looks for these credentials if no settings are saved:
```cpp
String ssid = "Al Amin Islam Ar";
String password = "ALAMINASR60";

🖥️ Web InterfacePageRouteDescriptionPortfolio/Displays your EEE & AI Developer professional bio.Admin Panel/adminForm to input new SSID and Password.Action/saveSaves data to NVS and restarts the ESP32.👨‍💻 Developed ByAshikul Islam B.Sc. in EEE | AI & RAG Solutions
---
