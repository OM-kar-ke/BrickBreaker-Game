// Force Redraw timer
#define TIMER 16.6          // 60 frame per second

// Window settings
#define WINTITLE "BrickBreaker Game"
const int WINWIDTH  = 700;
const int WINHEIGHT = 500;


const float WALLX = 39.5f;		//39.5 used becos bricks should be present at equal distance 					from lhs and rhs of window screen.
const float WALLY = WALLX;  
                   
const int WALLSPACE = 3;                            // Space between bricks
const int WALLCOLS = 13;                            // Bricks columns count
const int WALLROWS = 10;                            // Bricks rows count



const int BRICKWIDTH = 45;
const int BRICKHEIGHT = 16;                       //45*16

// Pedal Settings
const float PEDALWIDTH = 150.0f;                   //150*12
const float PEDALHEIGHT = 12.0f;

// Ball settings
const float BALL_VEL_X = 4.0f;
const float BALL_VEL_Y = 3.0f;
const float BALL_RADIUS = 6.0f;
const int CIRCLE_SEGMENTS = 50; // used in drawing ball 
