# shapelandia
Drawing and animating shapes using SFML

Our first goal is to draw shapes as dictated within our `src/config` file.

Each line has a directive - Either:
  - **Window** - That describes the size of the window displayed.
  - **Font** - Which defines the font that should be used for text within the program.
  - **Circle** - Which defines a circle that shall be drawn and animated on the screen.
  - **Rectangle** - Which defines a rectangle that shall be drawn and animated on the screen.

Every frame, all the shapes shall be moved as according to the speeds defined.

Should any shapes collide with the edges of the screen, they should bounce off and have their movement updated accordingly.

Additionally, each shape will have its name displayed centered upon its self.

