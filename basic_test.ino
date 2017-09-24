/* Ethernet W5100 Shield (W5100) [B004] : http://rdiot.tistory.com/33 [RDIoT Demo] */

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#include <SPI.h>
#include <Ethernet.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // LCD2004

//  * Ethernet shield attached to pins 10, 11, 12, 13
//byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 }; //MAC Address
char server[] = "www.naver.com";    // name address for Google (using DNS)
IPAddress ip(192, 168, 0, 177); // DHCP로 안될때
EthernetClient client;

#define REQ_BUF_SZ   20       // HTTP Request Buffer
char HTTP_req[REQ_BUF_SZ] = {0}; // 버퍼를 초기상태로 함
char req_index = 0;              // 버퍼인덱스 초기값으로

void setup()
{
  lcd.init();  // initialize the lcd 
  lcd.backlight();
  lcd.print("start LCD2004");

  delay(1000);

  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("B004:W5100 EthernetS");
  
  lcd.setCursor(0,1);
  lcd.print("Setup Start        ");

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    //Serial.println("Failed to configure Ethernet using DHCP");
    lcd.setCursor(0,1);
    lcd.print("Config Failed DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    //Ethernet.begin(mac, ip);
    // no point in carrying on, so do nothing forevermore:
    for (;;)
      ;
  }

  String ipaddr="";
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    ipaddr += Ethernet.localIP()[thisByte];
    if(thisByte !=3 )
    ipaddr += ".";
  }

  lcd.setCursor(0,1);
  lcd.print("ip="+ipaddr+"  ");

  // give the Ethernet shield a second to initialize:
  delay(2000);
  lcd.setCursor(0,1);
  lcd.print("connecting...     ");
  
  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    lcd.setCursor(0,2);
    lcd.print("naver.com connected ");
    // Make a HTTP request:
    //client.println("GET /search?q=arduino HTTP/1.1");
    client.println("GET / HTTP/1.1");
    client.println("Host: www.naver.com");
    client.println("Connection: close");
    client.println();

  }
  else {
    // kf you didn't get a connection to the server:
    lcd.setCursor(0,2);
    lcd.print("connected failed");
  }

}

void loop()
{
 
  lcd.setCursor(0,0);
  lcd.print("B004:W5100 EthernetL");

 if (client.available()) {
    char c = client.read();
    //Serial.print(c);
      if (req_index < (REQ_BUF_SZ - 1)) {
         HTTP_req[req_index] = c;          
         req_index++;
       }

    lcd.setCursor(0,3);
    lcd.print(HTTP_req);  
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
   // lcd.setCursor(0,3);
   // lcd.print("disconnecting.    ");
    client.stop();

    // do nothing forevermore:
    while (true);
  }
}
