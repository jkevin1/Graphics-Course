I got the coefficients for the Catmull-Rom Spline from:
http://www.lighthouse3d.com/tutorials/maths/catmull-rom-spline/
I didn't use any other external resources for this project.

It seems to function properly, but if you start from rest it briefly turns
the opposite direction so that the acceleration seems continuous between the
2 primary keyframes based on the 2 outer keyframes, which seems weird but
makes sense based on the algorithm.

Development platform:
Windows 10 Technical Preview 64bit
GT550M (OpenGL 4.4)
Visual Studio Express 2013 - Win32