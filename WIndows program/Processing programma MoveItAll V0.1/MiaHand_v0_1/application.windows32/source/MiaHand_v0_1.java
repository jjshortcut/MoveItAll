import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import controlP5.*; 
import processing.serial.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class MiaHand_v0_1 extends PApplet {

/**
 * Simple Write. 
 * 
 * Check if the mouse is over a rectangle and writes the status to the serial port. 
 * This example works with the Wiring / Arduino program that follows below.
 */




ControlP5 cp5;

Serial myPort;  // Create object from Serial class
int val;        // Data received from the serial port
int c1,c2;

public void setup() 
{
  // I know that the first port in the serial list on my mac
  // is always my  FTDI adaptor, so I open Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 9600);
  myPort.write('H');
  
  
  background(255);  // white background
  
  cp5 = new ControlP5(this);
  
  // create a new button with name 'buttonA'
  cp5.addButton("openButton")
     .setLabel("Open")
     .setValue(0)
     .setPosition(100,100)
     .setSize(200,19)
     ;
     
     // and add another 2 buttons
  cp5.addButton("closeButton")
     .setLabel("Close")
     .setValue(100)
     .setPosition(100,120)
     .setSize(200,19)
     ;
  
}


public void draw() {
  
    //myPort.write('H');              // send an H to indicate mouse is over square
}

// function colorA will receive changes from 
// controller with name colorA
public void openButton(int theValue) {
  println("a button event from colorA: "+theValue);
  c1 = c2;
  c2 = color(0,160,100);
  myPort.write('H');              // send an H to indicate mouse is over square
}

// function colorB will receive changes from 
// controller with name colorB
public void closeButton(int theValue) {
  println("a button event from colorB: "+theValue);
  c1 = c2;
  c2 = color(150,0,0);
  myPort.write('L');              // send an H to indicate mouse is over square
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
  public void settings() {  size(600, 600); }
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "--present", "--window-color=#666666", "--stop-color=#cccccc", "MiaHand_v0_1" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
