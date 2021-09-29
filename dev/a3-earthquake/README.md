# My solution to Assignment 3
# CSCI4611 Assignment 3 Earthquake
### Eric Hwang

# Description of design decisions

## Overview

The goal of this assignment is to working with data from NASA and the USGS to visualize on a globe the locations where earthquakes happened between 1905 and 2007.

### Flat map
In Earth::Init() function, vertices, indices for earth mesh is set. Earth::LatLongToPlane() function is used to help convert latitude/lonitude to (x, y z) point. The conversion is done using GfxMath::ToRadians() method. x value of xy plane is ranged from -pi to pi, y is ranging from -pi/2 to pi/2, hence, range of -180 to 180 and -90 to 90 is used for latitude and lonitude before the conversion. Similarily, Earth::LatLongToSphere() functino is helping convert latitude and longitude to 3D cartesian coordinates of the corresponding point. 

### Earth::Updae_global()
The functoin is used to update the value of globe_btn, and set vertices and normals for globe sphere. The normals for the lighting on the globe is calculated using formulas x = cos(lat)sus(lon), y = sin(lat), z = cos(lat)cos(lon).
<hr>

###  Earthquake markers
The size of the earthquake markers differs frome the magnitude of each earthquake. The bigger the magnitude is the larger the size of the marker. The formula used to calculate the sizes is : (magnitude/100 * (magnitude/min_mag)) - (magnitude/120). Each fraction of the formula is correlated to the value of the magnitude of the earthquake, hence, the size would shift based on the value of the magnitude. The color of the marker also based on the value of the magnitude. Concept of normalization is utilized here to help convert the range to (0, 1) to use in Color() function. Larger earth quake get the higher r-value and low g,b-value, thus, tend to have red like color while small earth quakes have blue like colors.

### rotate_angle for animating the globe
The rotation angle for globe animation is increasing by 0.01. The speed of animation is appropriate but is open to change anytime as a matter of convenience. 

# CSci-4611 Assignment 3:  Earthquake

## Some notes on the mingfx::Mesh class

### When you use a Mesh

:x: Instead of using: 

```cpp
my_mesh_.vertex()
my_mesh_.normal()
my_mesh_.color()
my_mesh_.tex_coords()
my_mesh_.triangle_vertices()
```

:heavy_check_mark: Use:

```cpp
my_mesh_.vertexReadOnly()
my_mesh_.normalReadOnly()
my_mesh_.colorReadOnly()
my_mesh_.tex_coordsReadOnly()
my_mesh_.triangle_verticesReadOnly()
```

### Background info

Recently, in the **mingfx::Mesh** class, we changed some function names to make it clear that these functions can only be used as getters but not setters. These functions are:

```cpp
vertexReadOnly
normalReadOnly
colorReadOnly
tex_coordsReadOnly
triangle_verticesReadOnly
```

These changes may have not been updated on the MinGfx API documentation website, so if you look at the [mingfx::Mesh Class Reference webpage](https://ivlab.github.io/MinGfx/classmingfx_1_1_mesh.html), you may still see these functions listed without the `ReadOnly` part in their names.
