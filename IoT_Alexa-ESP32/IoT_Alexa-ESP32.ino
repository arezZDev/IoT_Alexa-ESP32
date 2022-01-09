#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include "fauxmoESP.h"
#include <IRremote.h>


#define WIFI_SSID "NOMBRE-DE-TU-RED-WIFI"
#define WIFI_PASS "CONTRASEÑA-DE-TU-RED"

fauxmoESP fauxmo;

// -----------------------------------------------------------------------------

#define SERIAL_BAUDRATE     115200

#define PIN_ESPEJO          25
#define IR_MANDAR_PIN       4

#define ID_ESPEJO           "ESPEJO"
#define ID_DISPOSITIVO2     "LEDS ROJOS"
#define ID_DISPOSITIVO3     "LEDS VERDES"
#define ID_DISPOSITIVO4     "LEDS AZULES"
#define ID_DISPOSITIVO5     "LEDS MORADOS"
#define ID_DISPOSITIVO6     "LEDS BLANCOS"
#define ID_DISPOSITIVO7     "LEDS FLASH"
#define ID_DISPOSITIVO8     "LEDS SMOOTH"
#define ID_DISPOSITIVO9     "BRILLO MAS"
#define ID_DISPOSITIVO10     "BRILLO MENOS"

// -----------------------------------------------------------------------------
uint8_t sRepeats = 0; //IR LED

// -----------------------------------------------------------------------------
// Wifi
// -----------------------------------------------------------------------------

void wifiSetup() {

  // Set WIFI module to STA mode
  WiFi.mode(WIFI_STA);

  // Connect
  Serial.printf("[WIFI] Conectando a: %s ", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // Wait
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  // Connected!
  Serial.printf("[WIFI] Modo: ESTACION, SSID: %s, Direccion IP: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
  Serial.printf("Esperando instrucciones de ALEXA...\n");

}

void setup() {

  pinMode(IR_MANDAR_PIN, OUTPUT);
  // Init serial port and clean garbage
  Serial.begin(SERIAL_BAUDRATE);
  Serial.println();
  Serial.println();
  Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));
  IrSender.begin(IR_MANDAR_PIN, ENABLE_LED_FEEDBACK); // Specify send pin and enable feedback LED at default feedback LED pin
  Serial.print(F("Ready to send IR signals at pin "));
  Serial.println(IR_MANDAR_PIN);

  //Declaramos pines
  pinMode(PIN_ESPEJO, OUTPUT);


  //INICIALIZAMOS COMO QUEREMOS QUE INICIEN
  digitalWrite(PIN_ESPEJO, HIGH);//AL SER UN RELAY ES EL INVERSO SI ESTA EN HIGH ESTA APAGADO
  digitalWrite(IR_MANDAR_PIN, LOW);

  // Wifi
  wifiSetup();

  // By default, fauxmoESP creates it's own webserver on the defined port
  // The TCP port must be 80 for gen3 devices (default is 1901)
  // This has to be done before the call to enable()
  fauxmo.createServer(true); // not needed, this is the default value
  fauxmo.setPort(80); // This is required for gen3 devices

  // You have to call enable(true) once you have a WiFi connection
  // You can enable or disable the library at any moment
  // Disabling it will prevent the devices from being discovered and switched
  fauxmo.enable(true);

  // You can use different ways to invoke alexa to modify the devices state:
  // "Alexa, turn yellow lamp on"
  // "Alexa, turn on yellow lamp
  // "Alexa, set yellow lamp to fifty" (50 means 50% of brightness, note, this example does not use this functionality)

  // Add virtual devices
  fauxmo.addDevice(ID_ESPEJO);
  fauxmo.addDevice(ID_DISPOSITIVO2);
  fauxmo.addDevice(ID_DISPOSITIVO3);
  fauxmo.addDevice(ID_DISPOSITIVO4);
  fauxmo.addDevice(ID_DISPOSITIVO5);
  fauxmo.addDevice(ID_DISPOSITIVO6);
  fauxmo.addDevice(ID_DISPOSITIVO7);
  fauxmo.addDevice(ID_DISPOSITIVO8);
  fauxmo.addDevice(ID_DISPOSITIVO9);
  fauxmo.addDevice(ID_DISPOSITIVO10);

  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {

    // Callback when a command from Alexa is received.
    // You can use device_id or device_name to choose the element to perform an action onto (relay, LED,...)
    // State is a boolean (ON/OFF) and value a number from 0 to 255 (if you say "set kitchen light to 50%" you will receive a 128 here).
    // Just remember not to delay too much here, this is a callback, exit as soon as possible.
    // If you have to do something more involved here set a flag and process it in your main loop.

    Serial.printf("[MAIN] Dispositivo #%d (%s) estado: %s valor: %d\n", device_id, device_name, state ? "ON" : "OFF", value);

    // Checking for device_id is simpler if you are certain about the order they are loaded and it does not change.
    // Otherwise comparing the device_name is safer.

    if (strcmp(device_name, ID_ESPEJO) == 0) {
      digitalWrite(PIN_ESPEJO, state ? LOW : HIGH);
    }
    else if (strcmp(device_name, ID_DISPOSITIVO2) == 0) {
      //digitalWrite(PIN_DISPOSITIVO2, state ? LOW : HIGH);
      Serial.println(F("Mandamos Rojo IR"));
      IrSender.sendNECRaw(0xE619FF00, sRepeats);


    }
    else if (strcmp(device_name, ID_DISPOSITIVO3) == 0) {
      //digitalWrite(PIN_DISPOSITIVO2, state ? LOW : HIGH);
      Serial.println(F("Mandamos Verde IR"));
      IrSender.sendNECRaw(0xEE11FF00, sRepeats);

    }
    else if (strcmp(device_name, ID_DISPOSITIVO4) == 0) {
      //digitalWrite(PIN_DISPOSITIVO2, state ? LOW : HIGH);
      Serial.println(F("Mandamos Azul IR"));
      IrSender.sendNECRaw(0xE41BFF00, sRepeats);

    }
    else if (strcmp(device_name, ID_DISPOSITIVO5) == 0) {
      //digitalWrite(PIN_DISPOSITIVO2, state ? LOW : HIGH);
      Serial.println(F("Mandamos Morado IR"));
      IrSender.sendNECRaw(0xE31CFF00, sRepeats);

    }
    else if (strcmp(device_name, ID_DISPOSITIVO6) == 0) {
      //digitalWrite(PIN_DISPOSITIVO2, state ? LOW : HIGH);
      Serial.println(F("Mandamos Blanco IR"));
      IrSender.sendNECRaw(0xEA15FF00, sRepeats);

    }
    else if (strcmp(device_name, ID_DISPOSITIVO7) == 0) {
      //digitalWrite(PIN_DISPOSITIVO2, state ? LOW : HIGH);
      Serial.println(F("Mandamos Flash IR"));
      IrSender.sendNECRaw(0xB24DFF00, sRepeats);

    }
    else if (strcmp(device_name, ID_DISPOSITIVO8) == 0) {
      //digitalWrite(PIN_DISPOSITIVO2, state ? LOW : HIGH);
      Serial.println(F("Mandamos Smooth IR"));
      IrSender.sendNECRaw(0xF30CFF00, sRepeats);

    }
    else if (strcmp(device_name, ID_DISPOSITIVO9) == 0) {
      //digitalWrite(PIN_DISPOSITIVO2, state ? LOW : HIGH);
      Serial.println(F("Mandamos Brillo Mas IR"));
      IrSender.sendNECRaw(0xF609FF00, sRepeats);

    }
    else if (strcmp(device_name, ID_DISPOSITIVO10) == 0) {
      //digitalWrite(PIN_DISPOSITIVO2, state ? LOW : HIGH);
      Serial.println(F("Mandamos Brillo Menos IR"));
      IrSender.sendNECRaw(0xE21DFF00, sRepeats);

    }

  });

}

void loop() {

  // fauxmoESP uses an async TCP server but a sync UDP server
  // Therefore, we have to manually poll for UDP packets
  fauxmo.handle();

  // This is a sample code to output free heap every 5 seconds
  // This is a cheap way to detect memory leaks
  static unsigned long last = millis();
  if (millis() - last > 5000) {
    last = millis();
    Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
  }

  // If your device state is changed by any other means (MQTT, physical button,...)
  // you can instruct the library to report the new state to Alexa on next request:
  // fauxmo.setState(ID_ESPEJO, true, 255);

}
