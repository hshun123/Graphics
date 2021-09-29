# CSCI4611 Assignment 1 Text Rain
### Eric Hwang

# Description of design decisions

## Overview

The goal of this assignment is to re-implementing the famous interactive art installation, TextRain by Romy Achituv and Camile Utterback. This is an engaging video based digital installation that is in three permanent collections and has been exhibited dozens of times around the world.

## Main implementations

### Raindrop class
The Raindrop class is the class for text characters that are falling. The class includes local variables to set the x coordinates, y coordinates and the velocity of the raindrop. The class also includes falling() function and texting() function. Falling() function is the one to handle the movement of the raindrops. The function checks if the object is dark or not to decide whether a raindrop should land on the object or keep falling. Text() function set the color of the raindrops and draw them on the screen. Then the function calls falling() function to make the raindrops falling.  

<hr>

### debugging ()

debugging() function activate the debug mode. The function supports debugging and setting a good threshold by displaying the result of the threshold computation. It receives a color type parameter and checks the green scale value of the parameter to determine if a pixel is foreground or background. Pixels classified as foreground are set to pure black, pixels classified as background are set to pure white.

### flip ()

flip() function flips the viewing image so when the viewers look at the display it looks like they are looking in a mirror at themselves. filp() receives a PImage type parameter and make a copy of it, then it flips the image by change the order of pixels based on a symmetric matrix formula. The function is called in draw() function that the video image is displayed in flipped mode from the beginning.

### Picking characters to display at random
Inside setup() function, using a for-loop to pick characters from the chosen text. To make the rain looks random but remain a reasonable likelihood of catching a set of raindrops that spell out a whole word, The characters are selected randomly from 3 consecutive indices. That way, when the raindrops are falling, it looks like they are random, but since they are selected from 3 consecutive indices there is a reasonable likelihood of making a set of raindrops that consist a whole word from the text.
