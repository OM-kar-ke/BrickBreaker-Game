#include <vector>
#include <iostream>
#include <math.h>
#include <GL/glut.h>



#include "objects.h"      // Game-specific objects
#include "config.h"         // Game configurations

class BrickBreaker {

public:
    // Class Constructor/Destructor
	BrickBreaker();
	
    
    // Public functions (handle GLUT calls)
	void display(void);
	void init(void);
	void keyStroke(unsigned char key, int x, int y);
	void specialKeyPos(int key, int x, int y);
    
private:
    // Game statistics
    int score;
    int level;
    int reward;
    int lifesCount;
    
    // Possible ame mode
    enum State {INIT, Gameplay};
    BrickBreaker::State gameState;
    
    // Balls
    std::vector <Ball> balls;
    
    // Paddle
    Paddle paddle;
    
    // Bricks
    std::vector<Brick> bricks;
    
    // Private functions
    void drawBackground(void);
    void drawGame(void);
    void newBall();
    void drawBalls(void);
    void initPaddle(void);
    void drawPaddle(void);
    void initBricks(void);
    void bricksLevel1(void);
    void bricksLevel2(void);
    void drawBricks(void);
    template <typename Iterator>
    int wallCollision(Iterator it);
    template <typename Iterator>
    bool brickCollision(Iterator it, Iterator br);
    template <typename Iterator>
    Iterator hitBrick(Iterator brick);
    void drawLife(float x, float y);
    void drawGameStats(void);
    void drawScore(void);
    void gameover(int score);
    void won(void);
    
};
