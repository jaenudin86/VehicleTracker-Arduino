void uploadstatus(){//calling RESTful API to upload datapoint to MCS to report LED status

  while (!c2.connect(SITE_URL, 80))
  {
    printData(".");
    delay(500);
  }
  
  delay(100);
  
  if(digitalRead(13)==1)
    upload_led = "LED,,on";
  else
    upload_led = "LED,,off";
  
  int thislength = upload_led.length();
  HttpClient http(c2);
  c2.print("POST /mcs/v2/devices/");
  c2.print(DEVICEID);
  c2.println("/datapoints.csv HTTP/1.1");
  c2.print("Host: ");
  c2.println(SITE_URL);
  c2.print("deviceKey: ");
  c2.println(DEVICEKEY);
  c2.print("Content-Length: ");
  c2.println(thislength);
  c2.println("Content-Type: text/csv");
  c2.println("Connection: close");
  c2.println();
  c2.println(upload_led);
  delay(500);

  int errorcount = 0;
  while (!c2.available())
  {
    printData(".");
    delay(100);
  }
  int err = http.skipResponseHeaders();
  int bodyLen = http.contentLength();
  
  while (c2)
  {
    int v = c2.read();
    if (v != -1)
    {
      printData(String(char(v)));
    }
    else
    {
      printlnData("no more content, disconnect");
      c2.stop();
    }
    
  }
  printlnData("");
}

bool uploadGPS(){

  while (!c2.connect(SITE_URL, 80))
  {
    printData(".");
    delay(500);
  }
  
  delay(100);

  float latitude_post=latitude;
  float longitude_post=longitude;  
  if(latitude>-90 && latitude<90 && longitude>0 && longitude<360)
  {
    sprintf(buffer_latitude, "%.4f", latitude);
    sprintf(buffer_longitude, "%.4f", longitude);
  }
  else {
    return false;
  }
  printData("Latitude:");
  printlnData(String(buffer_latitude));
  printData("Longitude:");
  printlnData(String(buffer_longitude));
  String upload_GPS = "GPS,,"+String(buffer_latitude)+","+String(buffer_longitude)+","+"0"+"\n"+"latitude,,"+buffer_latitude+"\n"+"longitude,,"+buffer_longitude;//null altitude
  printlnData(upload_GPS);
  int GPS_length = upload_GPS.length();
  HttpClient http(c2);
  c2.print("POST /mcs/v2/devices/");
  c2.print(DEVICEID);
  c2.println("/datapoints.csv HTTP/1.1");
  
  c2.print("Host: ");
  c2.println(SITE_URL);
  
  c2.print("deviceKey: ");
  c2.println(DEVICEKEY);
  
  c2.print("Content-Length: ");
  c2.println(GPS_length);
  
  c2.println("Content-Type: text/csv");
  c2.println("Connection: close");
  c2.println();
  c2.println(upload_GPS);
  delay(500);

  int errorcount = 0;  
  while (!c2.available())
  {
    delay(100);
  }
  
  int err = http.skipResponseHeaders();
  int bodyLen = http.contentLength();
  
  while (c2)
  {    
    int v = c2.read();
    if (v != -1)
    {      
      printData(String(char(v)));
    }
    else
    {
      printlnData("no more content, disconnect");
      c2.stop();
    }    
  } 
  printlnData(String(c2));
  return true;
}
