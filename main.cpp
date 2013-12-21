#include <GL/glut.h>
#include <stdlib.h>
#include "three_angles.h"
#include <GL/glx.h>    /* this includes the necessary X headers */
#include <GL/gl.h>
#include <X11/X.h>    /* X11 constant (e.g. TrueColor) */
#include <X11/keysym.h>
using namespace std;

float bodywidth=200;
float bodyheight=50;
float bodylong=350;
float WinWid=1300.0;
float WinHei=800;
float WinDeep=800;
float w=WinWid;   //текущие размеры экрана. изменяются в процессе выполнения программы при растяжении или сжатии окна
float h=WinHei;
float angleX=0;
float angleY=0;
float angleZ=0;
struct axval {
float x;
float y;
float z;};
axval point={100,100,80};
axval L[3]={                               //размеры звеньев ноги
            {70, 20, 50},
            {100, 20, 50},
            {200, 20, 50}
            };
axval o[3][6];                                    //координаты центров
axval ang[3][6]={                                  //углы наклона
                 {{0,0,0},{-0,0,0},{-45,0,0},{45,0,0},{45,0,0},{-45,0,0}},            //вращательные звенья
                 {{0,0,-60},{0,0,30},{0,0,-30},{0,0,-30},{0,0,30},{0,0,30}},     //первое поступательное
                 {{0,0,60},{0,0,-60},{0,0,60},{0,0,60},{0,0,-60},{0,0,-60}}
                 };
void setka()                              //разметка окна
{
glColor3f(0.153,0.525,0.616);
glBegin(GL_QUADS);
 glVertex2f(WinWid-180, 20);
 glVertex2f(WinWid-10, 20);
 glVertex2f(WinWid-10, WinHei-20);
 glVertex2f(WinWid-180,WinHei-20);
glEnd();
};

void parallelepiped(int width, int height, int deep, axval angle, axval center)
{
glPushMatrix();
glTranslatef(center.x, center.y, center.z);
glRotatef(angle.x,0,1,0);
glRotatef(angle.y,1,0,0);
glRotatef(angle.z,0,0,1);
glScalef(1, (float) height/width, (float) deep/width);
glutWireCube(width);
glPopMatrix();
}
axval centerleg(axval o, axval angle, axval legsize, axval dir)
{
axval res;
res.y=o.y+dir.y*(float)legsize.x/2*sin(grad_to_rad(angle.z));
res.x=o.x+dir.x*(float)legsize.x/2*cos(grad_to_rad(angle.z))*cos(grad_to_rad(angle.x));
res.z=o.z+dir.z*(float)legsize.x/2*cos(grad_to_rad(angle.z))*sin(grad_to_rad(angle.x));
return res;
}
axval endleg(axval o, axval angle, axval legsize, axval dir)
{
axval res;
res.y=o.y+dir.y*legsize.x*sin(grad_to_rad(angle.z));
res.x=o.x+dir.x*legsize.x*cos(grad_to_rad(angle.z))*cos(grad_to_rad(angle.x));
res.z=o.z+dir.z*legsize.x*cos(grad_to_rad(angle.z))*sin(grad_to_rad(angle.x));
return res;
}
void legs(float bodywidth, float bodyheight, float bodylong)
{

axval dir[6]= {
{1,1,-1},{-1,-1,1},{1,1,-1},{1,1,-1},{-1,-1,1},{-1,-1,1},                            //вращательные звенья
};
axval opoint[6];
for (int i=0;i<6;i++)
opoint[i].y=(float)-bodyheight/2+L[0].y/2;
opoint[0].x=(float) bodywidth/2;
opoint[0].z=0;
opoint[1].x=-(float) bodywidth/2;
opoint[1].z=0;
opoint[2].x=(float) bodywidth/2;
opoint[2].z=(float) bodylong/2;
opoint[3].x=(float) bodywidth/2;
opoint[3].z=-(float) bodylong/2;;
opoint[4].x=-(float) bodywidth/2;
opoint[4].z=(float) bodylong/2;
opoint[5].x=-(float) bodywidth/2;
opoint[5].z=-(float) bodylong/2;
 for (int i=0;i<6;i++)
 {
 ang[1][i].x=ang[0][i].x;
 ang[2][i].x=ang[0][i].x;
}
 for (int i=0;i<6;i++)
o[0][i]=centerleg(opoint[i],ang[0][i],L[0],dir[i]);
axval beg[2][6];
for(int i=0;i<6;i++)
{
beg[0][i]=endleg(opoint[i],ang[0][i],L[0],dir[i]);
o[1][i]=centerleg(beg[0][i],ang[1][i],L[1],dir[i]);
}
for(int i=0;i<6;i++)
{
beg[1][i]=endleg(beg[0][i],ang[1][i],L[1],dir[i]);
o[2][i]=centerleg(beg[1][i],ang[2][i],L[2],dir[i]);
}
glPushMatrix();
glScalef(1, (float) bodywidth/bodyheight, (float) bodywidth/bodylong);
for (int j=0;j<3;j++)
for (int i=0;i<6;i++)
parallelepiped(L[j].x,L[j].y,L[j].z,ang[j][i],o[j][i]);
glPopMatrix();
}
void Keyboard (unsigned char key, int x, int y)
{
switch (key)
 {
 case 'a': break;
 case 'd': break;
 case 'w': break;
 case 's': break;
 }
}
void SKeyboard(int key, int x, int y)
{
if (key==GLUT_KEY_UP)
   {
   angleY--;
       }
  if (key==GLUT_KEY_DOWN)
   {
   angleY++;
   }
 if (key==GLUT_KEY_RIGHT)
 {
 angleX++;
 }
 if (key==GLUT_KEY_LEFT)
 {
 angleX--;
 }
 if (key==GLUT_KEY_F1)
 {
 angleZ--;
 }
 if (key==GLUT_KEY_F2)
 {
 angleZ++;
 }
}
int deltime (int x0, int x1, int time)
{return time/(abs(x1-x0)-1);}
void topoint(axval point)
{
float a[3];
for (int i=0;i<3;i++)
a[i]=angles(L[0].x,L[1].x,L[2].x,point.x,point.y,point.z)[i];
ang[0][0].x=a[0];
ang[1][0].z=-a[1];
ang[2][0].z=a[2]-a[1];
legs(bodywidth, bodyheight, bodylong);
}
void vstep(int microsec, int steplong, int height, int angle)          //??? ? ???????????
{  int x0=0; int y0=0; int x1,y1; double anr;
   int y, x;
   anr=grad_to_rad(angle);
  int z=height;
  int rz=height-20;
  if ((angle>=0)&&(angle<90))                                            // I
      {
         x1=steplong*cos(anr)+x0;
         y1=steplong*sin(anr)+y0;
         //if (abs(x1)>=abs(y1))
         //{
         static int X=x0;
         int Y;
           if (X<=x1)
         {
         X++;
         Y=(y1-y0)*(X-x0)/(x1-x0)+y0;
         topoint(point);
              }
         if (X>=x0)
        {
              X--;
              Y=(y1-y0)*(X-x0)/(x1-x0)+y0;
              topoint(point);
              }
      //   }
         /*else
            {
            static int Y=y0;
            int X;
            if (Y<=y1)
         {
               Y++;
               X=(x1-x0)*(Y-y0)/(y1-y0)+x0;
               topoint(X,Y,z);
              }
             if (Y>=y0)
        {
              Y--;
                X=(x1-x0)*(Y-y0)/(y1-y0)+x0;
                topoint(X,Y,rz);

              }
           }
      }
  if(angle==90)                                                       //Pi/2
           {
         x1=steplong*cos(anr)+x0;
         y1=steplong*sin(anr)+y0;
             for(y=y0;y<y1;y++)
               {
                  topoint(x,y,z);

               }
             for(y=y1;y>y0;y--)
               {
                  topoint(x,y,rz);

               }
           }
  if((angle>=91)&&(angle<=180))                                    //II
  {
      x1=x0-steplong*cos(Pi-anr);
      y1=steplong*sin(Pi-anr)+y0;
      if (abs(x1)>=abs(y1))
      {
         for (x=x0;x>x1;x--)
           {
             y=(y1-y0)*(x-x0)/(x1-x0)+y0;
             topoint(x,y,z);

           }
         for (x=x1;x<x0;x++)
           {
             y=(y1-y0)*(x-x0)/(x1-x0)+y0;
             topoint(x,y,rz);
           }
      } else
      {
         for (y=y0;y<y1;y++)
           {
             x=(x1-x0)*(y-y0)/(y1-y0)+x0;
             topoint(x,y,z);
           }
         for (y=y1;y>y0;y--)
           {
              x=(x1-x0)*(y-y0)/(y1-y0)+x0;
              topoint(x,y,rz);
            }
      }
 }
if ((angle>=181)&&(angle<270))                                 //III
  {
      x1=x0-steplong*cos(anr-Pi);
      y1=y0-steplong*sin(anr-Pi);
       if (x1<0)
      { int lmax=30;
      x1=x0-lmax*cos(Pi-anr);
      y1=lmax*sin(Pi-anr)+y0;}
      if (abs(x1)>=abs(y1))
      {
         for (x=x0;x>x1;x--)
           {
             y=(y1-y0)*(x-x0)/(x1-x0)+y0;
             topoint(x,y,z);
           }
         for (x=x1;x<x0;x++)
           {
             y=(y1-y0)*(x-x0)/(x1-x0)+y0;
             topoint(x,y,z);
           }
      }
      else
      {
         for (y=y0;y>y1;y--)
           {
             x=(x1-x0)*(y-y0)/(y1-y0)+x0;
             topoint(x,y,z);
           }
         for (y=y1;y<y0;y++)
           {
              x=(x1-x0)*(y-y0)/(y1-y0)+x0;
              topoint(x,y,rz);
            }
      }

  }
if (angle==270)                                                           //3Pi/2
  {
         x1=steplong*cos(Pi/2)+x0;
         y1=y0-steplong*sin(Pi/2);
             for(y=y0;y>y1;y--)
               {
                  topoint(x1,y,z);
               }
             for(y=y1;y<y0;y++)
               {
                  topoint(x1,y,rz);
               }
           }
if ((angle>=270)&&(angle<=360))                                                 //IV
{
      x1=x0+steplong*cos(2*Pi-anr);
      y1=y0-steplong*sin(2*Pi-anr);
      if (abs(x1)>=abs(y1))
      {
         for (x=x0;x<x1;x++)
           {
             y=(y1-y0)*(x-x0)/(x1-x0)+y0;
             topoint(x,y,z);
           }
         for (x=x1;x>x0;x--)
           {
             y=(y1-y0)*(x-x0)/(x1-x0)+y0;
             topoint(x,y,rz);
           }
      }
       else
      {
         for (y=y0;y>y1;y--)
           {
             x=(x1-x0)*(y-y0)/(y1-y0)+x0;
             topoint(x,y,z);
           }
         for (y=y1;y<y0;y++)
           {
              x=(x1-x0)*(y-y0)/(y1-y0)+x0;
              topoint(x,y,rz);
            }
      }
      */
 }
}
void change_point()
{
static int d=1;
point.x+=1*d;
if (point.x<=100) d=1;
if (point.x>=200) d=-1;
}

void draw()
{glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// Очистка экрана и буфера глубины
glPushMatrix();           //сохранить текущую матрицу
glScalef(w/WinWid,h/WinHei,1);
setka();
glTranslatef(WinWid/2-70,WinHei/2,0);
glRotatef(angleX,0,1,0);
glRotatef(angleY,1,0,0);
glRotatef(angleZ,0,0,1);
glScalef(1, (float) bodyheight/bodywidth, (float) bodylong/bodywidth);
glutWireCube(bodywidth);
change_point();
topoint(point);
//vstep(1,50,80,20);
glPopMatrix();
glutSwapBuffers();  //завершение функции рисования только для GLUT_DOUBLE
}

void  MousePressed(int bottom, int state, int ax, int ay)
{
}
void MousePressedMove(int ax, int ay)
{
}

void Timer(int value)
{
         //Draw();
        glutPostRedisplay();
        glutTimerFunc(1, Timer, 0);
}
void reshape (int ax, int ay)
{ w=ax;
  h=ay;
  glViewport(0,0,ax,ay);
glMatrixMode(GL_PROJECTION); //команды относятся к проекту
glLoadIdentity(); //считывает текущую матрицу
glOrtho(0,ax, ay, 0, WinDeep, -WinDeep);
glMatrixMode(GL_MODELVIEW); //команды относятся к модели
}
void Initialize()
{glClearColor(0.0, 0.0, 0.0, 1.0); //установка цвета очистки
glMatrixMode(GL_PROJECTION); //команды относятся к проекту
glLoadIdentity(); //считывает текущую матрицу
glOrtho(0,WinWid,WinHei,0,WinDeep,-WinDeep); //устанавливает систему координат
glMatrixMode(GL_MODELVIEW); //команды относятся к модели
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv); //инициализация режима
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB); //два буффера, цветовой режим RGB
	glutInitWindowSize(WinWid,WinHei); //размер окна
	glutInitWindowPosition(0, 0); //начало координат окна
	glutCreateWindow("Hexopod move"); //название окна
glutDisplayFunc(draw); //выполнение функции draw
glutReshapeFunc(reshape);
glutMotionFunc(MousePressedMove);
glutMouseFunc(MousePressed);
glutSpecialFunc(SKeyboard);
glutKeyboardFunc(Keyboard);
 glutTimerFunc(1, Timer, 0);
 Initialize(); //выполнение функции инициализации
	glutMainLoop(); //завершающая функция
	return 0;
}
