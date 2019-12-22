#include <HttpPacket.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <dht.h>
HttpPacketHead packet;
dht DHT;
#define DHT11_PIN 4


unsigned int tempMax = 20;   //报警温度



int sent = 0;
char OneNetServer[] = "api.heclouds.com";       //不需要修改
const char ssid[] = "GMJ-iPhone";     //修改为自己的路由器用户名
const char password[] = "tg300437"; //修改为自己的路由器密码
char device_id[] = "562274018";    //修改为自己的设备ID
char API_KEY[] = "lr5Xz=y8mKOxQ=VUZo7JY=6GhZs=";    //修改为自己的API_KEY
char sensor_id1[] = "Temperature";
char sensor_id2[] = "Humidity";


void connectWifi(){  
  Serial.print("Connecting to " + *ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");  
  }
  Serial.println("");
  Serial.println("Connected");
  Serial.println("");
  digitalWrite(0, HIGH);
  delay(1000);
  digitalWrite(0, LOW);
  delay(1000);
  digitalWrite(0, HIGH);
}

void postDataToOneNet(char* API_VALUE_temp, char* device_id_temp, char* sensor_id_temp, float thisData){  
  WiFiClient client;
  StaticJsonBuffer<250> jsonBuffer;
  JsonObject& myJson = jsonBuffer.createObject();
  JsonArray& datastreams= myJson.createNestedArray("datastreams");
  JsonObject& id_datapoints = datastreams.createNestedObject();
  id_datapoints["id"] = sensor_id_temp;
  JsonArray& datapoints = id_datapoints.createNestedArray("datapoints");
  JsonObject& value = datapoints.createNestedObject();
  value["value"] =thisData;
  char p[180];
  myJson.printTo(p, sizeof(p)); 
  packet.setHostAddress(OneNetServer);
  packet.setDevId(device_id_temp);   //device_id
  packet.setAccessKey(API_VALUE_temp);  //API_KEY
  /*create the http message about add datapoint */
  packet.createCmdPacket(POST, TYPE_DATAPOINT, p);
  if (strlen(packet.content))
  Serial.print(packet.content);
  Serial.println(p);
  char cmd[400];
  memset(cmd, 0, 400);  
  strcpy(cmd, packet.content);
  strcat(cmd, p);
  if (client.connect(OneNetServer, 80)){ 
    Serial.println("WiFi Client connected ");
    client.print(cmd);
    delay(1000);
  }//end if
  //  Serial.println(cmd);
  client.stop();
}



void setup() {
  
  Serial.begin(9600);
  pinMode(16, OUTPUT);

}

void loop() {
 int chk = DHT.read11(DHT11_PIN);
 Serial.print(DHT.humidity, 1);
 Serial.print(",\t");
 Serial.println(DHT.temperature, 1);
 int temp =(DHT.temperature); int hum = (DHT.humidity); //float pm = (((dustVal / 1024) - 0.0356) * 14868 * 0.035);
 postDataToOneNet(API_KEY, device_id, sensor_id1, temp);
  delay(100);
  postDataToOneNet(API_KEY, device_id, sensor_id2, hum);
  Serial.println("N0 " + String(sent) + " Stream: was send");
  sent++;

if(temp <= tempMax){
  digitalWrite(16,HIGH);
  }
else{
  digitalWrite(16,LOW);
  }
 
  Serial.print("Temp: ");  Serial.print(temp);
  delay(1000); // 控制刷新速度

 
}
