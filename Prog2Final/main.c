#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <GLUT/glut.h>

#include "game.h"
#include "TextureImage.h"
#include "GenericTimer.h"
#include "ImageData.h"
#include "ImageIO.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define MAX_NUM_GAME 1024
#define START_FILE "MAZEMON.ppm"
#define IMAGE_FILE "knmr.ppm"
#define GOAL_FILE "goal.ppm"
#define ROAD_FILE "road.ppm"
#define WALL_FILE "wall.ppm"
#define RANKIN_FILE "orderin.ppm"
#define RANK_FILE "order.ppm"
#define ORDER_FILE "ranking.txt"

ImageData g_Image, g_Goal, g_Start, g_Road, g_Wall, g_Rankin, g_Rank;
int g_WindowWidth = WIDTH*20;
int g_WindowHeight = HEIGHT*20;
const int g_MaxLineCount = 32;
int g_KeyboardInputCount = 0;
char g_MessageBuffer[BUFSIZE];

int maze[WIDTH*HEIGHT]; //迷路50x20が壁が道か、壁なら1道なら0,その外枠分も
int goal;//ゴール地点のマス座標を記憶
int mode = 0;//0:スタート画面
double take_time;
double latest_time=BUFSIZE;
Order order, ord[10];
int load;//loadorderの返り値を受け取る
char ord_time[10];

TextureImage g_Tex;
/* 画像の左下点の、初期 x 座標 */
int g_ImagePosX = 40.0;
/* 画像の左下点の、初期 y 座標 */
int g_ImagePosY = 40.0;
int g_CursorDelta = 20;

int g_MessageStartX = 470;
int g_MessageStartY = 253;

enum MouseOperation{
  NO_OPERATION,
  DRAGGING_IMAGE
};

int g_DraggingImage = NO_OPERATION;

double g_PrevTime = 0.0;
//初期化処理
void init(void){
  glClearColor(1.0, 1.0, 1.0, 1.0);//ウィンドウを消去するときの色を設定

  InitImageData(&g_Image);//画像データを初期化
  if(LoadPPMImage(IMAGE_FILE, &g_Image)){//画像データの読み込みに成功
    FlipImageData(&g_Image);//画像の上下反転
  }

  InitImageData(&g_Goal);
  if(LoadPPMImage(GOAL_FILE, &g_Goal)){//画像データの読み込みに成功
    FlipImageData(&g_Goal);//画像の上下反転
  }

  InitImageData(&g_Start);
  if(LoadPPMImage(START_FILE, &g_Start)){//画像データの読み込みに成功
    FlipImageData(&g_Start);//画像の上下反転
  }

  InitImageData(&g_Road);
  if(LoadPPMImage(ROAD_FILE, &g_Road)){//画像データの読み込みに成功
    FlipImageData(&g_Road);//画像の上下反転
  }

  InitImageData(&g_Wall);
  if(LoadPPMImage(WALL_FILE, &g_Wall)){//画像データの読み込みに成功
    FlipImageData(&g_Wall);//画像の上下反転
  }

  InitImageData(&g_Rankin);
  if(LoadPPMImage(RANKIN_FILE, &g_Rankin)){//画像データの読み込みに成功
    FlipImageData(&g_Rankin);//画像の上下反転
  }

  InitImageData(&g_Rank);
  if(LoadPPMImage(RANK_FILE, &g_Rank)){
    FlipImageData(&g_Rank);
  }

  goal = CreateMaze(maze);
}
/*座標(x, y)に文字を表示*/
void DisplayString(const char *str, int xi, int yi){

  int i, yStart = yi;

  glRasterPos2i(xi, yStart);

  for(i=0; str[i]!='\0'; i++){
    if(str[i]=='\n'){
      yStart -= 24;
      glRasterPos2i(xi, yStart);
    }
    else glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
  }
  glRasterPos2i(0, 0);

}

//表示処理のためのコールバック関数
void display(void){
  int i;
//ウィンドウを消去...glClearColorで指定した色で塗りつぶし
  glClear(GL_COLOR_BUFFER_BIT);
  if(mode==0){
  if(IsImageDataAllocated(&g_Start)){
    DrawImageData(&g_Start, 0, 0);
    }
  glutSwapBuffers();
  }
  else if(mode==1){
  int xi, yi;
  float size = 20.0;//マスの大きさ
  float cx = g_WindowWidth/2, cy=g_WindowHeight/2;//中心の座標
    for(xi = 0; xi<WIDTH; ++xi){
      for(yi = 0; yi<HEIGHT; ++yi){
	int posx = xi - WIDTH/2, posy = yi - HEIGHT/2;
	if(maze[xi+yi*WIDTH]&&xi+yi*WIDTH!=goal){//wall
	  if(IsImageDataAllocated(&g_Wall)){
	    DrawImageData(&g_Wall, cx+posx*size, cy+posy*size);
	  }
	}
	if(!maze[xi+yi*WIDTH]&&xi+yi*WIDTH!=goal){//road
	  if(IsImageDataAllocated(&g_Road)){
	    DrawImageData(&g_Road,cx+posx*size,cy+posy*size);
	  }
	}
	if(xi+yi*WIDTH==goal){//goal
	  if(IsImageDataAllocated(&g_Goal)){
	    DrawImageData(&g_Goal, cx+posx*size, cy+posy*size);
	  }
	}//ifgoal
      }//fory
    }//forx
    glutSwapBuffers();
    /*スタート地点*/
    glBegin(GL_QUADS);
    glColor4f(0.9, 1.0, 0.2, 0.0);//カラー
    glVertex2f(cx+(GLfloat)(2-WIDTH/2  )*size, cy+(GLfloat)(2-HEIGHT/2  )*size);
    glVertex2f(cx+(GLfloat)(2-WIDTH/2+1)*size, cy+(GLfloat)(2-HEIGHT/2  )*size);
    glVertex2f(cx+(GLfloat)(2-WIDTH/2+1)*size, cy+(GLfloat)(2-HEIGHT/2+1)*size);
    glVertex2f(cx+(GLfloat)(2-WIDTH/2  )*size, cy+(GLfloat)(2-HEIGHT/2+1)*size);

    glEnd();
  glFlush();

  if(IsImageDataAllocated(&g_Image)){
    DrawImageData(&g_Image, g_ImagePosX, g_ImagePosY);
    }
  glutSwapBuffers();
  }//else
  else if(mode==2){//名前を書き込む
    if(IsImageDataAllocated(&g_Rankin)){
      DrawImageData(&g_Rankin, 0.0, 0.0);
    }
    glutSwapBuffers();
    glColor3ub(0,0,0);
    DisplayString(g_MessageBuffer, g_MessageStartX, g_MessageStartY);

    glutSwapBuffers();
  }//else2
  else if(mode==3){
if(IsImageDataAllocated(&g_Rank)){
    DrawImageData(&g_Rank, 0.0, 0.0);
    }
  glutSwapBuffers();
  glColor3ub(0, 0, 0);
  load = LoadOrder(ORDER_FILE, ord, &latest_time);
  DisplayString("TIME", g_MessageStartX-150, g_MessageStartY+225);
  DisplayString("NAME", g_MessageStartX-50, g_MessageStartY+225);
  for(i=0; i<load; i++){
    sprintf(ord_time, "%lf", ord[i].time);
    DisplayString(ord_time,g_MessageStartX-150,g_MessageStartY+20*(10-i));
    DisplayString(ord[i].name,g_MessageStartX-50,g_MessageStartY+20*(10-i));
    } 
  
  glutSwapBuffers();
  }//else3
}

//定期的に呼び出され、画像の位置を更新するための関数
void idle(void){
  if(g_ImagePosX/20+WIDTH*g_ImagePosY/20==goal){//ゴールにたどり着いたら
    take_time = GetTime()/1000;

   load = LoadOrder(ORDER_FILE, ord, &latest_time);

    if(latest_time>take_time||load<10){
      order.time = take_time;
      mode = 2;
    }
    else mode = 3;
    display();

    goal++;
  }
}
//キーボード入力のためのコールバック関数
void keyboard(unsigned char key, int x, int y){
  if(mode!=2){//名前入力画面ではqやnキーを押してもコマンドを発生させない
    switch(key){
    case 13:
    case ' ':
      if(mode==0){
	mode=1;
	StartTimer();//計測開始
      }
      break;
    case 'n':
    case 'N':
      mode = 0;
      g_ImagePosX = 40.0;
      g_ImagePosY = 40.0;
      init();
      break;
    case 'q':
    case 'Q':
    case '\033':
      exit(-1);
      break;
    default:
      break;
    }
  }
  if(key=='\033')
    exit(-1);
    if(g_KeyboardInputCount > 0 && key == 8) /* key == 8 ... Backspace */
      {
	g_KeyboardInputCount--;
	g_MessageBuffer[g_KeyboardInputCount] = '\0';
      }
    if(g_KeyboardInputCount > 0 && key == 13){//Enter
      if(mode==3){//ランキング上位を表示中
      mode = 0;
      g_ImagePosX = 40.0;
      g_ImagePosY = 40.0;
      init();
      }
      else{
      strcpy(order.name, g_MessageBuffer);
      SaveOrder(ORDER_FILE, &order, &latest_time);
      }
      if(mode==2)
	mode=3;
    }
    else
      {
	if (g_KeyboardInputCount > 0 && g_KeyboardInputCount % g_MaxLineCount == 0)
	  g_MessageBuffer[g_KeyboardInputCount++] = '\r';

	g_MessageBuffer[g_KeyboardInputCount++] = key;

	if (g_KeyboardInputCount > BUFSIZE-1)
	  g_KeyboardInputCount = BUFSIZE-1;

	g_MessageBuffer[g_KeyboardInputCount] = '\0';
      }
    glutPostRedisplay();    /* ウィンドウ描画関数を呼ぶ */
}
/*特殊キー入力のためのコールバック関数*/
void special(int key, int x, int y){
  switch(key){
  case GLUT_KEY_UP://上
    if(wall_road(maze, g_ImagePosX, g_ImagePosY+g_CursorDelta))
       g_ImagePosY += g_CursorDelta;
    if (g_WindowHeight - g_Image.height <= g_ImagePosY)
      g_ImagePosY = g_WindowHeight - g_Image.height;
    break;
  case GLUT_KEY_DOWN://下
    if(wall_road(maze, g_ImagePosX, g_ImagePosY-g_CursorDelta))
      g_ImagePosY -= g_CursorDelta;
    if (g_ImagePosY < 0)
      g_ImagePosY = 0;
    break;
  case GLUT_KEY_RIGHT://右
    if(wall_road(maze, g_ImagePosX+g_CursorDelta, g_ImagePosY))
      g_ImagePosX += g_CursorDelta;
    if (g_WindowWidth - g_Image.width <= g_ImagePosX)
      g_ImagePosX = g_WindowWidth - g_Image.width;
    break;
  case GLUT_KEY_LEFT://左
    if(wall_road(maze, g_ImagePosX-g_CursorDelta, g_ImagePosY))
      g_ImagePosX -= g_CursorDelta;
    if (g_ImagePosX < 0)
      g_ImagePosX = 0;
    break;
  default:
    break;
  }
  glutPostRedisplay();
}
//マウス入力のためのコールバック関数
void mouse(int button, int state, int x, int _y){
  if(mode==0){
    mode=1;
    StartTimer();//計測開始
  }
  glutPostRedisplay();

}
//ウィンドウサイズが変更されたときのためのコールバック関数
void reshape(int w, int h){
  if(h<1)return;

  g_WindowWidth = w;
  g_WindowHeight = h;

  glViewport(0, 0, w, h);//ウィンドウの描画範囲を指定

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, w, 0, h);

  glutPostRedisplay(); //ウィンドウ描画関数を呼ぶ
}
int main(int argc, char *argv[]){
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA);
  glutInitWindowSize(g_WindowWidth, g_WindowHeight);
  glutCreateWindow("MAZE");
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  glutMouseFunc(mouse);
  glutReshapeFunc(reshape);
  init();
  glutMainLoop();
  return 0;
}
