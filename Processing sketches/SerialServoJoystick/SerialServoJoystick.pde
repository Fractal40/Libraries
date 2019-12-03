/**
* ControlP5 Slider2D
*
*
* find a list of public methods available for the Slider2D Controller
* at the bottom of this sketch.
*
* by Andreas Schlegel, 2012
* www.sojamo.de/libraries/controlp5
*
*/
import processing.serial.*;
import controlP5.*;

Serial port;


  
byte[] data = new byte[5]; 
int servoPWM = 700;
int serialPort = 5; //COM12
int ctrlFlag = 0;
String output = "";
String outputServo = "";
String outputPWM = str(servoPWM);
int lf = 10; // ASCII linefeed
boolean sendFlag = false;
int col;
boolean servoTestFlag = false;
float servoNum;
float defaultX = 50;
float defaultY = 50;
float rawX;
float rawY;
float x;
float y;
float alpha;
//int flag = 0;
boolean ctrl = false;

ControlP5 cp5;

Slider2D s;
Slider sliderXVector;
Slider servoSlider;
Button button_PWM;
Button button_reset;
Button button_sendXY;
Numberbox servonummer;



void setup() {
  size(400,400);
  cp5 = new ControlP5(this);
  
  s = cp5.addSlider2D("direction")
         .setPosition(50,50)
         .setSize(100,100)
         .setMinMax(0,0,100,100)
         .setValue(50,50)
         //.disableCrosshair()
         ;
  //cp5 = new ControlP5(this);       
  sliderXVector = cp5.addSlider("X vector")
                    .setPosition(50,200)
                   .setSize(100,20)
                   .setMax(100)
                   .setValue(50)
                   ;
                   
    //cp5 = new ControlP5(this);       
  servoSlider = cp5.addSlider("Servo PWM")
                   .setPosition(55,250)
                   .setSize(100,20)
                   .setMax(1400)
                   .setValue(700)
                   .setScrollSensitivity(0.01)
                   .setDecimalPrecision(0)
                   ;   
                   
   cp5.getController("Servo PWM").getCaptionLabel().align(ControlP5.LEFT, ControlP5.TOP_OUTSIDE).setPaddingX(0);
   
   
   button_PWM = cp5.addButton("send")
                   .setValue(0)
                   .setPosition(160,250)
                   .setSize(40,20)
                   .activateBy(1)
                   ;         
                   

                   
   button_reset = cp5.addButton("reset")
                   .setValue(0)
                   .setPosition(160,275)
                   .setSize(40,20)
                   ;
   button_sendXY = cp5.addButton("send XY")
                   .setValue(0)
                   .setPosition(50,25)
                   .setSize(40,20)
                   ;
                   
button_PWM.onRelease(new CallbackListener() { // add the Callback Listener to the button 
    public void controlEvent(CallbackEvent theEvent) {
      // specify whatever you want to happen here
      //println("callback for startBarCode " + variableName);
      output = 'C' + str(0) + 'X' + str(int(defaultX)) + 'Y' + str(int(defaultY)) + 'S' + outputServo + 'P' + outputPWM; port.write(output);
    }
  }
  );
      // create a toggle and change the default look to a (on/off) switch look
  cp5.addToggle("toggle")
     .setPosition(220,250)
     .setSize(50,20)
     .setValue(false)
     .setMode(ControlP5.SWITCH)
     ;
    
  cp5.addNumberbox("servo")
     .setPosition(20,250)
     .setSize(30,20)
     .setScrollSensitivity(0.5)
     .setDecimalPrecision(1)
     .setValue(0)
     .setRange(0,12)
     ;
     //cp5.getController("Servo PWM").getCaptionLabel().align(ControlP5.LEFT, ControlP5.TOP_OUTSIDE).setPaddingX(0);
            //mySlider.valueLabel().setVisible(false);
            // reposition the Label for controller 'slider'
  //cp5.getController("Servo PWM").getValueLabel().align(ControlP5.LEFT, ControlP5.BOTTOM_OUTSIDE).setPaddingX(0);
  //cp5.getController("Servo PWM").getValueLabel().setVisible(false);


  smooth();
  
    // List all the available serial ports:
  println(Serial.list());
    port = new Serial(this, Serial.list()[serialPort], 38400); //COM4
  
  // Fire a serialEvent() when when a linefeed comes in to the serial port.
//  port.bufferUntil(lf);
  
  //data[2] = byte(int(servoSlider.getValue()));
}



void draw() {
  background(0);
  pushMatrix();
  translate(250,100);
  noStroke();
  fill(50);
  //rect(0, -100, 400,200);
  ellipse(0,0,100,100);
  strokeWeight(1);
  //line(0,0,200, 0);
  stroke(255);
 
  x = s.getArrayValue()[0]-50;
  y = 100-s.getArrayValue()[1]-50;
  rawX = s.getArrayValue()[0];
  rawY = 100-s.getArrayValue()[1];
  alpha = atan2(y,x)*180/3.14;
  fill(255);
  //text(int(sqrt(pow(x,2)+pow(y,2))),0,150);
  //text(int(servoSlider.getValue()), 0, 100);

  
  line(0,0,x,-y);
  if (servoTestFlag == true) {
    defaultX = rawX;//s.getArrayValue()[0]-50;
    defaultY = rawY;//100-s.getArrayValue()[1]-50;
    //s.setValue(defaultX,defaultY);
  } else {
    x = s.getArrayValue()[0]-50;
    y = 100-s.getArrayValue()[1]-50;
  }
  
  
  translate(50,160);
  fill(col);
  ellipse(0,0,20,20);
  
  
  popMatrix();
  
   

    //ctrlFlag=0;
    if (servoTestFlag == true) {
      output = 'C' + str(ctrlFlag) + 'X' + str(int(defaultX)) + 'Y' + str(int(defaultY)) + 'S' + outputServo + 'P' + outputPWM;
      port.write(output);      
      
      print(ctrlFlag);
      print(" : ");
      print(str(int(defaultX)));
      print(" : ");
      print(str(int(defaultY)));
      print(" : ");
      print(byte(servoNum));
      print(" : ");
      println(int(servoSlider.getValue()));

    }
}

void servo(int theServoNum) {
  servoNum = theServoNum;
  //println("a numberbox event. setting background to "+theServoNum);
}

public void send() {
  //println("a button event from send: "+servoSlider.getValue());
  ctrlFlag = 1;
  outputServo = str(servoNum);
  outputPWM = str(servoSlider.getValue());
  //sendFlag = true;

  defaultX = int(rawX); //int(s.getArrayValue()[0]);
  defaultY = int(rawY);
  
  output = 'C' + str(int(ctrlFlag)) + 'X' + str(int(defaultX)) + 'Y' + str(int(defaultY)) + 'S' + outputServo + 'P' + outputPWM;
  port.write(output);
  
  //print(int(outputServo));
  //println(int(output));
  print(ctrlFlag);
  
  print(" : ");
  print(str(int(defaultX)));
  print(" : ");
  print(str(int(defaultX)));
  print(" : ");
  print(byte(servoNum));
  print(" : ");
  println(int(servoSlider.getValue()));
  //port.write(output);
  //output = "";
}

public void reset() {
  //println("a button event from reset: "+servoSlider.getValue());
  s.setValue(50,50);
  defaultX = int(rawX); //int(s.getArrayValue()[0]);
  defaultY = int(rawY);
  servoSlider.setValue(350);

  //sendFlag = true;
  //print(int(outputServo));
  //println(int(output));
  //port.write(output);
  //output = "";
}

public void button_sendXY() {
  //println("a button event from reset: "+servoSlider.getValue());
  //defaultX = int(rawX); //int(s.getArrayValue()[0]);
  //defaultY = int(rawY);
  //sliderXVector.setValue(50);
   // float rawX = s.getArrayValue()[0];
 // float rawY = 100-s.getArrayValue()[1];
  //sendFlag = true;
  //print(int(outputServo));
  //println(int(output));
  //port.write(output);
  //output = "";
}


void toggle(boolean theFlag) {
  if(theFlag==true) {
    println("toggle event true");
    col = color(0,200,50);
  } else {
    println("toggle event false");
    col = color(200,0,0);
  }
  servoTestFlag = theFlag;
}
