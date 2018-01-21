/**
 * Simple Write. 
 * 
 * Check if the mouse is over a rectangle and writes the status to the serial port. 
 * This example works with the Wiring / Arduino program that follows below.
 */

import controlP5.*;
import processing.serial.*;

ControlP5 cp5;

int buttonsizey = 30;
int buttonsizex = 50;
Textarea myTextarea;


Serial myPort;  // Create object from Serial class
int angle;        // holds the angle

void setup() 
{
  // I know that the first port in the serial list on my mac
  // is always my  FTDI adaptor, so I open Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 9600);
  
  size(600, 600);
  background(255);  // white background
  PFont font = createFont("arial",20);
  
  cp5 = new ControlP5(this);
  
      // create a new button with name 'buttonA'
      cp5.addButton("openButton")
     .setLabel("Open")
     .setValue(0)
     .setPosition(100,100)
     .setSize(buttonsizex,buttonsizey)
     ;
     
     // and add another 2 buttons
      cp5.addButton("closeButton")
     .setLabel("Close")
     .setValue(100)
     .setPosition(100,150)
     .setSize(buttonsizex,buttonsizey)
     ;
     
     cp5.addTextfield("input")
     .setPosition(20,100)
     .setSize(200,40)
     .setFont(font)
     .setFocus(true)
     .setColor(color(255,0,0))
     ;
     
     myTextarea = cp5.addTextarea("txt")
                  .setPosition(100,100)
                  //.setSize(200,200)
                  //.setFont(createFont("arial",12))
                  //.setLineHeight(14)
                  //.setColor(color(128))
                  //.setColorBackground(color(255,100))
                  //.setColorForeground(color(255,100));
                  ;
}

void draw() {
    //text(cp5.get(Textfield.class,"input").getText(), 360,130);
     myTextarea.setText("test");
    //myPort.write('H');              // send an H to indicate mouse is over square
}

// function colorA will receive changes from 
// controller with name colorA
public void openButton(int theValue) {
  println("Open, +");
  myPort.write('H');              // send +, open
}

// function colorB will receive changes from 
// controller with name colorB
public void closeButton(int theValue) {
  println("Close, -");
  myPort.write('L');              // send -, close
}

public void input(String theText) {
  // automatically receives results from controller input
  println("a textfield event for controller 'input' : "+theText);
}


/*
  // Wiring/Arduino code:
 // Read data from the serial and turn ON or OFF a light depending on the value
 
 char val; // Data received from the serial port
 int ledPin = 4; // Set the pin to digital I/O 4
 
 void setup() {
 pinMode(ledPin, OUTPUT); // Set pin as OUTPUT
 Serial.begin(9600); // Start serial communication at 9600 bps
 }
 
 void loop() {
 while (Serial.available()) { // If data is available to read,
 val = Serial.read(); // read it and store it in val
 }
 if (val == 'H') { // If H was received
 digitalWrite(ledPin, HIGH); // turn the LED on
 } else {
 digitalWrite(ledPin, LOW); // Otherwise turn it OFF
 }
 delay(100); // Wait 100 milliseconds for next reading
 }
 
 */