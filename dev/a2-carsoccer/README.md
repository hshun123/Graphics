# My solution to Assignment 2
# CSCI4611 Assignment 2 Car Soccer
### Eric Hwang

# Description of design decisions

## Overview

The goal of this assignment is to using the MinGfx API to work with 3D points and vectors in c++ to implement the simple 3D interactive game based on the 2015 game Rocket League.

## Main implementations

### Ball class
The ball class is the class for soccer ball used in the game. The class includes member variables to set the position, radius and the velocity of the ball. The class also includes member functions to get and set the values for those variables. set_velocity() function is the one to set the new velocity for the ball. velocity() function returns the current velocity of the ball. Inside the Reset() function, velocity is initialized with random values in the corresponding range to make the ball move through the air in a random velocity.   

### Car class
The car class is the class for car used in the game. The class includes member variables to set the position, velocity, collision radius and the theta angle of the car. The class also includes member functions to get and set the values for those variables. set_velocity() function is the one to set the new velocity for the car. velocity() function returns the current velocity of the car. set_theta() function is to set the angle of the car in XZ plane.theta() functions returns the current angle of the car. The velocity and theta is initialized in Reset() function.
<hr>

###  Drawing goals and boundary of the playing field

The goals and boundary are draw in DrawUsing OpenGL() function. By connecting points, drawing vertical and row lines to implement the net of the goals. Similarly, by connecting points based on given size of the field, the box boundary is drawn using DrawLines function.

### Ball bouncing

Detecting when the ball hits the walls or ceiling by check the position of the ball. If the distance of ball and the wall/ground/ceiling is smaller than the radius of the ball, update the position of the ball to prevent penetrating the wall, and update the velocity to decrease the speed of the ball.

### Collision between car and ball

Detecting when the ball and the car make the collision. When the distance of both objects is smaller than the sum of their radius, to prevent interaction, update the position of the ball. Then, compute the relative velocity and normal vector of the objects to help update the velocity of the ball.

### Car's motion

For each frame, the theta is updated by timeStep and the direction values. And, using Sine and Cosine value of the theta to make the car circular movement based on Left/Right joystick directions. Speed of the car is updated by constant factors and timeStep to have the proportional change. The velocity of the car is increased by the speed, and the position is changed by velocity*dt.  
