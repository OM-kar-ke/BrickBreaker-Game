

#include "BrickBreaker.h"

using namespace std;

void recomputeFrame(int value);

BrickBreaker::BrickBreaker() {

    gameState = BrickBreaker::INIT;
    
}


void BrickBreaker::display(void) {

    // Clear color buffer with clearing/background color (i.e. set the background color if it is 	 initialised previously)
    glClear(GL_COLOR_BUFFER_BIT);

    // Set OpenGL for 2D drawing
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    //Mapping the clipping area(i.e. opengl coordinates) to the viewport(i.e. window coordinates)
    //But here the range for y is inverted so the origin(0,0) can be the top-left corner of the 	window.
    glOrtho(0.0f, WINWIDTH, WINHEIGHT, 0.0f, 0.0f, 1.0f);

    
    // Draw background
    drawBackground();

    // Select which state of the game to display
    switch (gameState) {
        case INIT:
            // Init values
            init();
            break;
            
            
        case Gameplay:
            // Draw the game
            drawGame();
            // If no balls, player loses the game
            if (balls.size() <= 0 && lifesCount > 0) {
            
                newBall();
                reward = 100;
                
            }
            
            else if (balls.size() <= 0) {
                
		gameover(score);
                
     
            }
            
            //if level 1 bricks are breaked spawn level 2 bricks.
            if (bricks.size() <= 0 && level <= 2) {
            
                level++;
                initBricks();
                
            } 
            // If no bricks, player wins the level
            else if (bricks.size() <= 0) {
            
                won();
                
            }
            break;
            
        
        default:
            break;
    }
    
    glutTimerFunc(TIMER, recomputeFrame, 0);
    
    glutSwapBuffers();
    
}



void recomputeFrame(int value) {

	glutPostRedisplay();
	
}

void BrickBreaker::init(void) {

    // Reset game statistics
    score = 0;
    level = 1;
    reward = 100;
    lifesCount = 3;
    
    // Remove all balls from the set/array of Balls
    balls.clear();
    
    // Remove all bricks from the set/array of Bricks
    bricks.clear();
    
    // Init bricks
    initBricks();
    
    // Add ball and paddle
    initPaddle();
    newBall();
    
    // Start game play, set gamestate to gameplay
    gameState = BrickBreaker::Gameplay;
    
}

void BrickBreaker::drawBackground(void) {


    glBegin(GL_QUADS);
	    // Bottom color
	    //glColor3f(1.0f, 0.0f, 0.0f);
	    glColor3f(1.00,0.40,0.70);
	    glVertex2f(0, WINHEIGHT);
	    glVertex2f(WINWIDTH, WINHEIGHT);
	    
	    // Top color
	    //glColor3f(0.0f, 1.0f, 0.0f);
	    glColor3f(0.33,0.49,1.00);
	    glVertex2f(WINWIDTH, 0.0f);
	    glVertex2f(0.0f, 0.0f);
    glEnd();
    
}

void BrickBreaker::drawGame(void) {

    
    // Draw balls
    drawBalls();
    
    // Draw bricks
    drawBricks();
    
    // Draw paddle
    drawPaddle();
    
    // Draw game statistics (lifes, score)
    drawGameStats();
    

}

void BrickBreaker::newBall() {
    Ball b1;
   

    //coordinates of ball b1's centre
    b1.xpos = WINWIDTH / 2.0;
    b1.ypos = WINHEIGHT - 30.0f;
    
    //x and y components of ball b1's velocity
    b1.xvel = 0;
    b1.yvel = 0;
    
    //ball b1's radius
    b1.radius = BALL_RADIUS;
    
    //ball's color is random.
    b1.r = 0.4f + (float) rand() / (RAND_MAX);
    b1.g = 0.25f + (float) rand() / (RAND_MAX);
    b1.b = 0.4f + (float) rand() / (RAND_MAX);
    
    //append ball b1 to balls(vector)
    balls.push_back(b1);
    
}

void BrickBreaker::drawBalls(void) {

    for (std::vector<Ball>::iterator it = balls.begin(); it != balls.end(); ) {
    

        glBegin(GL_POLYGON);
        
		glColor3f(it->r, it->g, it->b);
		
		//drawing the ball by iterating thru theta = 0 to 2*pie.
		for(int j = 0; j < CIRCLE_SEGMENTS; j++) {
		
		    float const theta = 2.0f * 3.1415926f * (float)j / (float)CIRCLE_SEGMENTS;
		    float const x = it->radius * cosf(theta);
		    float const y = it->radius * sinf(theta);
		    glVertex2f(x + it->xpos, y + it->ypos);
		    
		}
		
        glEnd();
        
        // Set new position
        it->xpos += it->xvel;
        it->ypos += it->yvel;
        
        
        //1.Collision with left/right/top window sides
        
        //Collision with left and right window sides
        if ( (it->xpos <= (it->radius)) || (it ->xpos >= (WINWIDTH - it->radius)) ) {
        
            it->xvel *= -1;
            
        }
        
        //Collision with top window side
        if (it->ypos <= it->radius) {
        
            it->yvel *= -1;
            
        }
        
        //when ball collides with bottom side of the window instead of paddle then delete that ball
        if (it->ypos >= (WINHEIGHT - it->radius)) {
        
            it = balls.erase(it);
            lifesCount--;
            continue;
            
        }
        
        
        
        //2.Collission with the bricks
        
        for (std::vector<Brick>::iterator br = bricks.begin(); br != bricks.end();) {
        
            // Check collission between ball and vertical brick sides
            if (it->ypos >= br->ypos && it->ypos <= br->ypos + br->height) {
            
            
                // brick right edge and left point on ball
                if ((it->xpos - it->radius - br->xpos - br->width) <= BALL_VEL_X && (it->xpos - it->radius - br->xpos - br->width) >= 0) {
                
                    it->xvel *= -1;
                    br = hitBrick(br);
                    break;	//one ball can break one bricks at a time
                    
                }
                
                
                // brick left edge and right point on circle
                if ((it->xpos + it->radius - br->xpos) >= -BALL_VEL_X && (it->xpos + it->radius - br->xpos) <= 0) {
                
                    it->xvel *= -1;
                    br = hitBrick(br);
                    break;
                    
                }
                
            }
            
            
            // Check collission between circle and horizontal brick sides
            if (it->xpos >= br->xpos && it->xpos <= br->xpos + br->width) {
            
                // brick bottom edge and top point on circle
                if ((it->ypos - it->radius - br->ypos - br->height) <= BALL_VEL_Y && (it->ypos - it->radius - br->ypos - br->height) >= 0) {
                
                    it->yvel *= -1;
                    br = hitBrick(br);
                    break;
                    
                }
                
                
                // brick top edge and bottom point on circle
                if ((it->ypos + it->radius - br->ypos) >= -BALL_VEL_Y && (it->ypos + it->radius - br->ypos) <= 0) {
                
                    it->yvel *= -1;
                    br = hitBrick(br);
                    break;
                    
                }
                
            }
            
            
            
            
            // Check collission between ball and corners of brick
            
            //d is distance beteen ball's centre and brick's corner
            //v is magnitude of ball's velocity
            GLfloat d, v;
            
            
            
            v = pow(it->xvel, 2.0) + pow(it->yvel, 2.0);
            v = pow(v, 0.5);
            
            
            // Check collission with top left corner
            d = pow((it->xpos - br->xpos), 2.0) + pow((it->ypos - br->ypos), 2.0);
            d = pow(d, 0.5);
            if (d < it->radius + v) {
                it->xvel *= -1;
                it->yvel *= -1;
                br = hitBrick(br);
                continue;
            }

            // Check collission with top right corner
            d = pow((it->xpos - br->xpos - br->width), 2.0) + pow((it->ypos - br->ypos), 2.0);
            d = pow(d, 0.5);
            if (d < it->radius + v) {
                it->xvel *= -1;
                it->yvel *= -1;
                br = hitBrick(br);
                continue;
            }

            // Check collission with bottom left corner
            d = pow((it->xpos - br->xpos), 2.0) + pow((it->ypos - br->ypos - br->height), 2.0);
            d = pow(d, 0.5);
            if (d < it->radius + v) {
                it->xvel *= -1;
                it->yvel *= -1;
                br = hitBrick(br);
                continue;
            }
            
            // Check collission with bottom right corner
            d = pow((it->xpos - br->xpos - br->width), 2.0) + pow((it->ypos - br->ypos - br->height), 2.0);
            d = pow(d, 0.5);
            if (d < it->radius + v) {
                it->xvel *= -1;
                it->yvel *= -1;
                br = hitBrick(br);
                continue;
            }
            
            
            
            
            
            ++br;
            
            
        }
        
        
        
        //3.collision with paddle
        
        // Check collission between paddle's left part and bottom point on circle
        if (it->xpos >= paddle.xpos && it->xpos <= paddle.xpos + paddle.width/2.0) {
        
            if ((it->ypos + it->radius - paddle.ypos) >= -BALL_VEL_Y && (it->ypos + it->radius - paddle.ypos) <= 0) {
            
                it->yvel *= -1;
                //if the ball coming from left hits the paddle's left part then it should go to left again
                if (it->xvel > 0 ){
                	
                	it->xvel *= -1;	
                	
                }
                continue;
                
            }
            
        }
        
        // Check collission between paddle's right part and bottom point on circle
        else if (it->xpos >= paddle.xpos + paddle.width/2.0 && it->xpos <= paddle.xpos + paddle.width) {
        
            if ((it->ypos + it->radius - paddle.ypos) >= -BALL_VEL_Y && (it->ypos + it->radius - paddle.ypos) <= 0) {
            
                it->yvel *= -1;
                //if the ball coming from right hits the paddle's right part then it should go to right again
                if (it->xvel < 0 ){
                	
                	it->xvel *= -1;	
                	
                }
                continue;
                
            }
            
        }
        
        
        
        
        ++it; // next ball
    }
    
}

void BrickBreaker::initPaddle(void) {
	
    //paddle color
    paddle.r = 0.2f;
    paddle.g = 0.5f;
    paddle.b = 1.0f;
    
    //paddle dimensions
    paddle.width = 150.0f;
    paddle.height = 12.0f;
    
    //paddle's top-left vertex coordinates(for drawing)
    paddle.xpos = WINWIDTH / 2.0f - paddle.width / 2.0f;
    paddle.ypos = WINHEIGHT - 20.0f;	   //here there is 8.0f pixels distance is present between the bottom side of the window and the bottom side of the paddle
    
}



void BrickBreaker::drawPaddle() {

    // Make sure paddle is larger than 25px
    if (paddle.width < 25) {
        paddle.width = 25;
    }
    
    //color of the paddle
    glColor3f(paddle.r, paddle.g, paddle.b);
	
    //draw the paddle
    glRectf(paddle.xpos, paddle.ypos, paddle.xpos + paddle.width, paddle.ypos + paddle.height);

}









void BrickBreaker::initBricks(void) {

    if (level == 1)
        bricksLevel1();
        
    else if (level == 2)
        bricksLevel2();
        
}




void BrickBreaker::bricksLevel1(void) {

    Brick newBrick;
    
    //brick's height and width
    newBrick.width = BRICKWIDTH;
    newBrick.height = BRICKHEIGHT;             //45*16
    
    //set color and health for each brick
    for (int i = 0; i < WALLROWS; ++i) {
    
        for (int j = 0; j < WALLCOLS; ++j) {
        
            // Set stronger bricks having health 2
            if (i >= 3 && i <= 6 && j >= 3 && j <= 9) {
            
                newBrick.r = 1.0f;
                newBrick.g = 0.5f;           //red color
                newBrick.b = 0.5f;
                newBrick.health = 2;
                
            }
            
            //set normal bricks
            else {
            
                newBrick.r = 0.95f;
                newBrick.g = 0.95f;        //white color 
                newBrick.b = 0.95f;
                newBrick.health = 1;
                
            }
            
            //each brick's top-left vertex coordinates(for drawing)
            newBrick.xpos = WALLX + j * newBrick.width + j * WALLSPACE;
            newBrick.ypos = WALLY + i * newBrick.height + i * WALLSPACE;
            
            //append the brick to bricks(vector)
            bricks.push_back(newBrick);
            
        }
        
    }
    
}




void BrickBreaker::bricksLevel2(void) {

    Brick newBrick;

    //brick's height and width
    newBrick.width = BRICKWIDTH;
    newBrick.height = BRICKHEIGHT;         //45*16
    
    //set color and health for each brick
    for (int i = 0; i < WALLROWS; i++) {
    
        for (int j = 0; j < WALLCOLS; j++) {
        
            // Set stronger bricks
            if (i == 1 || i == WALLROWS - 2 || j == 1 || j == WALLCOLS - 2) {
            
                newBrick.r = 1.0f;
                newBrick.g = 0.5f;          //red color
                newBrick.b = 0.5f;
                newBrick.health = 2;
                
            }
            
            //Set normal bricks
            else {
            
                newBrick.r = 0.95f;
                newBrick.g = 0.95f;     //white color
                newBrick.b = 0.95f;
                newBrick.health = 1;
                
            }
            
            //each brick's top-left vertex coordinates(for drawing)
            newBrick.xpos = WALLX + j * newBrick.width + j * WALLSPACE;
            newBrick.ypos = WALLY + i * newBrick.height + i * WALLSPACE;
            
            //append the brick to bricks(vector)
            bricks.push_back(newBrick);
            
        }
        
    }
    
}

void BrickBreaker::drawBricks(void) {

    for (std::vector<Brick>::iterator it = bricks.begin(); it != bricks.end(); ++it) {
    
    	//color of the brick
        glColor3f(it->r, it->g, it->b);
        //draw the brick
        glRectf(it->xpos, it->ypos, it->xpos + it->width, it->ypos + it->height);
        
    }
    
}



template <typename Iterator>
Iterator BrickBreaker::hitBrick(Iterator brick) {

    score += reward;
    reward += 25;
    
    // Decrease brick health
    //if brick's health is 2 then change its color from red to white
    if (brick->health > 1) {
    
        brick->r = 0.95f;
        brick->g = 0.95f;
        brick->b = 0.95f;
        brick->health -= 1;
        return ++brick;
        
    } 
    
    else {
    
        return bricks.erase(brick);
        
    }
    
    
    
}



void BrickBreaker::gameover(int score) {

	// draw the line below score 
	glBegin(GL_QUADS);

		// glColor3f(0.23,0.16,0.33);
		glColor3f(0.0,0.0,0.0);
		glVertex2f(0, WINHEIGHT);
		glVertex2f(WINWIDTH, WINHEIGHT);

		glVertex2f(WINWIDTH, 0.0f);
		glVertex2f(0.0f, 0.0f);
	glEnd();
                
        //display text and display score
        
        glPushMatrix();
        // Print the text
	glColor3f(1.0f, 1.0f, 1.0);
	glRasterPos2f(WINWIDTH/2.0 - 100, WINHEIGHT/2.0);
	char buf[300], *p;
	p = buf;
	sprintf(buf, "YOU LOSE!!! YOUR SCORE:");
			    
	do glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *p); while(*(++p));
			    
	// Print the score
	p = buf;
	sprintf(buf, "%d", score);
	glColor3f(1.0f, 1.0f, 1.0);
	glRasterPos2f(WINWIDTH/2.0 + 110, WINHEIGHT/2.0);
	do glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *p); while(*(++p));
			    
	glPopMatrix();
	


}



void BrickBreaker::won(void) {

	// draw different background for won 
	glBegin(GL_QUADS);

		glColor3f(0.23,0.16,0.33);
		glVertex2f(0, WINHEIGHT);
		glVertex2f(WINWIDTH, WINHEIGHT);

		glVertex2f(WINWIDTH, 0.0f);
		glVertex2f(0.0f, 0.0f);
	glEnd();
                
        //display text and display score
        
        glPushMatrix();
        // Print the text
	glColor3f(1.0f, 1.0f, 1.0);
	glRasterPos2f(WINWIDTH/2.0 - 100, WINHEIGHT/2.0);
	char buf[300], *p;
	p = buf;
	sprintf(buf, "YOU WON!!! ALL LEVELS COMPLETED");
			    
	do glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *p); while(*(++p));
				    
	glPopMatrix();
	


}




void BrickBreaker::drawGameStats(void) {
    glBegin(GL_LINES);
    // Bottom right (red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(20.0f, 30.0f);
    glVertex2f(WINWIDTH - 20.0f, 30.0f);
    glEnd();
    
    float offset = 25.0f;
    for (int i = 0; i < lifesCount && i < 10; ++i) {
        drawLife(35 + offset * i, 15);
    }
    
    drawScore();
}



void BrickBreaker::drawLife(float x, float y) {
    // Scale the heart symbol
    float const scale = 0.5f;
    
    // Heart symbol equations from Walfram Mathworld: http://mathworld.wolfram.com/HeartCurve.html
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 0.2f, 0.2f);
    for(int j = 0; j < CIRCLE_SEGMENTS; j++) {
        float const theta = 2.0f * 3.1415926f * (float)j / (float)CIRCLE_SEGMENTS;
        float const xx = scale * 16.0f * sinf(theta) * sinf(theta) * sinf(theta);
        float const yy = -1 * scale * (13.0f * cosf(theta) - 5.0f * cosf(2.0f * theta) - 2 * cosf(3.0f * theta) - cosf(4.0f * theta));
        glVertex2f(x + xx, y + yy);
    }
    glEnd();
}





void BrickBreaker::drawScore(void) {
    glPushMatrix();
    // Write score word
 
    glColor3f(0.0f, 0.0f, 0.0);
    glRasterPos2f(WINWIDTH - 120, 20);
    char buf1[300], *p;
    p = buf1;
    sprintf(buf1, "Score : ");
    do glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *p); while(*(++p));

    // Print the score
    p = buf1;
    sprintf(buf1, "           %d", score);
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(WINWIDTH - 120, 20);
    do glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *p); while(*(++p));
    
    
    
    glColor3f(0.0f, 0.0f, 0.0);
    glRasterPos2f(WINWIDTH/2.0, 20);
    char buf2[300], *q;
    q = buf2;
    sprintf(buf2, "Level : ");
    do glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *q); while(*(++q));
    // Print the score
    q = buf2;
    sprintf(buf2, "      %d", level);
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(WINWIDTH/2.0 + 20, 20);
    do glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *q); while(*(++q));
    
    
    
    
    
    
    glPopMatrix();
}





void BrickBreaker::keyStroke(unsigned char key, int x, int y) {

    switch (key) {
    
        case 'q': // Exit
            exit(0);
            break;
            
        case 'n': // New game
            init();
            break;
            
        case 'h':
        
            lifesCount++;
            break;
        
        case 'z':
        
            lifesCount--;
            break;
            
        case 27: // Esc button
            exit(0);
            break;
            
        case ' ':
        	balls[0].xvel = -BALL_VEL_X;
        	balls[0].yvel = -BALL_VEL_Y;
        	break;
        	
        default:
            break;
            
    }
    
}



void BrickBreaker::specialKeyPos(int key, int x, int y) {

    switch(key)
	{
	case GLUT_KEY_LEFT:
            if (paddle.xpos > 0) {
            
                paddle.xpos -= 4*BALL_VEL_X;
  
            }
            
            break;
            
        case GLUT_KEY_RIGHT:
            if (paddle.xpos + paddle.width < WINWIDTH) {
            
                paddle.xpos += 4*BALL_VEL_X;

            }
            
            break;
        
        default:
            break;
            
    }
    
}
