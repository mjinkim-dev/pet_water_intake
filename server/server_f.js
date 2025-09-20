const mongoclient = require("mongodb").MongoClient;
const moment = require("moment-timezone");
const mqtt = require("mqtt");
const url = "mongodb+srv:/<username>:<password>@<cluster>.mongodb.net/";

const express = require("express");
const app = express();

let mydb;
mongoclient
  .connect(url)
  .then((client) => {
    console.log("Mongo DB 접속 성공");
    mydb = client.db("final"); //.db() : client의 db에 접속하는 함수

    app.listen(8080, function () {
      console.log("포트 8080으로 서버 대기중 ... ");
    });
  })
  .catch((err) => {
    console.log(err);
  });

// MQTT 클라이언트 설정
const mqttClient = mqtt.connect("mqtt://broker.hivemq.com"); // 예시로 공용 MQTT 브로커 사용

mqttClient.on("connect", () => {
  console.log("MQTT 브로커에 연결 성공");
  mqttClient.subscribe("/pet/water/command", (err) => {
    if (!err) {
      console.log("토픽 구독 성공: /pet/water/command");
    } else {
      console.log("토픽 구독 실패: ", err);
    }
  });
});

mqttClient.on("message", (topic, message) => {
  if (topic === "/pet/water/command") {
    const currentTime = moment().tz("Asia/Seoul").format();
    const newUser = { time: currentTime }; // 새로운 객체 생성

    mydb.collection("water-intake").insertOne(newUser)
      .then((result) => {
        console.log("User Created Successfully:", result);
      })
      .catch((err) => {
        console.log("User was not Successfully created:", err);
      });
  }
});

app.get("/", (req, res) => {
  res.send("Server is running and listening to MQTT messages.");
});
