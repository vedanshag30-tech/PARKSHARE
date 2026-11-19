# ParkShare — P2P Smart Parking & Auto-Care Platform

A peer-to-peer smart parking reservation system combining a C++ backend booking engine with an interactive GPS map interface.

## Architecture Highlights
- **Backend Engine (`main.cpp` & `models.h`):** Pure C++17 reservation engine featuring live client broadcast notifications and temporal overlap validation to prevent double-booking race conditions.
- **Frontend (`index.html`):** Interactive Leaflet/OpenStreetMap discovery portal with Haversine GPS distance sorting, 25%/75% split payment breakdown, and 45-minute grace cancellation logic.

## Tech Stack
- **Backend:** C++17, Standard Template Library (`<vector>`, `<string>`, `<ctime>`, `<iostream>`)
- **Frontend:** HTML5, TailwindCSS, JavaScript (ES6+), Leaflet Maps

## Running Locally

### 1. Compile and Run the C++ Backend Engine
Open a terminal inside the project folder and run:
```cmd
g++ -std=c++17 main.cpp -o parkshare.exe
parkshare.exe