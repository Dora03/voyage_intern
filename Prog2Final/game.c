#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <GLUT/glut.h>
#include "game.h"
#define MAX_ORDER 10
int CreateMaze(int *maze){//迷路生成
  int xi, yi;
  int route[WIDTH*HEIGHT], max=0;

  for(yi=0; yi<HEIGHT; yi++){//全部を壁に初期化、外枠は道に初期化
    for(xi=0; xi<WIDTH; xi++){
      if(xi==0||xi==(WIDTH-1)||yi==0||yi==(HEIGHT-1))
        maze[xi + yi*WIDTH] = 0;
      else maze[xi + yi*WIDTH] = 1;
      route[xi + yi*WIDTH] = 0;
    }
  }

  srand((unsigned)time(NULL));//乱数系列の変更
  //左下からスタート
  int x = 2;
  int y = 2;

  int sign;//プラスなら1マイナスなら-1
  int vertical;//xなら0,yなら1

  maze[x + y*WIDTH]=0;

  int s=0;//繰り返しを制限する
  while(s<10000){
    sign = (rand()%2)*2-1;
    vertical = rand()%2;
    int m = x + y*WIDTH + sign*(1+vertical*(WIDTH-1));//進みたいマス
    //1つ先から見て自分がいるマス以外の全方位が壁なら道を進める
    if((maze[m+1]==1)+(maze[m-1]==1)+(maze[m+WIDTH]==1)+(maze[m-WIDTH]==1)==3){
      maze[m]=0;
      route[m]=route[x + y*WIDTH]+1;
      if(route[m]>route[max])
        max = m;
      if(vertical==0)
        x += sign;
      else y += sign;
      s++;
    }
    else{//道なら新たなマスに飛ぶ
      while(1){
        x = rand()%(WIDTH-4)+2;
        y = rand()%(HEIGHT-4)+2;
        if(maze[x + y*WIDTH]==0)
          break;
      }//while
    }//else
  }//while
  return max;//最長経路をゴールとし、そのゴール地点の座標をreturn
}
int wall_road(int *maze, int x, int y){
  /*ピクセルで受け取った座標をmazeのマス座標に変換*/
  x /= 20;
  y /= 20;
  /*道なら1を、壁なら0を返す*/
  if(maze[x + y*WIDTH]==0)
    return 1;
  else return 0;
}
int LoadOrder(const char *filename, Order *order, double *latest_time){
  FILE *fp;
  int i;
  fp = fopen(filename, "rb");
  if(fp==NULL){
    printf("Can't open file %s\n", filename);
    return -1;
  }
  for(i=0; i<MAX_ORDER; i++){
    if(fscanf(fp, "%s %lf", order[i].name, &order[i].time)==-1)
      break;
  }
  if(i!=0)//テキストファイルにデータが1つでもあったら
    *latest_time = order[i-1].time;
  else *latest_time = -1;//なかったら-1を最長時間に代入
  fclose(fp);
  return i;//ランキングに入っているデータの数を返す
}
int SaveOrder(const char *filename, Order *order, double *latest_time){
  FILE *fp;
  Order ord[10];
  int i, n, count=0, repetition=0;

  n = LoadOrder(filename, ord, &*latest_time);
  if(n==-1){
    printf("error: LoadOrder in function ReOrder\n");
    return -1;
  }
  fp = fopen(filename, "wb");
  if(fp==NULL){
    printf("Can't open file %s\n", filename);
    return -1;
  }
  if(n==0)
    fprintf(fp, "%s %lf\n", order->name, order->time);

  else{
    if(n==10)
      n--;
    for(i=0; i<n; i++){
      if(ord[i].time>order->time&&count==0){
	fprintf(fp, "%s %lf\n", order->name, order->time);
	if(*latest_time<order->time)
	  *latest_time = order->time;
	count=1;
      }//if
      if(ord[i].time==order->time&&strcmp(ord[i].name, order->name)==0){
	repetition=1;
	count = 1;
      }
      if(repetition==0)
	fprintf(fp, "%s %lf\n", ord[i].name, ord[i].time);
    }
    if(count==0)
      fprintf(fp, "%s %lf\n", order->name, order->time);
  }
  fclose(fp);
  return i;
}
