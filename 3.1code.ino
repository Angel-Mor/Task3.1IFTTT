#include <BH1750.h>
  #include <WiFiNINA.h>
  #include <ESP_Mail_Client.h>
 
  const char* ssid = "Redmi";
  const char* password = "123456789";
  
  SMTPSession smtp;
  SMTP_Message message;
  ESP_Mail_Session session;
  
  const char* smtp_host = "smtp.gmail.com";
  const int smtp_port = 465;
  const char* email_sender = "angelmor0805@gmail.com";
  const char* email_sender_password = "movyspgddrttauyq";
  const char* email_recipient = "angel4761.be23@chitkara.edu.in";
  
  BH1750 lightMeter;
  
  void setup() {
    Serial.begin(9600);
    Wire.begin();
    connectWiFi();
  
    if (!lightMeter.begin()) {
      Serial.println("Error initializing BH1750 sensor.");
      while (1);
    }
  
    session.server.host_name = smtp_host;
    session.server.port = smtp_port;
    session.login.email = email_sender;
    session.login.password = email_sender_password;
    session.login.user_domain = "";
    message.sender.name = "Arduino Light Monitor";
    message.sender.email = email_sender;
    message.subject = "Sunlight Exposure Notification";
    message.addRecipient("Terrarium Monitor", email_recipient);
  }
  
  void loop() {
    float lux = lightMeter.readLightLevel();
    Serial.print("Light: ");
    Serial.print(lux);
    Serial.println(" lx");
  
    if (lux > 800) {
      message.text.content = "Your terrarium is currently exposed to sunlight.";
      sendEmail();
    } else if (lux < 100) {
      message.text.content = "Sunlight exposure has ended.";
      sendEmail();
    }
  
    delay(60000);
  }
  
  void connectWiFi() {
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
  
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
    }
  
    Serial.println("Connected to Wi-Fi");
  }
  
  bool sendEmail() {
    if (!smtp.connect(&session)) {
      Serial.println("Failed to connect to SMTP server");
      return false;
    }
  
    if (!MailClient.sendMail(&smtp, &message)) {
      Serial.println("Error sending Email, " + smtp.errorReason());
      return false;
    }
  
    Serial.println("Email sent successfully");
    return true;
  }
