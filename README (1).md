# GPS Tracking System

A real-time GPS tracker built on an ESP32 that reads location from a NEO-6M GPS module and sends it as an SMS via a SIM800L GSM module. Self-initiated project.

## Features

- **Live GPS tracking:** reads latitude, longitude, and satellite count continuously using TinyGPS++ over hardware serial.
- **SMS location updates:** sends the current location, along with a ready-to-open Google Maps link, to a phone number every 60 seconds whenever the GPS location updates.
- **SIM800L initialization over AT commands:** checks SIM status, signal quality, and network registration before sending messages.
- **Status LEDs:** one LED flashes on each GPS update, another flashes when an SMS is sent.

## Hardware

| Component | Role |
|---|---|
| ESP32 dev board | Main controller |
| NEO-6M GPS module | Location data (serial) |
| SIM800L GSM module | Sends location via SMS |
| 2x LEDs | GPS update / SMS sent status indicators |

## Pin connections (from the code)

| Signal | ESP32 Pin |
|---|---|
| GPS RX / TX | GPIO16 / GPIO17 |
| SIM800L RX / TX | GPIO26 / GPIO27 |
| GPS status LED | GPIO19 |
| GSM/SMS status LED | GPIO18 |

## How it works

1. The ESP32 continuously reads GPS data over serial and decodes it with TinyGPS++.
2. On startup, it initializes the SIM800L with AT commands (checks SIM, signal, network registration, sets SMS text mode).
3. Whenever the GPS location updates, it prints the coordinates and satellite count to the Serial Monitor and flashes the GPS LED.
4. Once every 60 seconds (or on the first fix), it sends an SMS containing the latitude, longitude, and a Google Maps link, then flashes the GSM LED.

## Applications

- Vehicle tracking
- Child safety monitoring
- Asset tracking

## Setup

1. Wire the components as listed in the pin table above.
2. Insert an active SIM card into the SIM800L module and ensure it has a balance for SMS.
3. Replace `+91XXXXXXXXXX` in the code with the phone number that should receive location updates. Do not commit a real phone number to a public repository — use a placeholder or an environment-specific config file instead.
4. Upload the sketch to the ESP32 using the Arduino IDE with the TinyGPS++ library installed.
5. Power the device outdoors or near a window for the GPS to get a satellite fix.

## Repository contents

- `gps_tracking_system.ino`: main ESP32 sketch (GPS reading + SIM800L SMS sending)

## What I learned

- [Add 1-2 real points, e.g. working with AT commands to control a GSM module, handling two hardware serial ports at once on the ESP32, timing SMS sends to avoid excess cost/spam.]

## Possible improvements

- [Only list ideas you'd actually want to build, e.g. switching to a data-based tracking service instead of SMS, adding a low-battery alert, GPS+WiFi hybrid tracking.]

## Author

Nikhil Gaurav, B.Tech ECE, Andhra University
