# Introduction
A procedural planet generator using perlin noise and a tesselated icosohedron for the sphere model.
The aim was to produce a convincing real-time render of a procedurally generated planet, including the surface and atmosphere.

# Building
The project will build under VS2015, it is currently windows only.

Build prerequisites are:
* [GLM ](http://glm.g-truc.net)
* [GLFW](http://www.glfw.org/)
* [GLEW](http://glew.sourceforge.net/)

The build process expects the dependencies in a /libraires subfolder of the location of the $(VisialStudioDir) macro.

# User Guide
-Move mouse to orbit.
-Scrool wheel zooms.
-Pressing _R_ will regenerate the planet with a random seed.
-Pressing _escape_ will cause the program to exit.

# Planned Additions
-Increased portion of parameters procedurally set (eg colours, height boundaries, etc.)
-Clouds.
