// Class for ball 
class Ball {
public:
    //datatype is GLfloat as we are implementing our GUI in Opengl
    GLfloat xpos, ypos;
    GLfloat xvel, yvel;
    GLfloat radius;
    GLfloat r, g, b;
};

//Class for  Paddle
class Paddle {
public:
    GLfloat xpos, ypos;
    GLfloat width, height;
    GLfloat r, g, b;
};

//Class for Brick
class Brick {
public:
    GLfloat xpos, ypos;
    GLfloat width, height;
    GLfloat r, g, b;
    GLint health;
    GLint value;
};
