antialias=0
change this to a 1 to enable RGSS antialiasing

I didnt use any resources other than the textbook, lectures, provided code, and project description to complete this project.
Everything seems to be working as intended.
I used the supplied code for guarenteed functionality. Most of my changes are either in Intersection.cpp or are called from there
(algorithm to calculate color) but I did make a few small changes elsewhere.

I decided to add antialiasing because the jagged edges were annoying, so I did do some external research for that but that wasn't
part of the project, I pretty much just used https://mediatech.aalto.fi/~samuli/publications/hasselgren2005cgf_paper.pdf  
I used "Rotated Grid Supersampling" (RGSS)
Basically, instead of spawning one ray in the center of each pixel, it spawns 4 rays per pixel in the following positions and averages the color
0X00
X000
000X
00X0
It will take approximately 4x as long to render but it does look a lot nicer! for comparison:
http://userpages.umbc.edu/~jkevin1/teapot.png
http://userpages.umbc.edu/~jkevin1/antialiased.png
