
import processing.serial.*;


// Initialize text writing
PrintWriter dataLog;

// Fonts used
PFont font_num;
PFont font_letter;

// Create object from Serial class
Serial myPort; 

// Create array to hold data
int[] data = new int[2];
int serialCount = 0;

// Connection state
boolean connection = false;

// Data log state
//boolean dataLogStatus = false;

// Display variables
int speed;
int tach;
int fuel;
//int tempReading;
String date;
//float temp = 275;
String rate = "---";
String comm = "All Okay";

//Window size
int w = 3*screen.width/4;
int h = screen.height/2;


String file1 = "C:/ArduinoLog.txt";
int logDelay = 1000; //ms delay
String sData;
int goLog = 0;
String logStatus = "Off";
int tachGraph[];
int xPos = 200;

void setup() 
{
  size(w, h);
  background(209);
  smooth();
  
  font_num = loadFont("Helvetica-Bold-74.vlw");
  font_letter = loadFont("Helvetica-18.vlw");
 
  // Connect to serial port
  String portName = "COM4";
  myPort = new Serial(this, portName, 57600);
  
  // Dont do anything until a linefeed is recieved
  myPort.bufferUntil('\n');
  
}

float tachTop = h*.2;
float tachBottom = h*.05;
float speedTop = h*.5;
float speedBottom = h*.3;
  
float tachBytePrev = h*.05;
float speedBytePrev = h*.3;

void draw()
{
  // Draw sidebar stuff
  textFont(font_letter);
  fill(255);
  noStroke();
  // White bars on left and right
  rect(0, 0, 175, h);
  rect(w-175, 0, 175, h);
  fill(75);
  text("Baja WiDash", 25, 25);

  
  stroke(225);
  strokeWeight(7);
  strokeCap(PROJECT);
  line(25, 32, 130, 32);
  
  text("Logging: " + logStatus, 25, 155);
   
  fill(255);
  noStroke();
  rect(5, 60, 100, 20);
  fill(75);
  text("tx rate:\t" + rate, 25, 75);
  
  fill(255);
  noStroke();
  rect(5, 85, 100, 20);
  fill(75);
  text("fuel level:\t" , 25, 115);
  textFont(font_num);
  // Value that changes fuel indicator from green to red
  if (fuel > 2000){
    fill(255, 0, 0);
  }
  else if (fuel <= 2000){
    fill(0, 255, 100);
  }
  text("*", 105, 150);
  
/*
  fill(255);
  noStroke();
  rect(20, 140, 130, 20);
  fill(75);
  textFont(font_letter);
  text("engine: ", 25, 155);
  // Value that changes tempersture readout color to red
  if (temp >= 82){
    fill(255, 0, 0);
  }
  else {
    fill(75);
  }
  // Convert raw value to fahrenheit
  temp = (((((tempReading * 3.3) / 1024.0) - 0.5) * 100) * (9.0 / 5.0)) + 32.0 ;
  // Convert float to int (remove decimals)
  int tempOut = int(temp);
  text(tempOut + "°F", 94, 156);

*/
  
  // If no initial connection has been made, say so in the notification center (bottom left)
  if (!connection) {
    textFont(font_letter);
    fill(0, 82, 185);
    text("establish connection!", w-130, h-65, 100, 200);
  }
  
  textSize(12);
  fill(34,127,255);
  text("  40 MPH --", 112, h - speedTop);
  text("   0 MPH --", 115, h - speedBottom);
  fill(127,34,255);
  text("4000 RPM --", 106, h - tachTop);
  text("   0 RPM --", 115, h - tachBottom);
  
  // Draw speed rectangle and display speed readout
  textFont(font_num);
  fill(0,0,0);
  rect(w-675, 25, 250, 125);
  fill(255);
  text(tach, w-650, 145);
  
  // Draw tach rectangle and display tach readout
  fill(0,0,0);
  rect(w-400, 25, 125, 125);
  fill(255);
  text(speed, w-375, 145);
    
  // Label the displays
  fill(255);
  textFont(font_letter);
  text("Tach", 385, 75);
  text("Speed", 665, 75);
 
 
 /*
  // When the distress signal is recieved, fuel and speed are set to 0
  // This is used to determine when to display the distress signal
  if (tach == 0 & speed == 0 & connection){
    fill(255,0,0);
    rect(w-565, h-150, 175, 75);
    fill(255);
    text("!", 345, 230);
    
    // Notification (bottom left)
    textFont(font_letter);
    fill(0, 82, 185);
    text("alert the crew!", w-130, h-65, 100, 200);
  }
  else{
    fill(209);
    rect(w-565, h-150, 175, 75);
  }
*/

}

void serialEvent(Serial myport){
  connection = true;
  // Read all serial data until linefeed
  String inString = myPort.readStringUntil('\n');
  
  // Get rid of the excess spaces
  inString = trim(inString);
  
  // Save the data to a text file if logging has been initialized
  //println(inString);
  /*
  if (dataLogStatus) {
    dataLog.println(inString);
  }
  */
  // Separate the serial data by spaces into the respective data
  int data[] = int(split(inString, ' '));
  //speed = ((data[0] - 200) / 25);
  speed = ((data[0]));
  tach = (data[1]*9 - 3*data[1]/2 - 75);
  fuel = data[2];
  //write to log file
  sData =  "\r\n" + tach + "\t" + speed;
  if (goLog != 0) {
    logData(file1,sData,true);
  }
  
  //being graph
  

  float tachByte = float(tach);
  float speedByte = float(speed);
  //inByte = map(inByte, 0, 1023, 500, 5000);
  tachByte = map(tachByte, 500, 4000, tachBottom, tachTop); 
  speedByte = map(speedByte, 0, 40, speedBottom, speedTop);
  // draw the line:
  stroke(34,127,255);
  line(xPos-5, height - speedBytePrev, xPos, height - speedByte);
  //textSize(12);
  //fill(34,127,255);
  //text("*", xPos, height - speedByte);
  stroke(127,34,255);
  line(xPos-5, height - tachBytePrev, xPos, height - tachByte);
  //fill(127,34,255);
  //text("*", xPos, height - tachByte);
  // at the edge of the screen, go back to the beginning:
  if (xPos >= w - 175) {
    xPos = 200;
    //fill(225);
    //rect(150,height - speedTop,w-175,height-tachBottom);
    background(209);
  }
  else {  // increment the horizontal position:
    xPos = xPos + 5;
    tachBytePrev = tachByte;
    speedBytePrev = speedByte;
  }
  //end graph
  
}

void logData( String fileName, String newData, boolean appendData)
 {
    BufferedWriter bw=null;
    try { //try to open the file
    FileWriter fw = new FileWriter(fileName, appendData);
    bw = new BufferedWriter(fw);
    bw.write(newData);// + System.getProperty("line.separator"));
    } catch (IOException e) {
    } finally {
    if (bw != null){ //if file was opened try to close
        try {
        bw.close();
        } catch (IOException e) {}
    }
    }
}
 
String getDateTime()
{
      SimpleDateFormat df = new SimpleDateFormat("dd/MM/yyyy hh:mm:ss");
      Calendar date = Calendar.getInstance(); // the current date and time
      return(df.format(date.getTime()));
}

void keyPressed(){
  // a, b, and c are used to set the transmission rate
  if (key == 'a'){
    myPort.write(key);
    rate = "high";
  }
  else if (key == 'b'){
    myPort.write(key);
    rate = "mid";
  }
  else if (key == 'c'){
    myPort.write(key);
    rate = "low";
  }
  else if (key == 'l') {
    if (goLog == 0) {
      logData(file1,"\r\n\r\nTach \tSpeed \t" + getDateTime(),true);
      goLog = 1;
      logStatus = "On";
    }
    else {
     goLog = 0; 
     logStatus = "Off";
    }
  }
 
  
  // Simple communication system through LEDs
  else if (key == 'd'){
    myPort.write(key);
  }
  
  else if (key == 'e'){
    myPort.write(key);
  }
  else if (key == 'f'){
    myPort.write(key);
  }
  
  else if (key == 'g'){
    myPort.write(key);
  }
  else if (key == 'h'){
    myPort.write(key);
  }
  else if (key == 'i'){
    myPort.write(key);
  }
  

/*  
  // Create a data file named by the month /(:) day _ hour(24) . min
  else if (key == 'l'){
    dataLogStatus = true;
    date = month() + ":" + day() + "_" + hour() + "." + minute() + ".txt";
    dataLog = createWriter(date);
  }
  
  // Finalize the data file
  else if (key == 's'){
    //dataLog.flush();
    dataLog.close();
    exit();
  }
*/
}
