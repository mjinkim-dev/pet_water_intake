# Pet Water Intake Monitoring System

This project implements an IoT-based system that monitors a pet's water intake and enables remote refilling via MQTT.  
It was built using ESP8266, Node.js, MongoDB, and Node-RED.

## Project Overview

- Goal: Enable pet owners to remotely refill water and track daily intake
- Functionality:
  - Mobile app(MQTT Panel) sends refill command via MQTT
  - ESP8266 activates water pump through relay
  - Water intake is estimated and recorded
  - Node.js server saves data to MongoDB
  - Node-RED handles MQTT message logic

## Hardware and Design

- MCU: WEMOS D1 R1 (ESP8266)
- Sensors: WK Water Level Sensor, NO-Type Water Sensor
- Pump: Small Submersible Pump
- Relay: 1-Channel Relay Module
- Power: 5V DC Input

Design Notes:
- Sensor detects water level below threshold
- MQTT app triggers pump operation
- Each refill estimates ~200ml consumption

## Software Components

- Firmware: `main.ino` (Arduino IDE)
- Server: Node.js with MQTT & MongoDB (`server_f.js`)
- Flow: Node-RED (`flows.json`)
- App: IoT MQTT Panel (Android)

## MQTT Topics Used

| Topic                 | Description                  |
|-----------------------|------------------------------|
| `/pet/water/command`  | Trigger water refill         |
| `/pet/water/volume`   | Publish estimated intake     |
| `/pet/water/reset`    | Reset daily intake counter   |
| `/pet/water/alert`    | (Planned) Low-water alert    |

## Demo
[![시연 영상](https://img.youtube.com/vi/mQTfiPeakgI/0.jpg)](https://youtu.be/mQTfiPeakgI)

---

## 반려동물 음수량 모니터링 시스템

이 프로젝트는 반려동물의 물 섭취를 추적하고 원격 급수를 지원하는 IoT 시스템입니다.  
ESP8266, Node.js, MongoDB, Node-RED를 활용해 구현되었습니다.

### 프로젝트 개요

- 목표: 반려동물의 하루 음수량을 추적하고, 모바일 앱으로 원격 급수 가능하게 함
- 기능:
  - 모바일 앱(MQTT Panel)이 MQTT를 통해 급수 명령 전송
  - ESP8266이 릴레이를 통해 펌프 작동
  - 음수량은 1회 기준 200ml로 추정
  - MongoDB에 누적 저장
  - Node-RED로 MQTT 흐름 제어

### 하드웨어 및 설계

- MCU: WEMOS D1 R1 (ESP8266)
- 센서: WK 수위센서, NO 타입 수위센서
- 펌프: 소형 수중펌프
- 릴레이: 1채널 릴레이
- 전원: 5V DC 입력

설계 노트:
- 수위센서가 물 부족 시 감지
- MQTT 앱에서 급수 버튼 누르면 펌프 작동
- 급수 1회당 약 200ml로 간주

### 소프트웨어 구성

- 펌웨어: `main.ino` (Arduino IDE)
- 서버: Node.js + MQTT + MongoDB (`server_f.js`)
- 흐름: Node-RED (`flows.json`)
- 앱: IoT MQTT Panel (안드로이드)

### 사용한 MQTT 토픽

| 토픽명                   | 용도                        |
|---------------------------|-----------------------------|
| `/pet/water/command`      | 급수 명령 전송              |
| `/pet/water/volume`       | 급수량 전송 (예: 200ml)     |
| `/pet/water/reset`        | 음수량 초기화               |
| `/pet/water/alert`        | 물 부족 경고 (예정)         |




