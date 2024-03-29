#include "Stepper_28BYJ_48.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

/**
 * LED RGB
 */
int led_r = 14; // D5
int led_g = 12;  // D6
int led_b = 13;  // D7

/**
 * Step motor
 */
Stepper_28BYJ_48 stepper(5,4,0,2);  // D1 D2 D3 D4
const int steps_to_open = 500;

/**
* WiFi settings - Modify as desired
**/
const char* wlan_ssid             = "AlgoAlgo";
const char* wlan_password         = "12345678";

/**
* Server settings
**/
const char host[]               = "192.168.0.163";
const int port                 = 8080;
const char baseURL[]           = "http://e2b72247.ngrok.io/lock/status/18bfd86f-539e-40e2-a917-64c9ed1d42d9";
//const char baseURL[]           = "http://192.168.0.163:8080/lock/status/18bfd86f-539e-40e2-a917-64c9ed1d42d9";

// Lock UUID
const char* lock_uuid = "18bfd86f-539e-40e2-a917-64c9ed1d42d9";


// Lock status
boolean isOpen = true;
boolean firstTime = true;

/***
 * Initial setup
 * @return void
 */
void setup() {
  Serial.begin(115200);
  Serial.println("clean");
  connect_wifi();
  pinMode (led_r, OUTPUT);
  pinMode (led_g, OUTPUT);
  pinMode (led_b, OUTPUT);
}

/***
 * Used to connect to the correct wifi network
 * @return void
 */
void connect_wifi(){
  WiFi.begin(wlan_ssid, wlan_password); //WiFi connection
 
  while (WiFi.status() != WL_CONNECTED) { //Wait for the WiFI connection completion
    led_rgb(255,255,255);
    delay(500);
    Serial.println("Waiting for connection");
  }

  firstTime = true;
}

/***
 * Main program loop
 * @return void
 */
void loop() {

        if(WiFi.status() == WL_CONNECTED){   //Check WiFi connection status
         
           HTTPClient http;    //Declare object of class HTTPClient
         
           http.begin(baseURL);  //Specify request destination
           http.addHeader("Content-Type", "text/plain");  //Specify content-type header
         
           int httpCode = http.POST("lock");   //Send the request
           String payload = http.getString();                  //Get the response payload
           http.end();  //Close connection
           Serial.print("Got:");
           Serial.print(httpCode);   //Print HTTP return code
           Serial.print(" payload: ");
           Serial.println(payload);    //Print request response payload
           process(payload);
         
         }else{
            Serial.println("Error in WiFi connection");   
            connect_wifi();
         }
         led_rgb(0,0,255);
         delay(3500);
}

/*****************************
 * Process the received data and executes the corresponding action
 * @return void
 **************************/
void process(String payload){
  if(payload.equals("OPEN") && (!isOpen || firstTime)){
    open_lock();
  } else if(payload.equals("CLOSE") && (isOpen || firstTime)){
    close_lock();
  }
}

/*****************************
 * Used to set the RGB led to the desired values
 * @param r red value
 * @param g green value
 * @param b blue value
 * @return void
 **************************/
void led_rgb(int r, int g, int b){
 analogWrite (led_r, r);
 analogWrite (led_g, g);
 analogWrite (led_b, b); 
}

/*****************************
 * Used to open the lock
 * @return void
 **************************/
void open_lock(){
  Serial.print("opening...");
  int i = steps_to_open;
  while(i > 0){
    i--;
    stepper.step(-1);
    led_rgb(0,255,0);
    yield();
  }
  isOpen = true;
  firstTime = false;
}

/*****************************
 * Used to close the lock
 * @return void
 **************************/
void close_lock(){
  Serial.print("closing...");
  int i = steps_to_open;
  while(i > 0){
    i--;
    stepper.step(1);
    led_rgb(255,0,0);
    yield();
  }
  isOpen = false;
  firstTime = false;
}
