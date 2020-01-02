// This #include statement was automatically added by the Particle IDE.
#include <ArduinoJson.h>

// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_SSD1306.h>


//battery
double batteryLife=0;
FuelGauge fuel;


int photosensor = A0;
int analogvalue;
int currentValue=0;
/* ============== MAIN =====================*/

 //Use I2C with OLED RESET pin on D4
 #define OLED_RESET D4
 Adafruit_SSD1306 oled(OLED_RESET);


 unsigned long previousMillis;
 unsigned long interval = 30000;
 
 bool isOpen=false;
 
 unsigned long lastReading;


void setup() {
    Serial.begin();
    Particle.variable("analogvalue", &analogvalue, INT);
    
    //subscribe to webservice
    
    Particle.subscribe("hook-response/getstartup", myHandler, MY_DEVICES);
    

 
    oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
     
    
    // oled.display(); // show splashscreen

  
  
  
  
  
  
  // Clear the buffer.
	oled.clearDisplay();

	
	
	///Battery 
	batteryLife = fuel.getSoC();
    Particle.variable("batteryLife", batteryLife);
	Particle.function("getSoC", getBatteryLife);
	
	
	lastReading=millis();
	
	
// 	webservice();
	
	
}



    String t=" start";

void loop()
{
  
   detectOpen();
   
   if(isOpen)
   {
       
    //   OpenBox("hello world");
   }
   else
   {
    //   closeBox();
   }
    

  
 //testscrolltext();
  delay(2000);
  
    
}

void OpenBox(String text)
{
    Serial.printlnf("ITS OPEN DIPSLAYING MESSAGE");
    showText(text);
}


void closeBox()
{
    // if(!isOpen) return;
    oled.clearDisplay();
    oled.display();
    requestIdea();
    Serial.printlnf("ITS Closed");
}
void showText(String text)
{
    Serial.printlnf("showing string");
    oled.clearDisplay();
  
    delay(200);
    oled.setTextSize(2);
    oled.setTextColor(WHITE);
    oled.setCursor(0,0);
//   oled.print(Time.hourFormat12()); oled.print(":"); oled.print(Time.minute()); oled.print(":"); oled.print(Time.second());
    oled.print(text);
//   oled.setTextColor(BLACK, WHITE); // 'inverted' text
    oled.setTextColor(WHITE,BLACK); 
    oled.display();
    delay(2000);
}



//function get battery life upon request
int getBatteryLife(String command) {
//Set Battery Life Variable with current battery life value
    batteryLife = fuel.getSoC();

    return (int)batteryLife;
}


void testscrolltext(void) {
  oled.setTextSize(3);
  oled.setTextColor(WHITE);
  oled.setCursor(10,0);
  oled.clearDisplay();
//   oled.setTextWrap(false);
  oled.println("Drive a car");
  oled.display();
 
  oled.startscrollright(0x00, 0x0F);
  delay(32000);
  oled.stopscroll();
  delay(1000);
  oled.startscrollleft(0x00, 0x0F);
  delay(2000);
  oled.stopscroll();
  delay(1000);    
  oled.startscrolldiagright(0x00, 0x07);
  delay(2000);
  oled.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  oled.stopscroll();
}



void myHandler(const char *event, const char *data) {
  // Handle the webhook response
  
    // Serial.printlnf("RESPONSE ");//for debug
    // Serial.printlnf(data);//for debug
    StaticJsonDocument<200> doc;
  

    DeserializationError error = deserializeJson(doc, data);
    const char* body=doc["body"];
    //Serial.printlnf(body);//for debug
    
    String s= body;
    String ss=s.substring(16,s.length()-2);
    t=ss;
    Serial.printlnf(ss);
}

void detectOpen()
{
    int analogvalue2;
    int analogvalue3;
    /*
    this function detects whether the box is open or not
    smaller value is covered larger is open
    */
    
     analogvalue = analogRead(photosensor);
     delay(20);
     analogvalue2 = analogRead(photosensor);
     delay(20);
     analogvalue3 = analogRead(photosensor);
     analogvalue=(analogvalue+analogvalue2+analogvalue3)/2;
     
     if (currentValue==0)
     {
         currentValue=analogvalue;
     }
     
     
     
    
     if(analogvalue>40) //box is open
     {
         if(!isOpen)
         {
             Serial.printlnf("Openning");
             isOpen=true;
             OpenBox(t); ///idea should go here
         }
     }
     
     if(analogvalue < 40)
     {
         if(isOpen)
         {
             Serial.printlnf("Closing");
             isOpen=false;
             closeBox();
         }
     }

         
    Serial.printlnf("Cur %d Reading %d", currentValue, analogvalue);
       
    currentValue =analogvalue;
    lastReading=millis();
}

///random ideas that are stored on device
String getstaticIdea()
{
    String ideas[3];
    ideas[0]="idea 0";
    ideas[1]="idea 1";
    ideas[2]="idea 2";
    
    return "an idea";
}


////gets an idea from webservice and stroes it to be retrieved 
void requestIdea()
{
    String data = String(10);
  // Trigger the integration
    Particle.publish("getstartup", "", PRIVATE);
    
}

