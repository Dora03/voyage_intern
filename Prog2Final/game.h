#ifndef __GAME_H__
#define __GAME_H__
#define WIDTH 42
#define HEIGHT 32
#define BUFSIZE 8192
typedef struct{
  char name[BUFSIZE];
  double time;
}Order;
int CreateMaze(int *maze);
int wall_road(int *maze, int x, int y);
int LoadOrder(const char *filename, Order *order, double *latest_time);
int SaveOrder(const char *filename, Order *order, double *latest_time);
#endif
