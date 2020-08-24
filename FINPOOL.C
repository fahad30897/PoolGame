#include<stdio.h>
#include<conio.h>
#include<graphics.h>
#include<process.h>
#include<dos.h>
#include<stdlib.h>
#include<math.h>
#define n 100
#define ballSize 10
#define whiteBallIndex 0
#define pi 3.14
#define maxPower 150
#define ballCollisionLoss 20
#define ballPoints 10
#define holeSize 12

int gameState = 0; // 0 for playing, 1 for gameover
int moving =  0; // 0 for aiming, 1 for power , 2 for moving
int fixedVelocity = 8;
double shootAngle = 0;
int aimDistance = 50;
int foul = 0;
int playerTurn =1,ballHole=0, player1Score = 0, player2Score= 0, changeTurn = 0;
int player1Fill = 1 , player2Fill=2;
union REGS in , out;

struct hole{
	int x,y;
	int rectx1,recty1,rectx2,recty2;
};

struct poolBall{
	float x , y;
	float vx , vy;
	int iterations;
	float ax, ay;
	int color;
	float doRender;
	int fill;
};
int power = 0;
int powerDirection = 1;
int lastBall = 0;
int topBoundary, bottomBoundary ,leftBoundary, rightBoundary;
//struct poolBall* balls = (struct poolBall*)malloc((sizeof(struct poolBall) * n));
struct poolBall balls[n];
struct hole holes[6];

int getMax(int a, int b){
	return (a>=b)?a:b;
}

void addBall(float x, float y, int c,int f){

	//struct 	poolBall *ball = (struct poolBall*)malloc(sizeof(struct poolBall));
	struct poolBall ball;
	ball.x = x;
	ball.y = y;
	ball.vx = 0;
	ball.vy = 0;
	ball.iterations = 0;
	ball.ax = 0;
	ball.ay = 0;
	ball.color = c;
	ball.fill = f;
	ball.doRender= 1;
	balls[lastBall] = ball;
	lastBall++;

}

void render(){
	int i;
	int rect[8];
	//struct poolBall ball;
	int detectMoving = 0;
	int detectGameEnd = 0;
	int curColor;
	struct fillsettingstype *curFillColor;
	char score1[50],score2[50];
	setfillstyle(SOLID_FILL, GREEN);
	setcolor(WHITE);
	rect[0] = leftBoundary;
	rect[1] = topBoundary;

	rect[2] = rightBoundary;
	rect[3] = topBoundary;

	rect[4] = rightBoundary;
	rect[5] = bottomBoundary;

	rect[6] = leftBoundary;
	rect[7] = bottomBoundary;

	fillpoly(4 , rect); // to render table
	settextstyle(SANS_SERIF_FONT, HORIZ_DIR,1);
	sprintf(score1, "player1 score: %d",player1Score);
	sprintf(score2, "player2 score: %d",player2Score);

	outtextxy( leftBoundary,topBoundary - 30,score1);
	outtextxy( rightBoundary-textwidth(score2), topBoundary - 30,score2);

	if(playerTurn == 1){
		outtextxy(getmaxx()/2-10, topBoundary - 20,"player 1 turn");
	}
	else{
		outtextxy(getmaxx()/2 -10, topBoundary -20,"player 2 turn");
	}

	// to render each ball

	for(i = 0; i < lastBall ; i++){
		//ball = balls[i];
		if(balls[i].doRender){
			double changeiter;
			/*if(i != 0){
				detectGameEnd = 1;
			}*/
			curColor = getcolor();
			//getfillsettings(curFillColor);
			setcolor(balls[i].color);
			//circle(balls[i].x,balls[i].y,ballSize);
			setfillstyle(balls[i].fill, balls[i].color);
			fillellipse(balls[i].x, balls[i].y , ballSize, ballSize);
			//floodfill(balls[i].x , balls[i].y , balls[i].color);
			setcolor(curColor);
			if(balls[i].iterations > 0 && (balls[i].vx != 0 || balls[i].vy !=0)){
				detectMoving = 1;
				changeiter = (double)balls[i].iterations / (double)maxPower;
				//printf("%f" , changeiter);

				if(balls[i].vx != 0){
					//printf(" render before %f %f \n" , balls[0].x,balls[0].y);
					if( i == 0)
						balls[i].x =balls[i].x + balls[i].vx * changeiter * 2;
					else
						balls[i].x =balls[i].x +  balls[i].vx;
					     //	balls[i].vx = ;
				//balls[i].vy = ;

					//printf(" render after %f %f \n" , balls[0].x,balls[0].y);
					//printf("Incrementing: %f" , balls[i].x);
				//ball.vx += ball.ax;
				//printf(" vx %d" ,ball.vx);
				//printf(" d %f" , deceleration);
				//printf(" decel %f", (deceleration*((double)ball.vx)));
				//ball.ax = -(int)(ball.vx/deceleration);
				//printf(" ax %d" ,ball.ax);

				}

				if(balls[i].vy != 0){
					if(i == 0)
						balls[i].y =balls[i].y +  balls[i].vy * changeiter*2;
					else
						balls[i].y =balls[i].y+ balls[i].vy;
					//ball.vy += ball.ay;
					//ball.ay = -(ball.vy/deceleration);
				}
				balls[i].iterations = balls[i].iterations - 1;
				//printf("Iterations : %d" , balls[i].iterations);
				if(balls[i].iterations <= 0){
					balls[i].iterations = 0;
					balls[i].vx = 0;
					balls[i].vy = 0;
				}
				//balls[i] = ball;
				//printf("Ball iteratoins : %f" , balls[i].x);
			}
			else{
				balls[i].iterations = 0;
				balls[i].vx = 0;
				balls[i].vy = 0;
			}


			//setfillstyle(curFillColor->pattern, curFillColor->color);
		}
	}
	//printf("%d %d %d %d", topBoundary, bottomBoundary, leftBoundary, rightBoundary);

	//rectangle(leftBoundary, topBoundary, rightBoundary, bottomBoundary);

	//floodfill(getmaxx()/2, getmaxy()/2, GREEN);
       /*	line(leftBoundary, topBoundary , rightBoundary , topBoundary);
	line(leftBoundary, bottomBoundary , rightBoundary, bottomBoundary);
	line(leftBoundary, topBoundary , leftBoundary , bottomBoundary);
	line(rightBoundary, topBoundary , rightBoundary , bottomBoundary);
	 */
	 //render holes


	 for(i = 0; i< 6; i++){
		int holerect[8];
		curColor = getcolor();
		setcolor(BLACK);
		setfillstyle(SOLID_FILL, BLACK);
		fillellipse(holes[i].x, holes[i].y , holeSize, holeSize);
		holerect[0] = holes[i].rectx1;
		holerect[1] = holes[i].recty1;

		holerect[2] = holes[i].rectx2;
		holerect[3] = holes[i].recty1;

		holerect[4] = holes[i].rectx2;
		holerect[5] = holes[i].recty2;

		holerect[6] = holes[i].rectx1;
		holerect[7] = holes[i].recty2;

		fillpoly(4, holerect);
		setcolor(curColor);

	 }
	 curColor = getcolor();
	 setcolor(WHITE);
	 rectangle(leftBoundary,topBoundary, rightBoundary, bottomBoundary);
	 setcolor(curColor);

	 // render power meter
	 if(moving == 1){
		setcolor(YELLOW);
		//Draw power bar
		line(leftBoundary, bottomBoundary + 30 , leftBoundary + maxPower, bottomBoundary + 30);
		line(leftBoundary, bottomBoundary + 31 , leftBoundary + maxPower, bottomBoundary + 31);
		line(leftBoundary, bottomBoundary + 32 , leftBoundary + maxPower, bottomBoundary + 32);
		setcolor(BLUE);
		//Draw power indicator
		setfillstyle(SOLID_FILL,BLUE);
		fillellipse(leftBoundary + power, bottomBoundary + 27, 3,3);
		//rectangle(leftBoundary + power, bottomBoundary + 28 , leftBoundary + power + 2, bottomBoundary+30);
		power += powerDirection * 5;
		if(power >= maxPower){
			powerDirection = -1;
		}
		if(power <= 0){
			powerDirection = 1;
		}
	 }
	 if(moving == 0){
		power = 0;
		powerDirection = 1;

		// to render aim

		curColor = getcolor();
		//getfillsettings(curFillColor);
		setcolor(RED);
		//circle(balls[i].x,balls[i].y,ballSize);
		setfillstyle(SOLID_FILL, BLACK);
		fillellipse(balls[0].x + (aimDistance * cos(shootAngle * pi/180)), balls[0].y - (aimDistance * sin(shootAngle * pi/180)), 3, 3);
		setcolor(curColor);
	 }
	 if(detectMoving == 0 && moving == 2){
		moving = 0;
		changeTurn = 1;
	 }
	 detectGameEnd = 0;
	 for(i = 1;i<lastBall; i++){
		if(balls[i].doRender == 1 && balls[i].fill == 1){
			detectGameEnd = 1;
		}

	 }
	 if(detectGameEnd == 0){
		gameState = 2;
	 }
	 else{
		detectGameEnd = 0;
		for(i = 1; i<lastBall;i++){
			if(balls[i].doRender == 1 && balls[i].fill == 2){
				detectGameEnd = 1;
			}
		}
		if(detectGameEnd ==0){
			gameState = 2;
		}
	 }
}

void howtoplay(){
	setcolor(4);
       settextstyle(7,0,5);
       outtextxy(100,20,"HOW TO PLAY ?");

       setcolor(5);
       settextstyle(4,0,4);
       outtextxy(20,70,"To set Aim:-");

       setcolor(7);
       settextstyle(5,0,4);
       outtextxy(20,100,"Use 'W' and 'S' keys to set the aim.");
       outtextxy(20,130,"Use 'ENTER' key to fix the aim. ");

       setcolor(5);
       settextstyle(4,0,4);
       outtextxy(20,170,"To set power:-");

       setcolor(7);
       settextstyle(5,0,4);
       outtextxy(20,200,"Once aim is set you will see a power meter");

       setcolor(7);
       settextstyle(5,0,4);
       outtextxy(20,230,"Power increases from left to right");


       setcolor(7);
       settextstyle(5,0,4);
       outtextxy(20,260,"Use any key to set the power of hit.");

       setcolor(7);
       settextstyle(5,0,4);
       outtextxy(20,290,"Player 1 goes for solids");

       setcolor(7);
       settextstyle(5,0,4);
       outtextxy(20,320,"Player 2 goes for stripes");



       setcolor(YELLOW);
       settextstyle(5,0,4);
       outtextxy(20,360,"Press ESC to go back");

       setcolor(YELLOW);
       settextstyle(5,0,4);
       outtextxy(20,400,"Press any key to play");



}


void initAnimateBall(int i,float x, float y, float vx, float vy){
	balls[i].x= x;
	balls[i].y =y;
	balls[i].vx = vx;
	balls[i].vy = vy;
}

void animate(){
	int i,j;
	char c;
	int x = getmaxx()/2, y =getmaxy()/2, r=1,splitr = 20;//,rs = 0;
	//int size=1;
	int angle;
	int fill = SOLID_FILL;
	int color = BROWN;
	char name[] = "POOL";
	char play[] = "P. PLAY";
	char ex[] = "E. EXIT";
	//setpalette(LIGHTGRAY,0);
	//setpalette(0,LIGHTGRAY);
	for(i = 1; i<40; i++){
		cleardevice();
		setcolor(color);
		setfillstyle(fill,color);
		fillellipse(x,y, r,r);
		//setfillstyle(fill,WHITE);
		//fillellipse(x,y,rs,rs);
		delay(25);
		r=r+2;
		//size++;
		//rs++;
	}
	for(i = 1; i<20; i++){
		cleardevice();
		setcolor(color);
		setfillstyle(fill,color);
		fillellipse(x,y,r,r);
		//setfillstyle(fill,WHITE);
		//fillellipse(x,y,rs,rs);
		r-=2;
		//rs--;
		delay(25);
	}
	for(i = 1; i<7; i++){
		cleardevice();
		setcolor(color);
		setfillstyle(fill,color);
		fillellipse(x,y,r,r);
		//setfillstyle(fill,WHITE);
		//fillellipse(x,y,rs,rs);
		r+=2;
		//rs--;
		delay(25);
	}

	for(i = 1; i<7; i++){
		cleardevice();
		setcolor(color);
		setfillstyle(fill,color);
		fillellipse(x,y,r,r);
		//setfillstyle(fill,WHITE);
		//fillellipse(x,y,rs,rs);
		r-=2;
		//rs--;
		delay(25);
	}
	delay(200);

	initAnimateBall(0,getmaxx()/2,getmaxy()/2,10,0);
	initAnimateBall(1,getmaxx()/2,getmaxy()/2,0,10);
	initAnimateBall(2,getmaxx()/2,getmaxy()/2,-10,0);
	initAnimateBall(3,getmaxx()/2,getmaxy()/2,0,-10);
	angle = 45;
	initAnimateBall(4,getmaxx()/2,getmaxy()/2,10*cos(angle*pi/180),10*sin(angle*pi/180));
	angle = -45;
	initAnimateBall(5,getmaxx()/2,getmaxy()/2,10*cos(angle*pi/180),10*sin(angle*pi/180));
	angle = 135;
	initAnimateBall(6,getmaxx()/2,getmaxy()/2,10*cos(angle*pi/180),10*sin(angle*pi/180));
	angle = -135;
	initAnimateBall(7,getmaxx()/2,getmaxy()/2,10*cos(angle*pi/180),10*sin(angle*pi/180));

	for(i = 1; i<55;i++){
		cleardevice();
		setcolor(color);
		setfillstyle(fill,color);

		for(j = 0; j<8; j++){
			balls[j].x += balls[j].vx;
			balls[j].y += balls[j].vy;
			fillellipse(balls[j].x,balls[j].y,splitr,splitr);
		}
		delay(25);
	       //	for(j=0;j<8;j++){
	     //		printf("\n%d %d %f %f",balls[j].x,balls[j].y,balls[j].vx,balls[j].vy);
	   //	}
	     //	c = getch();
	      //	if(c == 'a'){
	     //		break;
	       //	}
	}
	cleardevice();
	//setpalette(0,LIGHTGRAY);
	setcolor(BROWN);
	settextstyle(SANS_SERIF_FONT,HORIZ_DIR,6);
	outtextxy(320 - (textwidth(name)/2), 100-(textheight(name)/2),name);
	settextstyle(SANS_SERIF_FONT,HORIZ_DIR,4);
	outtextxy(320 - (textwidth(play)/2), 250-(textheight(play)/2),play);
	outtextxy(320 - (textwidth(ex)/2), 300 - (textheight(ex)/2), ex);
	setcolor(WHITE);
	//setpalette(LIGHTGRAY,LIGHTGRAY);
	//setpalette(0,BLACK);
}



void handleBallCollision(int i , int j, double dist){
	double velocity;
	int iter;
	double changeiter;
	//printf("sqrt %f" , dist);

	dist = sqrt(dist);
	//printf("printing %d %f" ,i, (balls[i].vx * balls[i].vx)+(balls[i].vy * balls[i].vy));
	//velocity = sqrt((balls[i].vx * balls[i].vx)+(balls[i].vy * balls[i].vy));
	//printf("velocity %d %f %f" , i ,velocity,(balls[i].x - balls[j].x)/dist);
	/*
       //	printf("sqrt2 %f" , dist);
	overlap = (dist - (2 * ballSize)) * 0.5;
       //	printf("overlap %f %f" , dist,overlap);
	balls[i].x -= overlap * ((balls[i].x - balls[j].x)/dist);
	balls[i].y -= overlap * ((balls[i].y - balls[j].y)/dist);

	balls[j].x += overlap * ((balls[i].x - balls[j].x)/dist);
	balls[j].y += overlap * ((balls[i].y - balls[j].y)/dist);
       */

       //overlap = (dist - (2 * ballSize)) * 0.5;
       //	printf("overlap %f %f" , dist,overlap);
       balls[i].vx = fixedVelocity * ((balls[i].x - balls[j].x)/dist) ;
       balls[i].vy = fixedVelocity * ((balls[i].y - balls[j].y)/dist);

       balls[j].vx = fixedVelocity * ((balls[j].x - balls[i].x)/dist);
       balls[j].vy = fixedVelocity * ((balls[j].y - balls[i].y)/dist);
       iter = getMax(balls[i].iterations, balls[j].iterations);
       iter = (balls[i].iterations + balls[j].iterations)/2;

       if(balls[i].iterations != 0 && balls[j].iterations != 0){
		balls[i].iterations = iter;// - ballCollisionLoss;
		balls[j].iterations = iter;// - ballCollisionLoss;
       }
       else{
	   //	if(balls[i].iterations == 0){
			balls[i].iterations = iter;
			balls[j].iterations = iter;
	  //	}
	     //	else if (balls[j].iterations == 0){
	     //		balls[i].iterations = iter - ballCollisionLoss;
	     //		balls[j].iterations = iter;
	     //	}
       }
       if(balls[i].iterations <= 0){
		balls[i].iterations = 2;
       }

       if(balls[j].iterations <= 0 ){
		balls[j].iterations = 2;
       }
       //printf("Velocity of %d : %f %f\n", i, balls[i].vx,balls[i].vy);
       //printf("Velocity of %d : %f %f\n" , j, balls[j].vx, balls[j].vy);
}

void checkBallCollision(){
	int i , j ;
	double dist;
	for(i = 0; i< lastBall ; i++){
		if(balls[i].doRender){
			for(j = i+1 ;j<lastBall; j++){
				if(i != j && balls[j].doRender){
					//printf("power %d %d %d %d",balls[i].x,balls[j].x  ,balls[i].y,balls[j].y);
					dist = ((balls[i].x - balls[j].x)*(balls[i].x - balls[j].x) )
					+ ((balls[i].y - balls[j].y)*(balls[i].y - balls[j].y));
					if(dist < 0){
						dist = -(dist);
					}
					if(dist < (4*ballSize*ballSize)){
						//printf("sending dist %f %d %d " , dist,(4*ballSize*ballSize),ballSize);
						handleBallCollision(i , j ,dist);
						//printf("Collsion Detected %d %d",i, j);
					}
				}
			}
		}
	}
}

void checkHoleCollision(){
	int i , j, x ,y;
	double dist;
	for(i = 0; i<lastBall; i++){
		if(balls[i].doRender){
			for(j = 0; j<6; j++){

				//printf("power %d %d %d %d",balls[i].x,balls[j].x  ,balls[i].y,balls[j].y);
				dist = ((balls[i].x - holes[j].x)*(balls[i].x - holes[j].x) )
				+ ((balls[i].y - holes[j].y)*(balls[i].y - holes[j].y));
				if(dist < 0){
					dist = -(dist);
				}
				if(dist < ((4*ballSize*holeSize) - 100)){
					//printf("llal %f" , dist);
					//getch();
					//printf("sending dist %f %d %d " , dist,(4*ballSize*ballSize),ballSize);
					//handleBallCollision(i , j ,dist);
					//printf("Collsion Detected %d %d",i, j);
					if(i != 0){
						balls[i].doRender = 0;
						balls[i].iterations = 0;
						balls[i].vx = 0;
						balls[i].vy = 0;

						if(playerTurn == 1 && balls[i].fill == player1Fill){
							player1Score += ballPoints;
						}
						else if(playerTurn == 2 && balls[i].fill == player2Fill){
							player2Score += ballPoints;
						}
						if(playerTurn == 1 && balls[i].fill == player2Fill){
							foul = 1;
						}
						else if(playerTurn == 2 && balls[i].fill == player1Fill){
							foul = 1;
						}
						ballHole = 1;
						break;
					}
					else{
						//When white balls falls in hole
						balls[i].iterations = 0;
						foul = 1;
						balls[i].doRender = 0;
						break;
					}
				 }

			}
		}
	}
}

int checkWhiteBallOverlap(){
	int i;
	double dist;
	for( i =1; i<lastBall;i++){
		if(balls[i].doRender){
			dist = ((balls[i].x - balls[0].x)*(balls[i].x - balls[0].x) )
					+ ((balls[i].y - balls[0].y)*(balls[i].y - balls[0].y));
			if(dist < 0){
				dist = -(dist);
			}
			if(dist < (4*ballSize*ballSize)){
				//printf("sending dist %f %d %d " , dist,(4*ballSize*ballSize),ballSize);
				return 1;
				//printf("Collsion Detected %d %d",i, j);
			}
		}
	}
	return 0;
}

void checkBoundaryCollision(){
	int i = 0;
	for(i = 0; i< lastBall; i++){
		//Handle top collision
		if( (balls[i].y - ballSize) <= topBoundary){
			/*if(i == 0){
				printf("top collsion detect");
				printf(" %f %f \n" , balls[i].x,balls[i].y);
			} */
			balls[i].vy = -(balls[i].vy);
			balls[i].iterations -= 5;
			if(balls[i].iterations <= 0 && i != 0){
				balls[i].iterations = 2;
			}
		}
		// Handle bottom collision
		if( (balls[i].y + ballSize) >= bottomBoundary){

			/*if(i == 0){
				printf("bottom collsion detect");
				printf(" %f %f \n" , balls[i].x,balls[i].y);
			} */
			balls[i].vy = -(balls[i].vy);
			balls[i].iterations -= 5;
			if(balls[i].iterations <= 0 && i != 0){
				balls[i].iterations = 2;
			}
		}
		// Handle right collsion
		if( (balls[i].x + ballSize) >= rightBoundary){
			/*if(i == 0){
				printf("right collsion detect");
				printf(" %f %f \n" , balls[i].x,balls[i].y);
			} */
			balls[i].vx = -(balls[i].vx);
			balls[i].iterations -= 5;
			if(balls[i].iterations <= 0 && i != 0){
				balls[i].iterations = 2;
			}
		}
		// Handle left collsion
		if( (balls[i].x - ballSize) <= leftBoundary){
			/*if(i == 0){
				printf(" Left collsion detect");
				printf(" %f %f \n" , balls[i].x,balls[i].y);
			} */
			balls[i].vx = -(balls[i].vx);
			balls[i].iterations -= 5;
			if(balls[i].iterations <= 0 && i != 0){
				balls[i].iterations = 2;
			}
		}

	}

}

void initHoles(){

	//Top left hole
	holes[0].x = leftBoundary + holeSize;
	holes[0].y = topBoundary + holeSize;
	holes[0].rectx1 = leftBoundary;
	holes[0].recty1 = topBoundary;
	holes[0].rectx2 = holes[0].x;
	holes[0].recty2 = holes[0].y;

	//Top middle hole
	holes[1].x = (rightBoundary - leftBoundary)/2;
	holes[1].y = topBoundary + holeSize;
	holes[1].rectx1 = holes[1].x - holeSize;
	holes[1].recty1 = topBoundary;
	holes[1].rectx2 = holes[1].x + holeSize;
	holes[1].recty2 = holes[1].y;
	//Top right hole

	holes[2].x = rightBoundary - holeSize;
	holes[2].y = topBoundary + holeSize;

	holes[2].rectx1 = rightBoundary;
	holes[2].recty1 = topBoundary;
	holes[2].rectx2 = holes[2].x;
	holes[2].recty2 = holes[2].y;
	//Bottom left hole
	holes[3].x = leftBoundary + holeSize;
	holes[3].y = bottomBoundary - holeSize;

	holes[3].rectx1 = leftBoundary;
	holes[3].recty1 = holes[3].y;
	holes[3].rectx2 = holes[3].x;
	holes[3].recty2 = bottomBoundary;
	//Bottom middle hole
	holes[4].x = (rightBoundary - leftBoundary)/2;
	holes[4].y = bottomBoundary - holeSize;

	holes[4].rectx1 = holes[4].x - holeSize;
	holes[4].recty1 = holes[4].y;
	holes[4].rectx2 = holes[4].x + holeSize;
	holes[4].recty2 = bottomBoundary;
	//Bottom right hole
	holes[5].x = rightBoundary - holeSize;
	holes[5].y = bottomBoundary - holeSize;

	holes[5].rectx1 = holes[5].x;
	holes[5].recty1 = holes[5].y;
	holes[5].rectx2 = rightBoundary;
	holes[5].recty2 = bottomBoundary;
}

void initBalls(){
	int center = (topBoundary + bottomBoundary)/2;
	addBall(getmaxx()/2 - 150,center-5, WHITE,1);
	addBall(getmaxx()/2 + 50,center,YELLOW,1);
	addBall(getmaxx()/2 + 70,center- 10,RED,2);
	addBall(getmaxx()/2 + 70 , center + 10,BLUE,1);
	addBall(getmaxx()/2 + 90,center-20,CYAN,2);
	addBall(getmaxx()/2 + 90,center,MAGENTA,1);
	addBall(getmaxx()/2 + 90,center + 20,YELLOW,2);
	addBall(getmaxx()/2 + 110,center + 10,CYAN,1);
	addBall(getmaxx()/2 + 110,center + 30,MAGENTA,2);
	addBall(getmaxx()/2 + 110,center - 10,RED,1);
	addBall(getmaxx()/2 + 110,center - 30,BLUE,2);

}

void main(){
	int driver = DETECT , mode;
	int x ,y;
	//float vx = 0, vy = 0 , ax = 0, ay = 0;
	//int stepx = 30, stepy = 15;
	char keypress;
	int color;
	struct poolBall currBall;
	clrscr();

	initgraph(&driver, &mode, "../BGI");

	topBoundary = (getmaxy()/6);
	bottomBoundary = 3*(getmaxy()/4);
	rightBoundary = getmaxx()-5;
	leftBoundary = 5;

	//printf("%d %d %d %d %d %d",topBoundary, bottomBoundary, leftBoundary, rightBoundary, (getmaxx()/3), 2*(getmaxy()/3));
	//pow(0,2);

	//render();
	//printf("%d %d %d %d %d %d",topBoundary, bottomBoundary, leftBoundary, rightBoundary, (getmaxx()/3), 2*(getmaxy()/3));
	//x = getmaxx()/2;
	//y = getmaxy()/2;

	/*if(detectMouse()){
		showMouse();
	} */
	//printf("ball : %f %f" , balls[0].x, balls[0].vx);
	//balls[0].vx = 5;
	//balls[0].vy = 0;
	//balls[0].iterations = 400;
	while(1){
		//printf(" very first ball : %f %f" , balls[0].x, balls[0].vx);
		//ch = getch();
		//if(ch != 'a'){
		if(gameState == 1){
			if(moving == 0){
				cleardevice();

				if(changeTurn == 1){
					if(foul == 1){
						//foul = 0;
						settextstyle(SANS_SERIF_FONT,HORIZ_DIR,1);
						color = getcolor();
						setcolor(RED);
						outtextxy(leftBoundary, bottomBoundary + 50 , "FOUL");
						setcolor(color);
						if(playerTurn == 1){
							playerTurn = 2;
						}
						else{
							playerTurn = 1;
						}
						balls[0].doRender = 1;
						x = getmaxx()/2 - 150;
						y = (topBoundary + bottomBoundary)/2;
						balls[0].x = x;
						balls[0].y = y;
						while(checkWhiteBallOverlap()){
							balls[0].y = ++y;
							if(y > bottomBoundary - ballSize){
								y = (topBoundary+bottomBoundary)/2;
								x += 10;
							}
						}

					}
					if(ballHole != 1 && foul ==0){
						if(playerTurn == 1){
							playerTurn = 2;
						}
						else{
							playerTurn = 1;
						}
						ballHole = 0;
					}
					ballHole = 0;
					foul = 0;
					changeTurn = 0;
				}

				render();
				keypress = getch();
				if(keypress == 'w'){
					shootAngle += 2;
				}
				else if(keypress == 's'){
					shootAngle -= 2;
				}
				else if(keypress == 13){
					moving = 1;
				}
				else if(keypress == 27){
					break;
				}

			}
			else if(moving == 1){
				if(kbhit()){
					getch();
					moving = 2;
					balls[0].vx = fixedVelocity * cos(shootAngle * pi/180);
					balls[0].vy = -(fixedVelocity * sin(shootAngle * pi/180));
					balls[0].iterations = power;
				}
				cleardevice();
				render();
			}
			else if(moving == 2){
				cleardevice();
				//printf(" first ball : %f %f" , balls[0].x, balls[0].vx);
				checkBallCollision();
				checkBoundaryCollision();
				checkHoleCollision();
				//getch();
				//printf("ball : %f %f" , balls[0].x, balls[0].vx);
				render();
				//printf(" back ball : %f %f" , balls[0].x, balls[0].vx);


			}
			else{
				printf("Invalid state");
				getch();
				break;
			}
			delay(30);
		}
		else if (gameState == 2){
			settextstyle(SANS_SERIF_FONT , 0 , 3);
			if(player1Score > player2Score){
				outtextxy(getmaxx()/2, getmaxy()/2, "Player 1 Wins!!");
			}
			else if (player2Score > player1Score){
				outtextxy(getmaxx()/2, getmaxy()/2, "Player 2 Wins!!");
			}
			else{
				outtextxy(getmaxx()/2, getmaxy()/2, "It's a Draw!!");
			}
			getch();
			gameState=0;
		}
		else if (gameState == 0){
			animate();
			keypress = getch();
			if(keypress == 'p'){
				gameState = 3;

				//initBalls();
				//initHoles();
				//render();
			}
			else if(keypress == 'e'){
				gameState = 0;
				break;
			}
			//render();
		}
		else if(gameState == 3){
			cleardevice();
			howtoplay();
			keypress = getch();
			if(keypress == 27){
				gameState = 0;
			}
			else{
				gameState = 1;
				initBalls();
				initHoles();
				render();
			}
		}

	}

	getch();
	closegraph();
}

