//
// Created by Miguel Ángel Fernández Moreno on 12/2/23.
// Based on:
//      https://randomnerdtutorials.com/esp8266-nodemcu-send-email-smtp-server-arduino/
//

#ifndef UNTITLED1_EMAIL_H
#define UNTITLED1_EMAIL_H
/*
  Rui Santos
  Complete project details at:
   - ESP32: https://RandomNerdTutorials.com/esp32-send-email-smtp-server-arduino-ide/
   - ESP8266: https://RandomNerdTutorials.com/esp8266-nodemcu-send-email-smtp-server-arduino/

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
  Example adapted from: https://github.com/mobizt/ESP-Mail-Client
*/

// To send Emails using Gmail on port 465 (SSL), you need to create an app password: https://support.google.com/accounts/answer/185833

#include <Arduino.h>
#include <WiFi.h>
#include <ESP_Mail_Client.h>
#include "wifi_credentials.h"
#include "email_credentials.h"

// Uncomment and replace with your email credentials inside "email_credentials.h"
//const char* email_address = "email_address@gmail";
//const char* email_password = "email_password";

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

/* The SMTP Session object used for Email sending */
SMTPSession smtp;

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status);
void sendEmail(const String& IP);

void sendEmail(const String& IP){

    Serial.println();
    Serial.print("Connecting to AP");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(200);
    }

    /** Enable the debug via Serial port
     * none debug or 0
     * basic debug or 1
    */
    smtp.debug(1);

    /* Set the callback function to get the sending results */
    smtp.callback(smtpCallback);

    /* Declare the session config data */
    ESP_Mail_Session session;

    /* Set the session config */
    session.server.host_name = SMTP_HOST;
    session.server.port = SMTP_PORT;
    session.login.email = email_address;
    session.login.password = email_password;
    session.login.user_domain = "";

    /* Declare the message class */
    SMTP_Message message;

    /* Set the message headers */
    message.sender.name = "ESP";
    message.sender.email = email_address;
    message.subject = "ESP IP";
    message.addRecipient("Miguel", email_address);

    /*Send HTML message*/
    String htmlMsg1 = "<div style=\"color:#2f4468;\"><h1>";
    String htmlMsg2 = "</h1><p>- Sent from ESP board</p></div>";
    String fullMsg = htmlMsg1+ IP + htmlMsg2;
    Serial.println(fullMsg);
    message.html.content = fullMsg.c_str();
    message.text.charSet = "us-ascii";
    message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

    /*
    //Send raw text message
    String textMsg = "Hello World! - Sent from ESP board";
    message.text.content = textMsg.c_str();
    message.text.charSet = "us-ascii";
    message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

    message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
    message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;*/

    /* Set the custom message header */
    //message.addHeader("Message-ID: <abcde.fghij@gmail.com>");

    /* Connect to server with the session config */
    if (!smtp.connect(&session))
        return;

    /* Start sending Email and close the session */
    if (!MailClient.sendMail(&smtp, &message))
        Serial.println("Error sending Email, " + smtp.errorReason());
}

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status){
    /* Print the current status */
    Serial.println(status.info());

    /* Print the sending result */
    if (status.success()){
        Serial.println("----------------");
        ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
        ESP_MAIL_PRINTF("Message sent failled: %d\n", status.failedCount());
        Serial.println("----------------\n");
        struct tm dt;

        for (size_t i = 0; i < smtp.sendingResult.size(); i++){
            /* Get the result item */
            SMTP_Result result = smtp.sendingResult.getItem(i);
            time_t ts = (time_t)result.timestamp;
            localtime_r(&ts, &dt);

            ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
            ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
            ESP_MAIL_PRINTF("Date/Time: %d/%d/%d %d:%d:%d\n", dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec);
            ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients.c_str());
            ESP_MAIL_PRINTF("Subject: %s\n", result.subject.c_str());
        }
        Serial.println("----------------\n");
    }
}
#endif //UNTITLED1_EMAIL_H
