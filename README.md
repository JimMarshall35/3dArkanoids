Is going to be a mobile game, but being developed on windows to begin with.
Based on arkanoids except in 3d with multiple layers of blocks.
Editor functions as a separate process (A WPF c# app) and communicates with game via GRPC

Will become a test bed for some openCV code  I am writing which does head tracking in 3d space using dlib, with a view to ultimately produce something similar to this: 

https://www.youtube.com/watch?v=xjH8Q4xsKpo

My plan is that the OpenCV head tracking code will also run in a seperate process and communicate with the game through grpc
