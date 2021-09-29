# My solution to Assignment 6
# CSCI4611 Assignment 6 Harold
### Eric Hwang

# Description of design decisions

## Overview

The goal of this assignment is to implement portions of the paper, "Harold: A World Made of Drawings", presented by Cohen et al. User can set colors by clicking Crayon color, Sky color and Ground color buttons on the left top of the screen. The arrow keys and W, A, S and D keys are used to control the sight.


### Drawing in the sky
In the Sky class, Sky::AddSkyStroke() function takes the stroke2d_mesh which is to projected onto the sky.

### Editing the Ground
In the Ground class, using the equations described in Section 4.5 of the paper the gorund in response to the stroke drawn by the user is edited. 