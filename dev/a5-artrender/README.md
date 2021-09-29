# My solution to Assignment 5
# CSCI4611 Assignment 5 Art render
### Eric Hwang

# Description of design decisions

## Overview

The goal of this assignment is to implement a GLSL shader that produces both realistic per-pixel lighting, ""toon shading, and a variety of other effects. User can rotate 3D models presented on the screen using mouse, and toggleing buttons "Phong Shading"
and "Artsy Shading." Different models can also be selected by toggling between model name buttons.

### Specular component
For this assignment, I used the "halfway vector" method rather than "reflection vector." Thus, H = l + e

### Silhouette deges
In outline.vert vertex shader program, checking each vertex to see if it lies on a sihouette edge or not by figuring out whether (nLeft dot e) and (nRight dot e) have different signs.
To check this, product of these two dot product is used. If the result is negative, it means they have different signs, then displace the vertex by thickness*n for gl_Position. 