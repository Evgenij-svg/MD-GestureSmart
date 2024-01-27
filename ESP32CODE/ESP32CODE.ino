
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "Func.h"
#include <vector>
//#include "ArduinoJson.h"  // Подключаем библиотеку для работы с JSON

// Ваш код для определения структуры MyObject и объявления вектора ArrayState



struct MyObject {
  String name;
  String gesture;
  String pin;
  String value;
  String id;
};

std::vector<MyObject> ArrayState;

// Пример переменной, значение которой будет выводиться на веб-страницу
String sensorValue = " ";

String SensorPrev = " ";


// const char* ssid = "TechnoPark";
// const char* password = "test@TEST";

const char* ssid = "Get-72F2E0";
const char* password = "gjmltncdmf";

// const char* ssid = "mdwifi";
// const char* password = "12345678";

// Создаем объект сервера на порту 80
AsyncWebServer server(80);



void addNewBoxHouse(AsyncWebServerRequest* request) {
  // Получаем параметры запроса
  String gesture = request->getParam("gesture")->value();
  String name = request->getParam("name")->value();
  String id = request->getParam("id")->value();
  String pin = request->getParam("pin")->value();
  String value = request->getParam("value")->value();


  ArrayState.push_back({ name, gesture, pin, value, id });

  for (const auto& obj : ArrayState) {
    Serial.print(obj.name);
    Serial.print(" ");
    Serial.print(obj.gesture);
    Serial.print(" ");
    Serial.print(obj.pin);
    Serial.print(" ");
    Serial.print(obj.value);
    Serial.print(" ");
    Serial.print(obj.id);
    Serial.println(' ');
  }


  // Отправляем ответ клиенту
  request->send(200, "text/plain", "OK");
}


void changeHouse(AsyncWebServerRequest* request) {

  String id = request->getParam("id")->value();
  String value = request->getParam("value")->value();

  Serial.print(id);
  Serial.print(" ");
  Serial.print(value);

  for (auto& obj : ArrayState) {
    if (obj.id == id) {
      obj.value = value;
    }
  }


  for (const auto& obj : ArrayState) {

    if (obj.id == id) {
      // Serial.print(obj.name);
      // Serial.print(" ");
      int PIN = obj.pin.toInt();
      pinMode(PIN,OUTPUT);
      if(obj.value == "true"){
        digitalWrite(PIN,1);
      }else if(obj.value == "false"){
        digitalWrite(PIN,0);
      }

    }
  }

  Serial.println(" ");
  request->send(200, "text/plain", "OK");
}

void removeBoxHouse(AsyncWebServerRequest* request) {

  String id = request->getParam("id")->value();

  int targetIndex = -1;

  for (size_t i = 0; i < ArrayState.size(); i++) {
    if (ArrayState[i].id == id) {
      targetIndex = i;
      break;
    }
  }

  if (targetIndex != -1) {
    // Используем итератор для удаления элемента из вектора
    ArrayState.erase(ArrayState.begin() + targetIndex);
  }



  for (const auto& obj : ArrayState) {
    Serial.print(obj.name);
    Serial.print(" ");
    Serial.print(obj.gesture);
    Serial.print(" ");
    Serial.print(obj.pin);
    Serial.print(" ");
    Serial.print(obj.value);
    Serial.print(" ");
    Serial.print(obj.id);
    Serial.println(' ');
  }

  Serial.println(" ");
  request->send(200, "text/plain", "OK");
}
String OBJ2JSON(MyObject& obj) {
  String JSON = "{\"name\":\"" + obj.name + "\",\"gesture\":\"" + obj.gesture + "\",\"pin\":\"" + obj.pin + "\",\"value\":\"" + obj.value + "\",\"id\":\"" + obj.id + "\"}";
  return JSON;
}

void sendArrayStateAsJson(AsyncWebServerRequest* request) {
  String arrayJSON = "";
  for (const auto& obj : ArrayState) {
    MyObject objCopy = obj; // Create a non-const copy of obj
    arrayJSON += OBJ2JSON(objCopy) + ",";
  }
  // Remove the trailing comma from the arrayJSON
  if (!arrayJSON.isEmpty()) {
    arrayJSON.remove(arrayJSON.length() - 1);
  }

  request->send(200, "application/json", "[" + arrayJSON + "]");
}


void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println();
  Serial.print("Подключение к Wi-Fi...");
  WiFi.begin(ssid, password);  // Подключение к существующей сети Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Подключение к Wi-Fi выполнено успешно!");
  Serial.print("IP-адрес ESP32: ");
  Serial.println(WiFi.localIP());


  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    WEBSITE(request);
  });

  server.on("/addNewBoxHouse", HTTP_GET, [](AsyncWebServerRequest* request) {
    addNewBoxHouse(request);
  });

  server.on("/removeBoxHouse", HTTP_GET, [](AsyncWebServerRequest* request) {
    removeBoxHouse(request);
  });

  server.on("/changeHouse", HTTP_GET, [](AsyncWebServerRequest* request) {
    changeHouse(request);
  });



  // Обработчик маршрута для получения значения переменной
  server.on("/sensor", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/plain", sensorValue.c_str());
  });

  server.on("/Gesture_data", HTTP_POST, [](AsyncWebServerRequest* request) {
    String data = request->arg("gestures");
    sensorValue = data;
    SensorPrev = sensorValue;
    request->send(200, "text/plain", "data received");
  });

    server.on("/get_data", HTTP_GET, [](AsyncWebServerRequest* request) {
    sendArrayStateAsJson(request);
  });


  // server.on("/data", HTTP_POST, [](AsyncWebServerRequest* request) {

  // });

  // Запуск сервера
  server.begin();
}

String removeBeforeLastSpace(String inputString) {
  int spaceIndex = inputString.lastIndexOf(' ');   // Ищем индекс последнего пробела
  if (spaceIndex != -1) {                          // Если пробел найден
    return inputString.substring(spaceIndex + 1);  // Возвращаем подстроку после пробела
  } else {
    return inputString;  // Если пробел не найден, возвращаем исходную строку
  }
}



void loop() {

}