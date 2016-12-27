/************************************************************************
File:			main.c
Description:	This is a C program that uses opengl and free glut to 
display a screen saver. It displays a blue G that can be turned into a 
star by pressing the morph button. By unpressing the morph button, it 
will turn back into a G. By pressing the sparkle button, a rotating 
sparkle that increases and decreases in size will follow the edge of the
G or star. By pressing the sparks button, sparks will come out of the 
centre of the sparkle. 

Bonus features: By pressing the fireworks button, a fireworks effect will
go all over the screen. By pressing the credits button, credits will come 
down from the top of the screen, and by unpressing it, they will go back
up. By pressing the crosshairs button, a crosshair will follow the cursor.

Author: Liam Gowan B00673126
Note: This program uses some ideas discussed from the course website. 
The specific information found in the color array was found at the 
following link:
http://prideout.net/archive/colors.php
*************************************************************************/

#include <gl\freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//dimensions for window
float windowHeight = 600;
float windowWidth = 400;

//used to store X and Y coords
typedef float XYCOORD[2];

//used to store quad x and y positions
typedef XYCOORD quad[4];

// Mouse variables
int   mousePressed = 0;
float mouseX = 0, mouseY = 0;

//Variables used for the scaling of the star
float scaleDelta = 0.0005f;
float scaleBy=1.0f;

//Variable used during rotation animation
GLfloat angle = 0;

//Variable used during majority of animation
float delta = .05f;

//2D array of 20 different colours
float colorArr[20][3] = {
	{1.0f,0.0f,0.0f},//red
	{ 0.5f,0.0f,0.0f },//dark red
	{ 0.0f,1.0f,0.0f },//green
	{ 0.0f,0.5f,0.0f },//dark green
	{ 0.0f,0.0f,1.0f },//blue
	{ 0.0f,0.0f,0.5f },//dark blue
	{ 1.0f,1.0f,0.0f },//yellow
	{ 1.0f,0.0f,1.0f },//purple
	{ 0.0f,1.0f,1.0f },//cyan
	{ 1.0f,1.0f,1.0f },//white
	{ 1.0f,0.5f,0.0f },//orange
	{ 0.5f,1.0f,0.0f },//greenish yellow
	{ 0.5f,1.0f,0.5f },//light green
	{1.0f,0.078f,0.576f},//deep pink
	{ 1.0f,0.843f,0.0f },//gold
	{ 1.0f,0.855f,0.725f },//peach
	{ 0.824f,0.412f,0.118f },//brown
	{ 0.498f,1.0f,0.831f },//aquamarine
	{ 0.482f,0.408f,0.933f },//purplish blue
	{ 0.333f,0.420f,0.184f }//dark olive green
};

//Coordinates for the 6 buttons for the top right corner
XYCOORD buttons[] = { {25,100}, {150,100}, {275,100}, {25,50}, {150,50 }, {275,50} };

// button width and height
float buttonWidth = 100;
float buttonHeight = 30;

//global states of buttons
int morphButState = 0;
int sparkleButState = 0;
int sparksButState = 0;
int fireworksButState = 0;
int creditButState = 0;
int crosshairButState = 0;

//Global X and Y position of the sparkle (set to start at the top left hand corner of G
GLfloat sparkleX = 100;
GLfloat sparkleY = 550;

//All spark X and Y locations
// spark 1 X and Y locations
float spark1XPos = 0;
float spark1YPos = 0;
// spark 2 X and Y locations
float spark2XPos = 0;
float spark2YPos = 0;
// spark 3 X and Y locations
float spark3XPos = 0;
float spark3YPos = 0;
// spark 4 X and Y locations
float spark4XPos = 0;
float spark4YPos = 0;
// spark 5 X and Y locations
float spark5XPos = 0;
float spark5YPos = 0;
// spark 6 X and Y locations
float spark6XPos = 0;
float spark6YPos = 0;
// spark 7 X and Y locations
float spark7XPos = 0;
float spark7YPos = 0;
// spark 8 X and Y locations
float spark8XPos = 0;
float spark8YPos = 0;
// spark 9 X and Y locations
float spark9XPos = 0;
float spark9YPos = 0;
// spark 10 X and Y locations
float spark10XPos = 0;
float spark10YPos = 0;
// spark 11 X and Y locations
float spark11XPos = 0;
float spark11YPos = 0;
// spark 12 X and Y locations
float spark12XPos = 0;
float spark12YPos = 0;

//variables for controlling the end location and color of every spark
//variables for controlling the end location and color for spark 1
int sparkEnd1X = 0;
int sparkEnd1Y = 0;
int colorIndex1 = 0;
float spark1R=0.0f;
float spark1G = 0.0f;
float spark1B = 0.0f;

//variables for controlling the end location and color for spark 2
int sparkEnd2X = 0;
int sparkEnd2Y = 0;
int colorIndex2 = 0;
float spark2R = 0.0f;
float spark2G = 0.0f;
float spark2B = 0.0f;

//variables for controlling the end location and color for spark 3
int sparkEnd3X = 0;
int sparkEnd3Y = 0;
int colorIndex3 = 0;
float spark3R = 0.0f;
float spark3G = 0.0f;
float spark3B = 0.0f;

//variables for controlling the end location and color for spark 4
int sparkEnd4X = 0;
int sparkEnd4Y = 0;
int colorIndex4 = 0;
float spark4R = 0.0f;
float spark4G = 0.0f;
float spark4B = 0.0f;

//variables for controlling the end location and color for spark 5
int sparkEnd5X = 0;
int sparkEnd5Y = 0;
int colorIndex5 = 0;
float spark5R = 0.0f;
float spark5G = 0.0f;
float spark5B = 0.0f;

//variables for controlling the end location and color for spark 6
int sparkEnd6X = 0;
int sparkEnd6Y = 0;
int colorIndex6 = 0;
float spark6R = 0.0f;
float spark6G = 0.0f;
float spark6B = 0.0f;

//variables for controlling the end location and color for spark 7
int sparkEnd7X = 0;
int sparkEnd7Y = 0;
int colorIndex7 = 0;
float spark7R = 0.0f;
float spark7G = 0.0f;
float spark7B = 0.0f;

//variables for controlling the end location and color for spark 8
int sparkEnd8X = 0;
int sparkEnd8Y = 0;
int colorIndex8 = 0;
float spark8R = 0.0f;
float spark8G = 0.0f;
float spark8B = 0.0f;

//variables for controlling the end location and color for spark 9
int sparkEnd9X = 0;
int sparkEnd9Y = 0;
int colorIndex9 = 0;
float spark9R = 0.0f;
float spark9G = 0.0f;
float spark9B = 0.0f;

//variables for controlling the end location and color for spark 10
int sparkEnd10X = 0;
int sparkEnd10Y = 0;
int colorIndex10 = 0;
float spark10R = 0.0f;
float spark10G = 0.0f;
float spark10B = 0.0f;

//variables for controlling the end location and color for spark 11
int sparkEnd11X = 0;
int sparkEnd11Y = 0;
int colorIndex11 = 0;
float spark11R = 0.0f;
float spark11G = 0.0f;
float spark11B = 0.0f;

//variables for controlling the end location and color for spark 12
int sparkEnd12X = 0;
int sparkEnd12Y = 0;
int colorIndex12 = 0;
float spark12R = 0.0f;
float spark12G = 0.0f;
float spark12B = 0.0f;

//variables to indicate which vertex the sparkle has already passed
int v1C = 0;
int v2C = 0;
int v3C = 0;
int v4C = 0;
int v5C = 0;
int v6C = 0;
int v7C = 0;
int v8C = 0;
int v9C = 0;
int v10C = 0;
int v11C = 0;
int v12C = 0;


//fireworks variables
float fireworkX = 0;
float fireworkY = 0;
float fireworksDelta = 0.05f;
float fireworkUpBy = 250;

//credits variables 
float creditY = 500;
float creditUpBy = 0;
float creditsDelta = 0.05f;

//The G and Star are composed of 5 different quads, which change shape.

//These are the coordinates of the X and Y positions for the quads that make up the G
quad  Gone = { { 100,550 },{ 300,550 },{300,500 },{ 100,500 } };
quad Gtwo = { {100,500},{150,500},{150, 300},{100,300} };
quad Gthree = { {100,300},{300,300},{300,250},{100,250} };
quad Gfour = { { 250,300 },{ 250,400 },{ 300,400 },{ 300,300 } };
quad Gfive = { {250, 400},{250, 350},{200,350},{200,400} };

//These are the coordinates that are actually drawn (the values later change, but are set to the values of G by default).
quad  interpQuad1 = { { 100,550 },{ 300,550 },{ 300,500 },{ 100,500 } };
quad interpQuad2 = { { 100,500 },{ 150,500 },{ 150, 300 },{ 100,300 } };
quad interpQuad3 = { { 100,300 },{ 300,300 },{ 300,250 },{ 100,250 } };
quad interpQuad4 = { { 250,300 },{250,400}, { 300,400 },{ 300,300 } };
quad interpQuad5 = { { 250, 400 },{ 250, 350 },{ 200,350 },{ 200,400 } };

//These are the coordinates of the X and Y positions for the quads that make up the G
quad SOne = { {200,600},{300,450},{200,250},{100,450} };
quad STwo = { {0,450},{100,450},{200,250},{200,250} };
quad SThree = { {200,250},{100,150},{100,350},{150,300} };
quad SFour = { {200,250},{300,350},{300,150},{300,150} };
quad SFive = { {400,450},{300,450},{200,250},{200,250} };

/************************************************************************
Function:		drawButtons
Description:	This function draws the buttons required. If user presses 
a button, it's color will change from red to purple, and back to red
if pressed again.
*************************************************************************/
void drawButtons(void) {

	// Loop through to draw all 4 buttons
	for (int i = 0; i<6; i++) {
		// Draws button shawdows first
		glBegin(GL_POLYGON);
		glColor3f(0.0, 0.0, 0.0);
		glVertex2f(buttons[i][0], buttons[i][1]);
		glVertex2f(buttons[i][0] + buttonWidth + 5, buttons[i][1]);
		glVertex2f(buttons[i][0] + buttonWidth + 5, buttons[i][1] - buttonHeight -5);
		glVertex2f(buttons[i][0], buttons[i][1] - buttonHeight - 5);
		glEnd();

		// Begin button drawing
		glBegin(GL_POLYGON);
		glColor3f(1, 0, 0);
		
		// Change morph button color if pressed
		if (morphButState && i == 0) {
			glColor3f(.5, 0, 1);
		}
		
		// Change sparkle button color if pressed
		if (sparkleButState && i == 1) {
			glColor3f(.5, 0, 1);
		}

		// If sparkle button is already pressed, and the sparks button has been pressed, change the color
		if (sparksButState && i == 2 && sparkleButState) {
			glColor3f(.5, 0, 1);
		}
		
		// change fireworks button color if pressed
		if (fireworksButState && i == 3) {
			glColor3f(.5, 0, 1);
		}

		//change credits button color if pressed
		if (creditButState && i == 4) {
			glColor3f(.5, 0, 1);
		}

		//change crosshair button color if pressed
		if (crosshairButState && i == 5) {
			glColor3f(.5, 0, 1);
		}

		// Draws every button
		glVertex2f(buttons[i][0], buttons[i][1]);
		glVertex2f(buttons[i][0] + buttonWidth, buttons[i][1]);
		glVertex2f(buttons[i][0] + buttonWidth, buttons[i][1] - buttonHeight);
		glVertex2f(buttons[i][0], buttons[i][1] - buttonHeight);
		glEnd();
		
	}
}

/************************************************************************
Function:		sparklePositionStarControl
Description:	This function will move the sparkle's position along the
edge of the star. It works by keeping track of which vertexes the sparkle
has passed and checking the current positions of the X and Y coordinates
of the sparkle, and then updating the location. It only works when the 
sparkle button is on, and when the morph button is on.
*************************************************************************/
void sparklePositionStarControl() {
	//ensures that sparkle and morph button are on
	if (sparkleButState && morphButState) {
		//Moves to top point
		if ((sparkleX <= SOne[0][0] || sparkleY <= SOne[0][1]) && v1C == 0 && v2C == 0 && v3C == 0
			&& v4C == 0 && v5C == 0 && v6C == 0
			&& v7C == 0 && v8C == 0 && v9C == 0
			&& v10C == 0 && v11C == 0 && v12C == 0) {
			if (sparkleX <= SOne[0][0])
				sparkleX += delta;
			if (sparkleY <= SOne[0][1])
				sparkleY += delta;
			if (sparkleX >= SOne[0][0] && sparkleY >= SOne[0][1])
				v1C = 1;
		}

		//Moves sparkle from top of star to vertex between top of star to top right point
		else if ((sparkleX != SOne[1][0] || sparkleY != SOne[1][1]) && v1C == 1 && v2C == 0 && v3C == 0
			&& v4C == 0 && v5C == 0 && v6C == 0
			&& v7C == 0 && v8C == 0 && v9C == 0
			&& v10C == 0 && v11C == 0 && v12C == 0) {
			if (sparkleX <= SOne[1][0])
				sparkleX += delta;
			if (sparkleY >= SOne[1][1])
				sparkleY += 1.5*-delta;
			if (sparkleX >= SOne[1][0] && sparkleY <= SOne[1][1])
				v2C = 1;
		}
		//Moves sparkle to top right point
		else if (sparkleX <= SFive[0][0] && v1C == 1 && v2C == 1 && v3C == 0
			&& v4C == 0 && v5C == 0 && v6C == 0
			&& v7C == 0 && v8C == 0 && v9C == 0
			&& v10C == 0 && v11C == 0 && v12C == 0) {
			if (sparkleX <= SFive[0][0])
				sparkleX += delta;
			if (sparkleX >= SFive[0][0])
				v3C = 1;
		}

		//Moves sparkle to vertex between top right point and bottom right point
		else if ((sparkleX >= SFour[1][0] || sparkleY >= SFour[1][1]) && v1C == 1 && v2C == 1 && v3C == 1
			&& v4C == 0 && v5C == 0 && v6C == 0
			&& v7C == 0 && v8C == 0 && v9C == 0
			&& v10C == 0 && v11C == 0 && v12C == 0) {
			if (sparkleX >= SFour[1][0])
				sparkleX += -delta;
			if (sparkleY >= SFour[1][1])
				sparkleY += -delta;
			if (sparkleX >= SFour[1][0] && sparkleY <= SFour[1][1])
				v4C = 1;
		}

		//Moves sparkle to bottom right point
		else if (sparkleY >= SFour[2][1] && v1C == 1 && v2C == 1 && v3C == 1
			&& v4C == 1 && v5C == 0 && v6C == 0
			&& v7C == 0 && v8C == 0 && v9C == 0
			&& v10C == 0 && v11C == 0 && v12C == 0) {
			if (sparkleY >= SFour[2][1])
				sparkleY += -delta;
			if (sparkleY <= SFour[2][1])
				v5C = 1;
		}

		//Moves sparkle to vertex between both bottom points
		else if ((sparkleX >= SFour[0][0] || sparkleY <= SFour[0][1]) && v1C == 1 && v2C == 1 && v3C == 1
			&& v4C == 1 && v5C == 1 && v6C == 0
			&& v7C == 0 && v8C == 0 && v9C == 0
			&& v10C == 0 && v11C == 0 && v12C == 0) {
			if (sparkleX >= SFour[0][0])
				sparkleX += -delta;
			if (sparkleY <= SFour[0][1])
				sparkleY += delta;
			if (sparkleX <= SFour[0][0] && sparkleY >= SFour[0][1])
				v6C = 1;
		}

		//Moves sparkle to left bottom point
		else if ((sparkleX >= SThree[1][0] || sparkleY >= SThree[1][1]) && v1C == 1 && v2C == 1 && v3C == 1
			&& v4C == 1 && v5C == 1 && v6C == 1
			&& v7C == 0 && v8C == 0 && v9C == 0
			&& v10C == 0 && v11C == 0 && v12C == 0) {
			if (sparkleX >= SThree[1][0])
				sparkleX += -delta;
			if (sparkleY >= SThree[1][1])
				sparkleY += -delta;
			if (sparkleX <= SThree[1][0] && sparkleY <= SThree[1][1])
				v7C = 1;
		}

		//Moves sparkle to vertex between left bottom point and left top point
		else if (sparkleY <= SThree[2][1] && v1C == 1 && v2C == 1 && v3C == 1
			&& v4C == 1 && v5C == 1 && v6C == 1
			&& v7C == 1 && v8C == 0 && v9C == 0
			&& v10C == 0 && v11C == 0 && v12C == 0) {
			if (sparkleY <= SThree[2][1])
				sparkleY += delta;
			if (sparkleY >= SThree[2][1])
				v8C = 1;
		}

		//Moves sparkle to top left point
		else if ((sparkleX >= STwo[0][0] || sparkleY <= STwo[0][1]) && v1C == 1 && v2C == 1 && v3C == 1
			&& v4C == 1 && v5C == 1 && v6C == 1
			&& v7C == 1 && v8C == 1 && v9C == 0
			&& v10C == 0 && v11C == 0 && v12C == 0) {
			if (sparkleX >= STwo[0][0])
				sparkleX += -delta;
			if (sparkleY <= STwo[0][1])
				sparkleY += delta;
			if (sparkleX <= STwo[0][0] && sparkleY >= STwo[0][1])
				v9C = 1;
		}

		//Moves sparkle to vertex between top left point and top point
		else if (sparkleX <= STwo[1][0] && v1C == 1 && v2C == 1 && v3C == 1
			&& v4C == 1 && v5C == 1 && v6C == 1
			&& v7C == 1 && v8C == 1 && v9C == 1
			&& v10C == 0 && v11C == 0 && v12C == 0) {
			if (sparkleX <= STwo[1][0])
				sparkleX += delta;
			if (sparkleX >= STwo[1][0])
				v10C = 1;
		}

		//move sparkle to top point
		else if ((sparkleX <= SOne[0][0] || sparkleY <= SOne[0][1]) && v1C == 1 && v2C == 1 && v3C == 1
			&& v4C == 1 && v5C == 1 && v6C == 1
			&& v7C == 1 && v8C == 1 && v9C == 1
			&& v10C == 1 && v11C == 0 && v12C == 0) {
			if (sparkleX <= SOne[0][0])
				sparkleX += delta;
			if (sparkleY <= SOne[0][1])
				sparkleY += 1.5*delta;
			if (sparkleX >= SOne[0][0] && sparkleY >= SOne[0][1])
				v11C = 1;
		}
		//resets all vertex markers
		else if (v11C == 1) {
			v1C = 1;
			v2C = 0;
			v3C = 0;
			v4C = 0;
			v5C = 0;
			v6C = 0;
			v7C = 0;
			v8C = 0;
			v9C = 0;
			v10C = 0;
			v11C = 0;
		}
	}


}

/************************************************************************
Function:		sparklePositionGControl
Description:	This function will move the sparkle's position along the
edge of the G. It works by keeping track of which vertexes the sparkle
has passed and checking the current positions of the X and Y coordinates
of the sparkle, and then updating the location. It only works when the
sparkle button is on, and when the morph button is off.
*************************************************************************/
void sparklePositionGControl() {
	//ensures sparkle button is on, and morph button is off
	if (sparkleButState && !morphButState) {
		//moves sparkle from top left corner to top right corner
		if (sparkleX <= 300 && v1C == 0 && v2C == 0 && v3C == 0
			&& v4C == 0 && v5C == 0 && v6C == 0
			&& v7C == 0 && v8C == 0 && v9C == 0
			&& v10C == 0 && v11C == 0 && v12C == 0) {
			sparkleX += delta;
			if (sparkleX >= 300)
				v1C = 1;
		}
		//moves sparkle to next vertex, which is 50 points down
		else if (sparkleY >= 500 && v1C == 1 && v2C == 0 && v3C == 0
			&& v4C == 0 && v5C == 0 && v6C == 0
			&& v7C == 0 && v8C == 0 && v9C == 0
			&& v10C == 0 && v11C == 0 && v12C == 0) {
			sparkleY += -delta;
			if (sparkleY <= 500)
				v2C = 1;
		}

		//moves sparkle to next vertex, which is 150 points left
		else if (sparkleX >= 150 && v1C == 1 && v2C == 1 && v3C == 0
			&& v4C == 0 && v5C == 0 && v6C == 0
			&& v7C == 0 && v8C == 0 && v9C == 0
			&& v10C == 0 && v11C == 0 && v12C == 0) {
			sparkleX += -delta;
			if (sparkleX <= 150)
				v3C = 1;
		}

		//moves sparkle to next vertex, which is 200 points down
		else if (sparkleY >= 300 && v1C == 1 && v2C == 1 && v3C == 1
			&& v4C == 0 && v5C == 0 && v6C == 0
			&& v7C == 0 && v8C == 0 && v9C == 0
			&& v10C == 0 && v11C == 0 && v12C == 0) {
			sparkleY += -delta;
			if (sparkleY <= 300)
				v4C = 1;
		}

		//moves sparkle to next vertex, which is 100 points right
		else if (sparkleX <= 250 && v1C == 1 && v2C == 1 && v3C == 1
			&& v4C == 1 && v5C == 0 && v6C == 0
			&& v7C == 0 && v8C == 0 && v9C == 0
			&& v10C == 0 && v11C == 0 && v12C == 0) {
			sparkleX += delta;
			if (sparkleX >= 250)
				v5C = 1;
		}

		//moves sparkle to next vertex, which is 50 points up
		else if (sparkleY <= 350 && v1C == 1 && v2C == 1 && v3C == 1
			&& v4C == 1 && v5C == 1 && v6C == 0
			&& v7C == 0 && v8C == 0 && v9C == 0
			&& v10C == 0 && v11C == 0 && v12C == 0) {
			sparkleY += delta;
			if (sparkleY >= 350)
				v6C = 1;
		}

		//moves sparkle to next vertex, which is 50 points left
		else if (sparkleX >= 200 && v1C == 1 && v2C == 1 && v3C == 1
			&& v4C == 1 && v5C == 1 && v6C == 1
			&& v7C == 0 && v8C == 0 && v9C == 0
			&& v10C == 0 && v11C == 0 && v12C == 0) {
			sparkleX += -delta;
			if (sparkleX <= 200)
				v7C = 1;
		}

		//moves sparkle to next vertex, which is 50 points up
		else if (sparkleY <= 400 && v1C == 1 && v2C == 1 && v3C == 1
			&& v4C == 1 && v5C == 1 && v6C == 1
			&& v7C == 1 && v8C == 0 && v9C == 0
			&& v10C == 0 && v11C == 0 && v12C == 0) {
			sparkleY += delta;
			if (sparkleY >= 400)
				v8C = 1;
		}

		//moves sparkle to next vertex, which is 100 points right
		else if (sparkleX <= 300 && v1C == 1 && v2C == 1 && v3C == 1
			&& v4C == 1 && v5C == 1 && v6C == 1
			&& v7C == 1 && v8C == 1 && v9C == 0
			&& v10C == 0 && v11C == 0 && v12C == 0) {
			sparkleX += delta;
			if (sparkleX >= 300)
				v9C = 1;
		}
		//moves sparkle to bottom right corner
		else if (sparkleY >= 250 && v1C == 1 && v2C == 1 && v3C == 1
			&& v4C == 1 && v5C == 1 && v6C == 1
			&& v7C == 1 && v8C == 1 && v9C == 1
			&& v10C == 0 && v11C == 0 && v12C == 0) {
			sparkleY += -delta;
			if (sparkleY <= 250)
				v10C = 1;
		}

		//moves sparkle to bottom left corner
		else if (sparkleX >= 100 && v1C == 1 && v2C == 1 && v3C == 1
			&& v4C == 1 && v5C == 1 && v6C == 1
			&& v7C == 1 && v8C == 1 && v9C == 1
			&& v10C == 1 && v11C == 0 && v12C == 0) {
			sparkleX += -delta;
			if (sparkleX <= 100)
				v11C = 1;
		}

		//moves sparkle to top right corner (the starting position)
		else if (sparkleY <= 550 && v1C == 1 && v2C == 1 && v3C == 1
			&& v4C == 1 && v5C == 1 && v6C == 1
			&& v7C == 1 && v8C == 1 && v9C == 1
			&& v10C == 1 && v11C == 1 && v12C == 0) {
			sparkleY += delta;
			if (sparkleY >= 550)
				v12C = 1;
		}

		//resets all vertex tracking values
		else if (sparkleY >= 550 && v1C == 1 && v2C == 1 && v3C == 1
			&& v4C == 1 && v5C == 1 && v6C == 1
			&& v7C == 1 && v8C == 1 && v9C == 1
			&& v10C == 1 && v11C == 1 && v12C == 1) {
			v1C = 0;
			v2C = 0;
			v3C = 0;
			v4C = 0;
			v5C = 0;
			v6C = 0;
			v7C = 0;
			v8C = 0;
			v9C = 0;
			v10C = 0;
			v11C = 0;
			v12C = 0;
		}
	}
}

/************************************************************************
Function:		sparkMaker
Description:	This function will create 12 sparks come out of the 
centre of the sparkle to a random locations that are up to a maximum of 
200 points away from the centre of the sparkle. The colors of each spark
will be one of twenty colors, picked at random. The speed is delta 
multiplied by a random number between 1 and 3. Because the locations and
speeds are random, the time between each spark being generated is also 
random.
*************************************************************************/
void sparkMaker(float x, float y) {
	//make lines smooth
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_LINES);

	//How sparks 1 works will be explained, the rest of the sparks are the same, except that the coordinate is different

	//Spark 1 - Coordinate pos x, pos y
	//Checks to see if X and Y of spark1 and 0
	if (spark1XPos == 0 && spark1YPos == 0) {
		//picks a random X and Y location in the positive X-positive-Y part of the graph from the centre of the sparkle 
		sparkEnd1X = rand() % 201;
		sparkEnd1Y = rand() % 201;
		//picks a color at random for the color array
		colorIndex1 = rand() % 19;
		//assigns the red, green and blue variable the appropriate value
		spark1R = colorArr[colorIndex1][0];
		spark1G = colorArr[colorIndex1][1];
		spark1B = colorArr[colorIndex1][2];
	}
	//executes if the x position and y position of the spark aren't where they are supposed to be
	if (spark1XPos <= sparkEnd1X && spark1YPos <= sparkEnd1Y) {
		//draws spark to be transparent at beginning
		glColor4f(spark1R, spark1G, spark1B,0);
		glVertex2f(x, y);
		//draws spark to be opaque at end
		glColor4f(spark1R, spark1G, spark1B, 1);
		glVertex2f(x + spark1XPos, y + spark1YPos);
		//increases x and y position of the spark by a random number between 1 and 3 times delta
		spark1XPos += (rand()%3)*delta;
		spark1YPos += (rand()%3)*delta;
	}
	//when x is where it is supposed to be, it resets
	if (spark1XPos >= sparkEnd1X)
		spark1XPos = 0;
	//when y is where it is supposed to be, it resets
	if (spark1YPos >= sparkEnd1Y)
		spark1YPos = 0;

	//spark two - Coordinate pos x, neg y
	if (spark2XPos == 0 && spark2YPos == 0) {
		sparkEnd2X = rand() % 201;
		sparkEnd2Y = 0-(rand() % 201);
		colorIndex2 = rand() % 19;
		spark2R= colorArr[colorIndex2][0];
		spark2G = colorArr[colorIndex2][1];
		spark2B = colorArr[colorIndex2][2];
	}
	if (spark2XPos <= sparkEnd2X && spark2YPos >= sparkEnd2Y) {
		glColor4f(spark2R, spark2G, spark2B, 0);
		glVertex2f(x, y);
		glColor4f(spark2R, spark2G, spark2B, 1);
		glVertex2f(x + spark2XPos, y + spark2YPos);
		spark2XPos += (rand()%3)*delta;
		spark2YPos += -(rand()%3)*delta;
	}
	if (spark2XPos >= sparkEnd2X)
		spark2XPos = 0;
	if (spark2YPos <= sparkEnd2Y) {
		spark2YPos = 0;
	}

	//spark 3 - Coordinate neg x, neg y
	if (spark3XPos == 0 && spark3YPos == 0) {
		sparkEnd3X = 0-(rand() % 201);
		sparkEnd3Y = 0 - (rand() % 201);
		colorIndex3 = rand() % 19;
		spark3R = colorArr[colorIndex3][0];
		spark3G = colorArr[colorIndex3][1];
		spark3B = colorArr[colorIndex3][2];
	}
	if (spark3XPos >= sparkEnd3X && spark3YPos >= sparkEnd3Y) {
		glColor4f(spark3R, spark3G, spark3B, 0);
		glVertex2f(x, y);
		glColor4f(spark3R, spark3G, spark3B, 1);
		glVertex2f(x + spark3XPos, y + spark3YPos);
		spark3XPos += -(rand()%3)*delta;
		spark3YPos += -(rand()%3)*delta;
	}
	if (spark3XPos <= sparkEnd3X)
		spark3XPos = 0;
	if (spark3YPos <= sparkEnd3Y)
		spark3YPos = 0;

	//spark 4 - Coordinate neg x, pos y
	if (spark4XPos == 0 && spark4YPos == 0) {
		sparkEnd4X = 0 - (rand() % 201);
		sparkEnd4Y = (rand() % 201);
		colorIndex4 = rand() % 19;
		spark4R = colorArr[colorIndex4][0];
		spark4G = colorArr[colorIndex4][1];
		spark4B = colorArr[colorIndex4][2];
	}
	if (spark4XPos >= sparkEnd4X && spark4YPos <= sparkEnd4Y) {
		glColor4f(spark4R, spark4G, spark4B, 0);
		glVertex2f(x, y);
		glColor4f(spark4R, spark4G, spark4B, 1);
		glVertex2f(x + spark4XPos, y + spark4YPos);
		spark4XPos += -(rand()%3)*delta;
		spark4YPos += (rand()%3)*delta;
	}
	if (spark4XPos <= sparkEnd4X)
		spark4XPos = 0;
	if (spark4YPos >= sparkEnd4Y)
		spark4YPos = 0;

	//Spark 5 - Coordinate pos x, pos y
	if (spark5XPos == 0 && spark5YPos == 0) {
		sparkEnd5X = rand() % 201;
		sparkEnd5Y = rand() % 201;
		colorIndex5 = rand() % 19;
		spark5R = colorArr[colorIndex5][0];
		spark5G = colorArr[colorIndex5][1];
		spark5B = colorArr[colorIndex5][2];
	}
	if (spark5XPos <= sparkEnd5X && spark5YPos <= sparkEnd5Y) {
		glColor4f(spark5R, spark5G, spark5B, 0);
		glVertex2f(x, y);
		glColor4f(spark5R, spark5G, spark5B, 1);
		glVertex2f(x + spark5XPos, y + spark5YPos);
		spark5XPos += (rand()%3)*delta;
		spark5YPos += (rand()%3)*delta;
	}
	if (spark5XPos >= sparkEnd5X)
		spark5XPos = 0;
	if (spark5YPos >= sparkEnd5Y)
		spark5YPos = 0;

	//spark 6 - Coordinate pos x, neg y
	if (spark6XPos == 0 && spark6YPos == 0) {
		sparkEnd6X = rand() % 201;
		sparkEnd6Y = 0 - (rand() % 201);
		colorIndex6 = rand() % 19;
		spark6R = colorArr[colorIndex6][0];
		spark6G = colorArr[colorIndex6][1];
		spark6B = colorArr[colorIndex6][2];
	}
	if (spark6XPos <= sparkEnd6X && spark6YPos >= sparkEnd6Y) {
		glColor4f(spark6R, spark6G, spark6B, 0);
		glVertex2f(x, y);
		glColor4f(spark6R, spark6G, spark6B, 1);
		glVertex2f(x + spark6XPos, y + spark6YPos);
		spark6XPos += (rand()%3)*delta;
		spark6YPos += -(rand()%3)*delta;
	}
	if (spark6XPos >= sparkEnd6X)
		spark6XPos = 0;
	if (spark6YPos <= sparkEnd6Y) {
		spark6YPos = 0;
	}

	//spark 7- Coordinate neg x, neg y
	if (spark7XPos == 0 && spark7YPos == 0) {
		sparkEnd7X = 0 - (rand() % 201);
		sparkEnd7Y = 0 - (rand() % 201);
		colorIndex7 = rand() % 19;
		spark7R = colorArr[colorIndex7][0];
		spark7G = colorArr[colorIndex7][1];
		spark7B = colorArr[colorIndex7][2];
	}
	if (spark7XPos >= sparkEnd7X && spark7YPos >= sparkEnd7Y) {
		glColor4f(spark7R, spark7G, spark7B, 0);
		glVertex2f(x, y);
		glColor4f(spark7R, spark7G, spark7B, 1);
		glVertex2f(x + spark7XPos, y + spark7YPos);
		spark7XPos += -(rand()%3)*delta;
		spark7YPos += -(rand()%3)*delta;
	}
	if (spark7XPos <= sparkEnd7X)
		spark7XPos = 0;
	if (spark7YPos <= sparkEnd7Y)
		spark7YPos = 0;

	//spark 8 - Coordinate neg x, pos y
	if (spark8XPos == 0 && spark8YPos == 0) {
		sparkEnd8X = 0 - (rand() % 201);
		sparkEnd8Y = (rand() % 201);
		colorIndex8 = rand() % 19;
		spark8R = colorArr[colorIndex8][0];
		spark8G = colorArr[colorIndex8][1];
		spark8B = colorArr[colorIndex8][2];
	}
	if (spark8XPos >= sparkEnd8X && spark8YPos <= sparkEnd8Y) {
		glColor4f(spark8R, spark8G, spark8B, 0);
		glVertex2f(x, y);
		glColor4f(spark8R, spark8G, spark8B, 1);
		glVertex2f(x + spark8XPos, y + spark8YPos);
		spark8XPos += -(rand()%3)*delta;
		spark8YPos += (rand()%3)*delta;
	}
	if (spark8XPos <= sparkEnd8X)
		spark8XPos = 0;
	if (spark8YPos >= sparkEnd8Y)
		spark8YPos = 0;

	//Spark 9 - Coordinate pos x, pos y
	if (spark9XPos == 0 && spark9YPos == 0) {
		sparkEnd9X = rand() % 201;
		sparkEnd9Y = rand() % 201;
		colorIndex9 = rand() % 19;
		spark9R = colorArr[colorIndex9][0];
		spark9G = colorArr[colorIndex9][1];
		spark9B = colorArr[colorIndex9][2];
	}
	if (spark9XPos <= sparkEnd9X && spark9YPos <= sparkEnd9Y) {
		glColor4f(spark9R, spark9G, spark9B, 0);
		glVertex2f(x, y);
		glColor4f(spark9R, spark9G, spark9B, 1);
		glVertex2f(x + spark9XPos, y + spark9YPos);
		spark9XPos += (rand()%3)*delta;
		spark9YPos += (rand()%3)*delta;
	}
	if (spark9XPos >= sparkEnd9X)
		spark9XPos = 0;
	if (spark9YPos >= sparkEnd9Y)
		spark9YPos = 0;

	//spark 10 - Coordinate pos x, neg y
	if (spark10XPos == 0 && spark10YPos == 0) {
		sparkEnd10X = rand() % 201;
		sparkEnd10Y = 0 - (rand() % 201);
		colorIndex10 = rand() % 19;
		spark10R = colorArr[colorIndex10][0];
		spark10G = colorArr[colorIndex10][1];
		spark10B = colorArr[colorIndex10][2];
	}
	if (spark10XPos <= sparkEnd10X && spark10YPos >= sparkEnd10Y) {
		glColor4f(spark10R, spark10G, spark10B, 0);
		glVertex2f(x, y);
		glColor4f(spark10R, spark10G, spark10B, 1);
		glVertex2f(x + spark10XPos, y + spark10YPos);
		spark10XPos += (rand()%3)*delta;
		spark10YPos += -(rand()%3)*delta;
	}
	if (spark10XPos >= sparkEnd10X)
		spark10XPos = 0;
	if (spark10YPos <= sparkEnd10Y) {
		spark10YPos = 0;
	}

	//spark 11 - Coordinate neg x, neg y
	if (spark11XPos == 0 && spark11YPos == 0) {
		sparkEnd11X = 0 - (rand() % 201);
		sparkEnd11Y = 0 - (rand() % 201);
		colorIndex11 = rand() % 19;
		spark11R = colorArr[colorIndex11][0];
		spark11G = colorArr[colorIndex11][1];
		spark11B = colorArr[colorIndex11][2];
	}
	if (spark11XPos >= sparkEnd11X && spark11YPos >= sparkEnd11Y) {
		glColor4f(spark11R, spark11G, spark11B, 0);
		glVertex2f(x, y);
		glColor4f(spark11R, spark11G, spark11B, 1);
		glVertex2f(x + spark11XPos, y + spark11YPos);
		spark11XPos += -(rand()%3)*delta;
		spark11YPos += -(rand()%3)*delta;
	}
	if (spark11XPos <= sparkEnd11X)
		spark11XPos = 0;
	if (spark11YPos <= sparkEnd11Y)
		spark11YPos = 0;

	//spark 12 - Coordinate neg x, pos y
	if (spark12XPos == 0 && spark12YPos == 0) {
		sparkEnd12X = 0 - (rand() % 201);
		sparkEnd12Y = (rand() % 201);
		colorIndex12 = rand() % 19;
		spark12R = colorArr[colorIndex12][0];
		spark12G = colorArr[colorIndex12][1];
		spark12B = colorArr[colorIndex12][2];
	}
	if (spark12XPos >= sparkEnd12X && spark12YPos <= sparkEnd12Y) {
		glColor4f(spark12R, spark12G, spark12B, 0);
		glVertex2f(x, y);
		glColor4f(spark12R, spark12G, spark12B, 1);
		glVertex2f(x + spark12XPos, y + spark12YPos);
		spark12XPos += -(rand()%3)*delta;
		spark12YPos += (rand()%3)*delta;
	}
	if (spark12XPos <= sparkEnd12X)
		spark12XPos = 0;
	if (spark12YPos >= sparkEnd12Y)
		spark12YPos = 0;
	//ends the drawing
	glEnd();
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_BLEND);
}

/************************************************************************
Function:		mouseCheck
Description:	This function will, when a mouse button is pressed, check to see
if the cursor was pressed within the coordinates of a button in the program.
If it is, it will turn on that button. When appropriate, it will also reset
values for variables that involve vertex tracking, sparkle coordinates, and 
resetting other button values
*************************************************************************/
void mouseCheck(void) {
	// executes when mouse button is pressed
	if (mousePressed){
		// resets value to 0 to see if it can be pressed again
		mousePressed = 0;

		// checks to see if cursor is within morph button, and if morph button is on,
		//it will turn it off. Resets all vertex trackers to 0, and sets sparkle position
		if (mouseX >= buttons[0][0] && mouseX <= (buttons[0][0] + buttonWidth) &&
			mouseY <= buttons[0][1] && mouseY >= (buttons[0][1] - buttonHeight) && morphButState) {
			v1C = 0;
			v2C = 0;
			v3C = 0;
			v4C = 0;
			v5C = 0;
			v6C = 0;
			v7C = 0;
			v8C = 0;
			v9C = 0;
			v10C = 0;
			v11C = 0;
			v12C = 0;
			morphButState = 0;
			sparkleX = 100;
			sparkleY = 550;
		}
		// checks to see if cursor is within morph button, and if morph button is off,
		//it will turn it on. Resets all vertex trackers to 0, and sets sparkle position
		else if (mouseX >= buttons[0][0] && mouseX <= (buttons[0][0] + buttonWidth) &&
			mouseY <= buttons[0][1] && mouseY >= (buttons[0][1] - buttonHeight)) {
			v1C = 0;
			v2C = 0;
			v3C = 0;
			v4C = 0;
			v5C = 0;
			v6C = 0;
			v7C = 0;
			v8C = 0;
			v9C = 0;
			v10C = 0;
			v11C = 0;
			v12C = 0;
			morphButState = 1;
			sparkleX = 200;
			sparkleY = 600;
		}

		// checks to see if cursor is within sparkle button, and if sparkle button is on,
		//it will turn it and the spark button off. Resets all vertex trackers to 0, and 
		//sets sparkle position
		if (mouseX >= buttons[1][0] && mouseX <= (buttons[1][0] + buttonWidth) &&
			mouseY <= buttons[1][1] && mouseY >= (buttons[1][1] - buttonHeight) && sparkleButState) {
			sparkleButState = 0;
			sparksButState = 0;
			v1C = 0;
			v2C = 0;
			v3C = 0;
			v4C = 0;
			v5C = 0;
			v6C = 0;
			v7C = 0;
			v8C = 0;
			v9C = 0;
			v10C = 0;
			v11C = 0;
			v12C = 0;
			sparkleX = 100;
			sparkleY = 550;
			
			
			// checks to see if cursor is within sparkle button, and if sparkle button is off,
			//it will turn it and the spark button on. Sets sparkle X and Y positions and 
			//activates the sparkle position control
		}
		else if (mouseX >= buttons[1][0] && mouseX <= (buttons[1][0] + buttonWidth) &&
			mouseY <= buttons[1][1] && mouseY >= (buttons[1][1] - buttonHeight)) {
			sparkleButState = 1;
			if (morphButState) {//execute star sparkle control
				sparkleX = 200;
				sparkleY = 600;
				sparklePositionStarControl();
			}
			if (!morphButState) {//execute G sparkle control
				sparkleX = 100;
				sparkleY = 550;
				sparklePositionGControl();
			}	
		}

		// checks to see if cursor is within spark button, and if spark button is on,
		//it will turn it off
		if (mouseX >= buttons[2][0] && mouseX <= (buttons[2][0] + buttonWidth) &&
			mouseY <= buttons[2][1] && mouseY >= (buttons[2][1] - buttonHeight) && sparksButState) {
			sparksButState = 0;
		}

		// checks to see if cursor is within spark button, and if spark button is off,
		//it will turn it on
		else if (mouseX >= buttons[2][0] && mouseX <= (buttons[2][0] + buttonWidth) &&
			mouseY <= buttons[2][1] && mouseY >= (buttons[2][1] - buttonHeight)&&sparkleButState) {
			sparksButState = 1;
		}

		// checks to see if cursor is within fireworks button, and if fireworks button is on,
		//it will turn it off
		if (mouseX >= buttons[3][0] && mouseX <= (buttons[3][0] + buttonWidth) &&
			mouseY <= buttons[3][1] && mouseY >= (buttons[3][1] - buttonHeight) && fireworksButState) {
			fireworksButState = 0;
		}
		// checks to see if cursor is within fireworks button, and if fireworks button is off,
		//it will turn it on
		else if (mouseX >= buttons[3][0] && mouseX <= (buttons[3][0] + buttonWidth) &&
			mouseY <= buttons[3][1] && mouseY >= (buttons[3][1] - buttonHeight)) {
			fireworksButState = 1;
		}

		// checks to see if cursor is within credit button, and if credit button is on,
		//it will turn it off
		if (mouseX >= buttons[4][0] && mouseX <= (buttons[4][0] + buttonWidth) &&
			mouseY <= buttons[4][1] && mouseY >= (buttons[4][1] - buttonHeight) && creditButState) {
			creditButState = 0;
		}
		// checks to see if cursor is within credit button, and if credit button is off,
		//it will turn it on
		else if (mouseX >= buttons[4][0] && mouseX <= (buttons[4][0] + buttonWidth) &&
			mouseY <= buttons[4][1] && mouseY >= (buttons[4][1] - buttonHeight)) {
			creditButState = 1;
		}

		// checks to see if cursor is within crosshair button, and if crosshair button is on,
		//it will turn it off
		if (mouseX >= buttons[5][0] && mouseX <= (buttons[5][0] + buttonWidth) &&
			mouseY <= buttons[5][1] && mouseY >= (buttons[5][1] - buttonHeight) && crosshairButState) {
			crosshairButState = 0;
		}
		// checks to see if cursor is within crosshair button, and if crosshair button is off,
		//it will turn it on
		else if (mouseX >= buttons[5][0] && mouseX <= (buttons[5][0] + buttonWidth) &&
			mouseY <= buttons[5][1] && mouseY >= (buttons[5][1] - buttonHeight)) {
			crosshairButState = 1;
		}
	}
}

/************************************************************************
Function:		drawSpark
Description:	This function will draw a yellow spark from a given start
location to a given end location. It will start opaque and end transparent.
Not to be confused with sparkMaker.
*************************************************************************/
void drawSpark(GLfloat startX, GLfloat startY, GLfloat endX, GLfloat endY) {
	//Make lines smooth
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_LINES);
	//Yellow, opaque
	glColor4f(1, 1, 0, 1);
	glVertex2f(startX, startY);
	//Yellow, translucent
	glColor4f(1, 1, 0, 0);
	glVertex2f(endX, endY);
	//end drawing
	glEnd();
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_BLEND);
}

/************************************************************************
Function:		drawSparkle
Description:	This function will utilize the drawSpark function and draw
8 sparks from the sparkle location. 
*************************************************************************/
void drawSparkle(GLfloat centreX, GLfloat centreY) {

	//The centre is always the centreX and centreY, and the end
	//is to make it look like a sparkle
	drawSpark(centreX, centreY, centreX, centreY + 50);
	drawSpark(centreX, centreY, centreX + 25, centreY + 25);
	drawSpark(centreX, centreY, centreX + 50, centreY);
	drawSpark(centreX, centreY, centreX + 25, centreY - 25);
	drawSpark(centreX, centreY, centreX, centreY - 50);
	drawSpark(centreX, centreY, centreX - 25, centreY - 25);
	drawSpark(centreX, centreY, centreX - 50, centreY);
	drawSpark(centreX, centreY, centreX - 25, centreY + 25);
	//calcutlates the rotation of sparkle
	angle-=0.06f;
	//resets angle to 0 if it gets too low 
	if (angle <= (GLfloat)-360)
		angle = 0;
}

/************************************************************************
Function:		fireworks
Description:	This function will utilize the sparkMaker function to make
a fireworks effect.
*************************************************************************/
void fireworks(void){
	if (fireworksButState) {
		sparkMaker(30+fireworkUpBy/2,fireworkUpBy-100);
		sparkMaker(400-fireworkUpBy/2, fireworkUpBy);
		sparkMaker(280, fireworkUpBy-100);
		sparkMaker(375, fireworkUpBy);
		sparkMaker(50, fireworkUpBy);
	}
}

/************************************************************************
Function:		writeButtonText
Description:	This function will draw/write the text onto the buttons.
*************************************************************************/
void writeButtonText(void) {

	//make the color yellow
	glColor3f(1, 1, 0);

	//Morph button text position
	glRasterPos3f(buttons[0][0] + 20, buttons[0][1] - 20, 0.0f);
	//Text for morph button
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, "Morph");
	
	//Sparkle button text position
	glRasterPos3f(buttons[1][0] + 20, buttons[1][1] -20, 0.0f);
	//text for sparkle button
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, "Sparkle");

	//Sparks button text position
	glRasterPos3f(buttons[2][0] + 20, buttons[2][1] - 20, 0.0f);
	//Text for sparks button
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, "Sparks");
	
	//fireworks button text position
	glRasterPos3f(buttons[3][0] + 10, buttons[3][1] -20, 0.0f);
	// Text for fireworks button
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, "Fireworks");

	//credit button text position
	glRasterPos3f(buttons[4][0] + 20, buttons[3][1] - 20, 0.0f);
	// Text for credit button
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, "Credits");

	//Crosshair button text position
	glRasterPos3f(buttons[5][0] + 10, buttons[5][1] - 20, 0.0f);
	// Text for crosshair button
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, "Crosshair");
}

/************************************************************************
Function:		credits
Description:	This function will make a message displaying the credits
slowly come down the screen when the button is pressed, and back up the
screen when the button is not pressed.
*************************************************************************/
void credits(void) {
	if (creditButState) {
		//make color white
		glColor3f(1, 1, 1);
		//moves the y value down when pressed
		if(creditY>=0)
			creditY += -creditsDelta;
		//writes all the credits
		glRasterPos3f(115, 255+ creditY, 0.0f);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, "A CSCI 3161 Project");
		glRasterPos3f(75, 200+creditY, 0.0f);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, "Brought to you by Liam Gowan");
		glRasterPos3f(118, 120+creditY, 0.0f);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, "I hope you enjoyed");
	}
	else {
		//makes color white
		glColor3f(1, 1, 1);
		//moves y value up when not pressed
		if (creditY <= 500)
			creditY += creditsDelta;
		//writes all the credits
		glRasterPos3f(115, 255 + creditY, 0.0f);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, "A CSCI 3161 Project");
		glRasterPos3f(75, 200 + creditY, 0.0f);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, "Brought to you by Liam Gowan");
		glRasterPos3f(118, 120 + creditY, 0.0f);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, "I hope you enjoyed");
	}
}

/************************************************************************
Function:		crosshair
Description:	This function will use the mouse positions to draw a 
crosshair around it.
*************************************************************************/
void crosshair(void) {
	//set line width to 3
	glLineWidth(3);
	//draw to the right from mouse 
	glBegin(GL_LINES);
	glColor3f(.75, 0, .5);
	glVertex2f(mouseX, mouseY);
	glVertex2f(mouseX + 25, mouseY);
	glEnd();
	//draw to to left from mouse 
	glBegin(GL_LINES);
	glVertex2f(mouseX, mouseY);
	glVertex2f(mouseX-25, mouseY);
	glEnd();
	//draw down from mouse
	glBegin(GL_LINES);
	glVertex2f(mouseX, mouseY);
	glVertex2f(mouseX, mouseY-25);
	glEnd();
	//draw up from mouse
	glBegin(GL_LINES);
	glVertex2f(mouseX, mouseY);
	glVertex2f(mouseX, mouseY+25);
	glEnd();
	//set line width back to 1
	glLineWidth(1);
}

/************************************************************************
Function:		mouseEventListener
Description:	This function will check to see if the left button is 
pressed, and where it is
*************************************************************************/
void mouseEventListener(int button, int state, int x, int y){
	//sees if left button is down
	if (button == GLUT_LEFT_BUTTON  && state == GLUT_DOWN){
		mousePressed = 1;
		
		// Inverts y coordinate
		y = windowHeight - (float)y;
		//updates values
		mouseX = x;
		mouseY = y;
		//force a redraw
		glutPostRedisplay();
	}
}

/************************************************************************
Function:		mousePos
Description:	This function will get the mouseX and mouseY position, 
even if the user did not click.
*************************************************************************/
void mousePos(int x, int y) {
	//gets mouseX
	mouseX = x;
	//gets mouse Y inverted
	mouseY = 600-y;
}

/************************************************************************
Function:		display
Description:	This function will display things to the screen and call 
other appropriate methods. It also draws the quads that from the G/Star.
*************************************************************************/
void display(void){
	//checks where mouse is
	mouseCheck();
	//sets screen to black
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	//draws the first quad
	glBegin(GL_QUADS);
	glColor3f(0, 0, 0.7f);
	glVertex2d(interpQuad1[0][0], interpQuad1[0][1]);
	glVertex2d(interpQuad1[1][0], interpQuad1[1][1]);
	glVertex2d(interpQuad1[2][0], interpQuad1[2][1]);
	glVertex2d(interpQuad1[3][0], interpQuad1[3][1]);
	glEnd();

	//draws the second quad
	glBegin(GL_QUADS);
	glColor3f(0, 0, 0.7f);
	glVertex2d(interpQuad2[0][0], interpQuad2[0][1]);
	glVertex2d(interpQuad2[1][0], interpQuad2[1][1]);
	glVertex2d(interpQuad2[2][0], interpQuad2[2][1]);
	glVertex2d(interpQuad2[3][0], interpQuad2[3][1]);
	glEnd();

	//draws the third quad
	glBegin(GL_QUADS);
	glColor3f(0, 0, 0.7f);
	glVertex2d(interpQuad3[0][0], interpQuad3[0][1]);
	glVertex2d(interpQuad3[1][0], interpQuad3[1][1]);
	glVertex2d(interpQuad3[2][0], interpQuad3[2][1]);
	glVertex2d(interpQuad3[3][0], interpQuad3[3][1]);
	glEnd();

	//draws the fourth quad
	glBegin(GL_QUADS);
	glColor3f(0, 0, 0.7f);
	glVertex2d(interpQuad4[0][0], interpQuad4[0][1]);
	glVertex2d(interpQuad4[1][0], interpQuad4[1][1]);
	glVertex2d(interpQuad4[2][0], interpQuad4[2][1]);
	glVertex2d(interpQuad4[3][0], interpQuad4[3][1]);
	glEnd();

	//draws the fith quad
	glBegin(GL_QUADS);
	glColor3f(0, 0, 0.7f);
	glVertex2d(interpQuad5[0][0], interpQuad5[0][1]);
	glVertex2d(interpQuad5[1][0], interpQuad5[1][1]);
	glVertex2d(interpQuad5[2][0], interpQuad5[2][1]);
	glVertex2d(interpQuad5[3][0], interpQuad5[3][1]);
	glEnd();


	//if sparkle button is on, draw the sparkle and rotate and
	//increase/decrease the size
	if (sparkleButState) {
		glLoadIdentity();
		glPushMatrix();
		glTranslatef(sparkleX, sparkleY, 0);
		glRotatef(angle, 0, 0, 1);
		glScalef(scaleBy, scaleBy, 1);
		glTranslatef(-sparkleX, -sparkleY, 0);
		drawSparkle(sparkleX, sparkleY);
		glPopMatrix();
		glLoadIdentity();
	}
	//if spark button is on, draw sparks
	if (sparksButState)
		sparkMaker(sparkleX,sparkleY);

	
	//Draws the background for the buttons
	glBegin(GL_QUADS);
	glColor3f(0, .5, 0);
	glVertex2d(0, 150);
	glVertex2d(400, 150);
	glVertex2d(400, 0);
	glVertex2d(0, 0);
	glEnd();



	//draw buttons
	drawButtons();
	//write button text
	writeButtonText();

	//calls the credit function
	credits();

	//if crosshair button is pressed, draw the crosshair
	if (crosshairButState)
		crosshair();
	
	if (fireworksButState)
		fireworks();
	//force redraw
	glutPostRedisplay();
	glutSwapBuffers();
	//draw when ready
	glFlush();
}

/************************************************************************
Function:		init
Description:	This function will initialize everything needed.
*************************************************************************/
void init(void){
	//sets background color to black
	glColor3f(0,0,0);
	//makes it projection mode
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// set window height and width
	glOrtho(0, windowWidth, 0, windowHeight,-1,1);
	glMatrixMode(GL_MODELVIEW);
}

/************************************************************************
Function:		myIdle
Description:	This function handles the scaling effects of the sparkle,
calls the sparkle position functions, and updates the position of the 
quads when morph is pressed.
*************************************************************************/
void myIdle() {
	//Increases the scale amount
	scaleBy += scaleDelta;
	//makes scaleDelta the opposite when required so the the sparkle will
	//both increase and decrease in size
	if (scaleBy >= 1.5 || scaleBy <= .5)
		scaleDelta = -scaleDelta;

	//Increases the fireworks rise amount
	fireworkUpBy += fireworksDelta;
	//makes fireworkUpBy the opposite when required so the fireworks will
	//both rise and fall
	if (fireworkUpBy >= 550 || fireworkUpBy <= 250)
		fireworksDelta=-fireworksDelta;


	//morph from G to Star
	//This just increases or decreases the appropiate value by either a positive 
	//or negative delta value, until the shape looks like a star
	if (morphButState) {
		//calls the sparkle control for the star
		sparklePositionStarControl();
		//Quad 1
		//first X
		if (interpQuad1[0][0] <= SOne[0][0])
			interpQuad1[0][0] += delta;
		//first Y
		if (interpQuad1[0][1] <= SOne[0][1])
			interpQuad1[0][1] += delta;
		//second X
		if (interpQuad1[1][0] <= SOne[1][0])
			interpQuad1[1][0] += delta;
		//second Y
		if (interpQuad1[1][1] >= SOne[1][1])
			interpQuad1[1][1] += -delta;
		//third X
		if (interpQuad1[2][0] >= SOne[2][0])
			interpQuad1[2][0] += -delta;
		//third Y
		if (interpQuad1[2][1] >= SOne[2][1])
			interpQuad1[2][1] += -delta;
		//fourth X
		if (interpQuad1[3][0] <= SOne[3][0])
			interpQuad1[3][0] += delta;
		//fourth Y
		if (interpQuad1[3][1] >= SOne[3][1])
			interpQuad1[3][1] += -delta;

		//Quad 2
		//First X
		if (interpQuad2[0][0] >= STwo[0][0])
			interpQuad2[0][0] += -delta;
		//first Y
		if (interpQuad2[0][1] >= STwo[0][1])
			interpQuad2[0][1] += -delta;
		//second X
		if (interpQuad2[1][0] >= STwo[1][0])
			interpQuad2[1][0] += -delta;
		//second Y
		if (interpQuad2[1][1] >= STwo[1][1])
			interpQuad2[1][1] += -delta;
		//third X
		if (interpQuad2[2][0] <= STwo[2][0])
			interpQuad2[2][0] += delta;
		//third Y
		if (interpQuad2[2][1] >= STwo[2][1])
			interpQuad2[2][1] += -delta;
		//fourth X
		if (interpQuad2[3][0] <= STwo[3][0])
			interpQuad2[3][0] += delta;
		//fourth Y
		if (interpQuad2[3][1] >= STwo[3][1])
			interpQuad2[3][1] += -delta;

		//Quad 3
		//First X
		if (interpQuad3[0][0] <= SThree[0][0])
			interpQuad3[0][0] += delta;
		//first Y
		if (interpQuad3[0][1] >= SThree[0][1])
			interpQuad3[0][1] += -delta;
		//second X
		if (interpQuad3[1][0] >= SThree[1][0])
			interpQuad3[1][0] += -delta;
		//second Y
		if (interpQuad3[1][1] >= SThree[1][1])
			interpQuad3[1][1] += -delta;
		//third X
		if (interpQuad3[2][0] >= SThree[2][0])
			interpQuad3[2][0] += -delta;
		//third Y
		if (interpQuad3[2][1] <= SThree[2][1])
			interpQuad3[2][1] += delta;
		//fourth X
		if (interpQuad3[3][0] <= SThree[3][0])
			interpQuad3[3][0] += delta;
		//fourth Y
		if (interpQuad3[3][1] <= SThree[3][1])
			interpQuad3[3][1] += delta;

		//Quad 4
		//First X
		if (interpQuad4[0][0] >= SFour[0][0])
			interpQuad4[0][0] += -delta;
		//first Y
		if (interpQuad4[0][1] >= SFour[0][1])
			interpQuad4[0][1] += -delta;
		//second X
		if (interpQuad4[1][0] <= SFour[1][0])
			interpQuad4[1][0] += delta;
		//second Y
		if (interpQuad4[1][1] >= SFour[1][1])
			interpQuad4[1][1] += -delta;
		//third X
		if (interpQuad4[2][0] <= SFour[2][0])
			interpQuad4[2][0] += delta;
		//third Y
		if (interpQuad4[2][1] >= SFour[2][1])
			interpQuad4[2][1] += -delta;
		//fourth X
		if (interpQuad4[3][0] <= SFour[3][0])
			interpQuad4[3][0] += delta;
		//fourth Y
		if (interpQuad4[3][1] >= SFour[3][1])
			interpQuad4[3][1] += -delta;

		//Quad 5
		//First X
		if (interpQuad5[0][0] <= SFive[0][0])
			interpQuad5[0][0] += delta;
		//first Y
		if (interpQuad5[0][1] <= SFive[0][1])
			interpQuad5[0][1] += delta;
		//second X
		if (interpQuad5[1][0] <= SFive[1][0])
			interpQuad5[1][0] += delta;
		//second Y
		if (interpQuad5[1][1] <= SFive[1][1])
			interpQuad5[1][1] += delta;
		//third X
		if (interpQuad5[2][0] <= SFive[2][0])
			interpQuad5[2][0] += delta;
		//third Y
		if (interpQuad5[2][1] >= SFive[2][1])
			interpQuad5[2][1] += -delta;
		//fourth X
		if (interpQuad5[3][0] <= SFive[3][0])
			interpQuad5[3][0] += delta;
		//fourth Y
		if (interpQuad5[3][1] >= SFive[3][1])
			interpQuad5[3][1] += -delta;	
	}

	//morph from Star to G
	//This just increases or decreases the appropiate value by either a positive 
	//or negative delta value, until the shape looks like a G
	else {
		sparklePositionGControl();
		//Quad 1
		//first X
		if (interpQuad1[0][0] >= Gone[0][0])
			interpQuad1[0][0] += -delta;
		//first Y
		if (interpQuad1[0][1] >= Gone[0][1])
			interpQuad1[0][1] += -delta;
		//second X
		if (interpQuad1[1][0] >= Gone[1][0])
			interpQuad1[1][0] += -delta;
		//second Y
		if (interpQuad1[1][1] <= Gone[1][1])
			interpQuad1[1][1] += delta;
		//third X
		if (interpQuad1[2][0] <= Gone[2][0])
			interpQuad1[2][0] += delta;
		//third Y
		if (interpQuad1[2][1] <= Gone[2][1])
			interpQuad1[2][1] += delta;
		//fourth X
		if (interpQuad1[3][0] >= Gone[3][0])
			interpQuad1[3][0] += -delta;
		//fourth Y
		if (interpQuad1[3][1] <= Gone[3][1])
			interpQuad1[3][1] += delta;

		//Quad 2
		//First X
		if (interpQuad2[0][0] <= Gtwo[0][0])
			interpQuad2[0][0] += delta;
		//first Y
		if (interpQuad2[0][1] <= Gtwo[0][1])
			interpQuad2[0][1] += delta;
		//second X
		if (interpQuad2[1][0] <= Gtwo[1][0])
			interpQuad2[1][0] += delta;
		//second Y
		if (interpQuad2[1][1] <= Gtwo[1][1])
			interpQuad2[1][1] += delta;
		//third X
		if (interpQuad2[2][0] >= Gtwo[2][0])
			interpQuad2[2][0] += -delta;
		//third Y
		if (interpQuad2[2][1] <= Gtwo[2][1])
			interpQuad2[2][1] += delta;
		//fourth X
		if (interpQuad2[3][0] >= Gtwo[3][0])
			interpQuad2[3][0] += -delta;
		//fourth Y
		if (interpQuad2[3][1] <= Gtwo[3][1])
			interpQuad2[3][1] += delta;

		//Quad 3
		//First X
		if (interpQuad3[0][0] >= Gthree[0][0])
			interpQuad3[0][0] += -delta;
		//first Y
		if (interpQuad3[0][1] <= Gthree[0][1])
			interpQuad3[0][1] += delta;
		//second X
		if (interpQuad3[1][0] <= Gthree[1][0])
			interpQuad3[1][0] += delta;
		//second Y
		if (interpQuad3[1][1] <= Gthree[1][1])
			interpQuad3[1][1] += delta;
		//third X
		if (interpQuad3[2][0] <= Gthree[2][0])
			interpQuad3[2][0] += delta;
		//third Y
		if (interpQuad3[2][1] >= Gthree[2][1])
			interpQuad3[2][1] += -delta;
		//fourth X
		if (interpQuad3[3][0] >= Gthree[3][0])
			interpQuad3[3][0] += -delta;
		//fourth Y
		if (interpQuad3[3][1] >= Gthree[3][1])
			interpQuad3[3][1] += -delta;

		//Quad 4
		//First X
		if (interpQuad4[0][0] <= Gfour[0][0])
			interpQuad4[0][0] += delta;
		//first Y
		if (interpQuad4[0][1] <= Gfour[0][1])
			interpQuad4[0][1] += delta;
		//second X
		if (interpQuad4[1][0] >= Gfour[1][0])
			interpQuad4[1][0] += -delta;
		//second Y
		if (interpQuad4[1][1] <= Gfour[1][1])
			interpQuad4[1][1] += delta;
		//third X
		if (interpQuad4[2][0] >= Gfour[2][0])
			interpQuad4[2][0] += -delta;
		//third Y
		if (interpQuad4[2][1] <= Gfour[2][1])
			interpQuad4[2][1] += delta;
		//fourth X
		if (interpQuad4[3][0] >= Gfour[3][0])
			interpQuad4[3][0] += -delta;
		//fourth Y
		if (interpQuad4[3][1] <= Gfour[3][1])
			interpQuad4[3][1] += delta;

		//Quad 5
		//First X
		if (interpQuad5[0][0] >= Gfive[0][0])
			interpQuad5[0][0] += -delta;
		//first Y
		if (interpQuad5[0][1] >= Gfive[0][1])
			interpQuad5[0][1] += -delta;
		//second X
		if (interpQuad5[1][0] >= Gfive[1][0])
			interpQuad5[1][0] += -delta;
		//second Y
		if (interpQuad5[1][1] >= Gfive[1][1])
			interpQuad5[1][1] += -delta;
		//third X
		if (interpQuad5[2][0] >= Gfive[2][0])
			interpQuad5[2][0] += -delta;
		//third Y
		if (interpQuad5[2][1] <= Gfive[2][1])
			interpQuad5[2][1] += delta;
		//fourth X
		if (interpQuad5[3][0] >= Gfive[3][0])
			interpQuad5[3][0] += -delta;
		//fourth Y
		if (interpQuad5[3][1] <= Gfive[3][1])
			interpQuad5[3][1] += delta;
	}
}

/************************************************************************
Function:		main
Description:	This function calls all functions required to make the 
program work.
*************************************************************************/
void main(int argc, char** argv) {
	//Ensures that values are random
	srand((unsigned)time(NULL));
	//initializes 
	glutInit(&argc, argv);
	//sets the display to use double buffering and use all colors
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	//Sets window width and height
	glutInitWindowSize(windowWidth, windowHeight);
	//creates the window
	glutCreateWindow("Liam Gowan - CSCI Assignment 1");
	//calls the display function
	glutDisplayFunc(display);
	//forces redraw
	glutPostRedisplay();
	//calls the idle function required for animation
	glutIdleFunc(myIdle);
	//calls the mouse event listener
	glutMouseFunc(mouseEventListener);
	glutPassiveMotionFunc(mousePos);
	//initializes
	init();
	//executes the main loop
	glutMainLoop();
}