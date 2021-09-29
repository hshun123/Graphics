/*
 * CSci-4611 Assignment #1 Text Rain
 * Name: Eric Hwang
 * x500: hwang241
 */


import processing.video.*;

// Global variables for handling video data and the input selection screen
String[] cameras;
Capture cam;
Movie mov;
PImage inputImage;
boolean inputMethodSelected = false;


//debugging
boolean debugGfx = false;
// Image of the flipped screen
PImage flippedImage;

String words = "Two roads diverged in a wood and I - I took the road less travels by";
// integer used to gap between each char of words
int gapX = 20;

// Threshold
float threshold = 128;

Raindrop[] raindrop = new Raindrop [words.length()];

class Raindrop {
  char drop;
  float xPos, yPos;
  int velocity;
  
  Raindrop(char drop){
    this.drop = drop;
    this.xPos = (float)(random(width));
    this.yPos = -(float)(random(100));
    this.velocity = 5;
  }
  
  // The function to handle the rain falling motion
  void falling(){
    color pixChar = flippedImage.get((int)(xPos), (int)(yPos + 2)); // yPos + 2 to make it look more smooth
    if (brightness(pixChar) < threshold && (yPos + 2) < height){
      
      if(yPos - velocity >= 0){
        yPos -= (velocity + 2) ;
        
      }else{
       // yPos = 0;
       yPos = -(int)random(280);
      }// end of handling
    }
    else{
      yPos += velocity;
    }
    // check if a char has reached the bottom
    if( yPos == height){
      yPos = -(int)random(80); // set to negative random when reaches the bottom
    }
  } // end of falling function
  
  // texting
  void texting(){
    fill(0, 0, 128); // set the font to blue
    text(drop, xPos, yPos);
    falling();
  }
}

// debugging function to activate the debug mode
// Thresholding the screen
// referenced from the worksheet
color debugging(color pix){
  
  if (green(pix) > threshold){
    pix = 255;
  }else{
    pix = 0;
  }
  
  return pix;
}

// flip function to mirror the screen for viewr
// referenced from the formula for flipping pixels
// flipping the image pixel by pixel
// worksheet
PImage flip(PImage img){
  PImage flipped = img.copy();
 
  for(int i = 0; i < img.width; i++){
    for(int j = 0; j < img.height; j++){
      int tmp = img.width - i - 1 + img.width*j;
      int cur = i + img.width*j;
      flipped.pixels[cur] = img.pixels[tmp];
    }
  }
  return flipped;
  
} // end of flip funciton

PFont font;

void setup() {
  size(1280, 720);  
  inputImage = createImage(width, height, RGB);
  font = createFont("Caladea-Regular-48.vlw",20); // create font
  
  float x = 0;
  for(int i = 0; i < words.length(); i++){
    Raindrop temp;
    // Randomly pick character within 3 index so that
    // make it random but still has chance to spell out the whole world
    if(i < words.length()- 2){
      temp = new Raindrop (words.charAt((int)(random(i , i + 2))));
    }else{
      temp = new Raindrop (words.charAt(i));
    }
    
    temp.xPos = x;
    raindrop[i] = temp;
    x += gapX; // Make a gap between each character to avoid overlapping
  } // end of for loop
}


void draw() {
  
  // When the program first starts, draw a menu of different options for which camera to use for input
  // The input method is selected by pressing a key 0-9 on the keyboard
  if (!inputMethodSelected) {
    cameras = Capture.list();
    int y=40;
    text("O: Offline mode, test with TextRainInput.mov movie file instead of live camera feed.", 20, y);
    y += 40; 
    for (int i = 0; i < min(9,cameras.length); i++) {
      text(i+1 + ": " + cameras[i], 20, y);
      y += 40;
    }
    return;
  }

  // This part of the draw loop gets called after the input selection screen, during normal execution of the program.
  
  // STEP 1.  Load an image, either from a movie file or from a live camera feed. Store the result in the inputImage variable
  if ((cam != null) && (cam.available())) {
    cam.read();
    inputImage.copy(cam, 0,0,cam.width,cam.height, 0,0,inputImage.width,inputImage.height);
  }
  else if ((mov != null) && (mov.available())) {
    mov.read();
    inputImage.copy(mov, 0,0,mov.width,mov.height, 0,0,inputImage.width,inputImage.height);
  }

  // This code draws the current camera/movie image to the screen
  set(0, 0, inputImage);

  // TODO: Fill in your code to implement the rest of TextRain here..
  
  // flip the screen
  flippedImage = flip(inputImage);
  
  flippedImage.filter(GRAY); // Display the video in gray scale
  // flippedImage.filter(BLUR); // Blur magic for smoothing
  set(0,0, flippedImage);  //set the flipped image
  
  // check debug mode
  if(debugGfx){
    // loop through pixels and change into binary mode
    for(int i = 0; i < flippedImage.pixels.length; i++){
      flippedImage.pixels[i] = color(debugging(flippedImage.pixels[i]));
    }
    set(0,0, flippedImage);
  }
 
  textFont(font);
  for (int i=0; i < words.length(); i++){
    raindrop[i].texting();
    raindrop[i].velocity = (int)(millis()%3 + 1); //(int)random(4); // randomly change the veloocity
  }
}



void keyPressed() {
  if (!inputMethodSelected) {
    
    // If we haven't yet selected the input method, then check for 0 to 9 keypresses to select from the input menu
    if ((key >= '0') && (key <= '9')) { 
      int input = key - '0';
      if (input == 0) {
        println("Offline mode selected.");
        mov = new Movie(this, "TextRainInput.mov");
        mov.loop();
        inputMethodSelected = true;
      }
      else if ((input >= 1) && (input <= 9)) {
        println("Camera " + input + " selected.");
        
        // The camera can be initialized directly using an element from the array returned by list():
        cam = new Capture(this, cameras[input-1]);
        cam.start();
        inputMethodSelected = true;
      }
    }
    return;
  }

  // This part of the keyPressed routine gets called after the input selection screen during normal execution of the program
  // TODO: Fill in your code to handle keypresses here..
  if (key == CODED) {
    if (keyCode == UP) {
      // up arrow key pressed
      threshold = min(threshold + 5, 255);
    }
    else if (keyCode == DOWN) {
      // down arrow key pressed
      threshold = max (threshold - 5, 0);
    }
  }
  else if (key == ' ') {
    // spacebar pressed
    debugGfx = !debugGfx;
  } 
}