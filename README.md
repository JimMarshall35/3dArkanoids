A work in progress 3d arkanoids game.
It has a level editor which is a separate process that connects to the game via GRPC

Has a generalised framework for serializing properties in classes and exposing them to the editor. To be worked on more so i can use in other games.

possible mechanics will include:
- ramps that launches the ball upwards
- a "Jump" powerup that lets you jump the ball - the jump is implemented
- blocks that, if lined up by knocking down the ones below will cause an explosion
- classic arkanoids powerups
- different shaped and sized bricks
- classic arkanoids bricks like the gold and silver ones
- enemies
- turret blocks that shoot at the bat and can kill it. You could have these on top of stacks of other blocks
- a line that shows the trajectory of the ball when it's going to hit the bat (showing where it will got in real time) - implemented
- a powerup that flips the entire play-field upside down, possibly resulting in a different or weird board
- stuff around the outside of the board, better rendering engine with textures, more and better lighting
- sound effects

work in progress.

VS 2019
