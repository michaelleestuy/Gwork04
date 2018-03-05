#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PI 3.141592654

struct matrix {
  int entries;
  float array[4][40];
};

struct matrix* make_new(){
  struct matrix * b = malloc(sizeof(struct matrix));
  (*b).entries = 0;
  return b;
}



void add_entry(struct matrix * b, float x0, float y0, float z0, float b0, float x1, float y1, float z1, float b1){
  b->array[0][b->entries] = x0;
  b->array[1][b->entries] = y0;
  b->array[2][b->entries] = z0;
  b->array[3][b->entries] = b0;
  b->array[0][b->entries + 1] = x1;
  b->array[1][b->entries + 1] = y1;
  b->array[2][b->entries + 1] = z1;
  b->array[3][b->entries + 1] = b1;
  b->entries += 2;
}

struct matrix* make_identity(){
  struct matrix *b = make_new();
  add_entry(b, 1, 0, 0, 0, 0, 1, 0, 0);
  add_entry(b, 0, 0, 1, 0, 0, 0, 0, 1);
}

void print_matrix(struct matrix * b){
  int i, j;
  for(i = 0; i < 4; i++){
    for(j = 0; j < b->entries; j++){
      printf("%f ", b->array[i][j]);      
    }
    printf("\n");
  }
}

void multiply(struct matrix * a, struct matrix * b){
  int counter = b->entries;
  float w = 0;
  float x = 0;
  float y = 0;
  float z = 0;

  int i;
  for(i = 0; i < counter; i++){
    w = a->array[0][0] * b->array[0][i] + a->array[0][1] * b->array[1][i] + a->array[0][2] * b->array[2][i] + a->array[0][3] * b->array[3][i];
    x = a->array[1][0] * b->array[0][i] + a->array[1][1] * b->array[1][i] + a->array[1][2] * b->array[2][i] + a->array[1][3] * b->array[3][i];
    y = a->array[2][0] * b->array[0][i] + a->array[2][1] * b->array[1][i] + a->array[2][2] * b->array[2][i] + a->array[2][3] * b->array[3][i];
    z = a->array[3][0] * b->array[0][i] + a->array[3][1] * b->array[1][i] + a->array[3][2] * b->array[2][i] + a->array[3][3] * b->array[3][i];
    b->array[0][i] = w;
    b->array[1][i] = x;
    b->array[2][i] = y;
    b->array[3][i] = z;    
  }
}

void linemaker(char image[500][500], int x, int y, int ex, int ey){
  float dx = ex - x;
  float dy = ey - y;
  if( (dx < 0 && dy < 0) || (dx > 0 && dy < 0) || (dx == 0 && dy < 0) || (dy == 0 && dx < 0)){
    float tx = x;
    float ty = y;
    x = ex;
    y = ey;
    ex = tx;
    ey = ty;
    dx = ex - x;
    dy = ey - y;
  }
  float c = dx * y - dy * x;
  if(dy > 0 && dx < 0){
    float d =  2 * dy * (x + 1) - dx * (2 * y + 1) + 2 * c;
    if(dx + dy <= 0){
       while(x >= ex){
	 image[x][y] = 'r';
	 if(d < 0){
	   d -= 2 * dx;
	   y++;
	 }
	 d -= 2 * dy;
	 x--;
       }
    }
    else{
      while(y <= ey){
	 image[x][y] = 'r';
	 if(d > 0){
	   d -= 2 * dy;
	   x--;
	 }
	 d -= 2 * dx;
	 y++;
       }
    }
  } 
  else if(dx >= dy){
    float d =  2 * dy * (x + 1) - dx * (2 * y + 1) + 2 * c;   
    while(x <= ex){
      image[x][y] = 'r';
      if(d > 0){
	d -= 2 * dx;
	y++;
      }
      d += 2 * dy;
      x++;
    }
  }  
  else if(dy > dx){
    float d =  dy * (2 * x + 1) - dx * (2 * y) + 2 * c;   
    while(y <= ey){
      image[x][y] = 'r';
      if(d < 0){
	d += 2 * dy;
	x++;
      }
      d -= 2 * dx;
      y++;
    }
  }
}

void edgemaker(struct matrix * a, char image[500][500]){
  int counter = a->entries;
  int i;
  for(i = 0; i < counter; i+=2){
    linemaker(image, (int)round(a->array[0][i]), (int)round(a->array[1][i]), (int)round(a->array[0][i+1]), (int)round(a->array[1][i+1]));
  }
}


struct matrix * translation(float x, float y, float z){
  struct matrix * b = make_identity();
  b->array[0][3] = x;
  b->array[1][3] = y;
  b->array[2][3] = z;
  return b;
}

struct matrix * scale(float x){
  struct matrix * b = make_identity();
  b->array[0][0] = x;
  b->array[1][1] = x;
  b->array[2][2] = x;
  return b;
}

struct matrix * z_rotation(float x){ //x-y
  struct matrix * b = make_identity();
  
  b->array[0][0] = cosf(x * PI / 180);
  b->array[0][1] = -1 * sinf(x * PI / 180);
  b->array[1][0] = sinf(x * PI / 180);
  b->array[1][1] = cosf(x * PI / 180);
  
  return b;
}

struct matrix * y_rotation(float x){ //z-x
  struct matrix * b = make_identity();
  
  b->array[2][2] = cosf(x * PI / 180);
  b->array[2][0] = -1 * sinf(x * PI / 180);
  b->array[0][2] = sinf(x * PI / 180);
  b->array[0][0] = cosf(x * PI / 180);
  
  return b;
}

struct matrix * x_rotation(float x){ //y-z
  struct matrix * b = make_identity();
  
  b->array[1][1] = cosf(x * PI / 180);
  b->array[1][2] = -1 * sinf(x * PI / 180);
  b->array[2][1] = sinf(x * PI / 180);
  b->array[2][2] = cosf(x * PI / 180);
  
  return b;
}

void main(){
  
}
