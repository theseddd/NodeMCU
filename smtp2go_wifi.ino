#include "base64.hpp"
#include <ESP8266WiFi.h>
///WIFI////////////////////////////////////////////////////////
 const char* ssid     = "Name WiFi";
 const char* password = "Password WiFi";
///SMTP Server/////////////////////////////////////////////////
 const char* host = "mail.smtp2go.com";
 const int httpPort = 2525;
 unsigned char UserName[] = "Username smtp2go";
 unsigned char Password[] = "Password smtp2go";
/////////////////////////////////////////////////////////////// 
 String UserName64, Password64, Sender, Recipient, Subject, Body, ServerResponse;
 int wait = 1000;
 WiFiClient client;
 
void setup() {
  Serial.begin(115200);
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");
  //////////////////////////////////////////////////////////////
  encodeBase64();
  Emailformat();
  senderEmail();
  //////////////////////////////////////////////////////////////
  }

void loop() {
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
  if (!client.connected()) {
    //Serial.println();
    //Serial.println("disconnecting.");
    client.stop();
  }
}
 void SendMsg(String m) {
   client.println(m);
   Serial.println(">>>" + m);
   delay(wait); // wait for a response 
   GetResponse();
 }
 void GetResponse() {
    if (client.available()) {
         char c = client.read();
         while (client.available()) { // Store command char by char.
           ServerResponse +=c;
           c = client.read();
         }
    Serial.println("<<<" + ServerResponse);
    ServerResponse="";
  }
 }
 void Emailformat(){
  Sender = "Sender Email";
  Recipient = "Recipient Email";
  Subject = "Test Messaggio Arduino!";
  Body += "Test messaggio.\n"; // "\n" is new line
  Body += "abcdefghijklmnopqrstuvwxyz.\n";
  Body += "0123456789.";
 }
 void senderEmail(){
  if (!client.connect(host, httpPort)) {
    Serial.println("SMTP server connection failed");
    return;
  }else{
     Serial.println("SMTP server connected ");
     SendMsg("helo"); /* say hello*/
     SendMsg("ehlo"); /* say hello*/
     //SendMsg("STARTTLS");
     SendMsg("AUTH LOGIN");
     SendMsg(UserName64); /* Username*/
     SendMsg(Password64); /* Password */
     SendMsg("MAIL From:<" + Sender +">"); /* identify sender */
     SendMsg("RCPT To:<" + Recipient + ">"); /* identify recipient */
     SendMsg("DATA");
     SendMsg("To: " + Recipient); /*  recipient in message header */
     SendMsg("From: " + Sender); /* seder name in message header */
     SendMsg("Subject: "+ Subject); /* insert subject */
     SendMsg(""); /* empty line */
     SendMsg(Body); /* insert body */
     SendMsg(""); /* empty line */
     SendMsg("."); /* end mail */
     SendMsg("QUIT"); /* terminate connection */
     client.println();
     delay(1000);
     client.stop();
  }
 }
 void encodeBase64(){
  unsigned char base64a[63];
  unsigned char base64b[63];
  int base64_lengtha = encode_base64(UserName, sizeof(UserName)-1, base64a);
  int base64_lengthb = encode_base64(Password, sizeof(Password)-1, base64b);
  UserName64 = (char *) base64a;;
  Password64 = (char *) base64b;;
 }
 
