#include <GL/glut.h>
#include <stdlib.h>
#include "three_angles.h"
#include <GL/glx.h>    /* this includes the necessary X headers */
#include <GL/gl.h>
#include <X11/X.h>    /* X11 constant (e.g. TrueColor) */
#include <X11/keysym.h>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include "send_data.h"
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
float z;
};
axval point={180,0,190};
axval point0={180,0,190};
axval L[3]={                               //размеры звеньев ноги
            {80, 20, 50},
            {100, 20, 50},
            {190, 20, 50}
            };
axval o[3][6];                                    //координаты центров
axval ang[3][6]={                                  //углы наклона
                 {{0,0,0},{-0,0,0},{-45,0,0},{45,0,0},{45,0,0},{-45,0,0}},            //вращательные звенья
                 {{0,0,-60},{0,0,30},{0,0,-30},{0,0,-30},{0,0,30},{0,0,30}},     //первое поступательное
                 {{0,0,60},{0,0,-60},{0,0,60},{0,0,60},{0,0,-60},{0,0,-60}}
                 };
struct leg_angles {
int Q1;
int Q2;
int Q3;
};
leg_angles step_angles[2][500];  //[0][i] grad; [1][i]
int direction=0;
void draw_string_bitmap(void *font, const char* string)
{
  while (*string)
    glutBitmapCharacter(font, *string++);
}
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
void navigation(int angle)
{
axval O, p;
int l=50;
O.x=100;
O.y=70;
O.z=0;
glColor3f(0.653,0.625,0.616);
glPushMatrix();
glTranslatef(O.x, O.y, O.z);
glutSolidSphere(l,50,50);
if ((direction>=0)&&(direction<90))
{
p.x=-l*sin(grad_to_rad(direction));
p.y=-l*cos(grad_to_rad(direction));
}
if ((direction>=90)&&(direction<180))
{
p.x=-l*sin(grad_to_rad(180-direction));
p.y=l*cos(grad_to_rad(180-direction));
}
if ((direction>=180)&&(direction<270))
{
p.x=l*sin(grad_to_rad(direction-180));
p.y=l*cos(grad_to_rad(direction-180));
}
if ((direction>=270)&&(direction<360))
{
p.x=l*sin(grad_to_rad(360-direction));
p.y=-l*cos(grad_to_rad(360-direction));
}
glColor3f(0.153,0.525,0.616);
glBegin(GL_LINES);
glVertex2f(0,0);
glVertex2f(p.x,p.y);
glEnd();
glPopMatrix();
glColor3f(0,0,0);
glRasterPos3f(WinWid-150,50,0);
char *a=to_char(direction);
draw_string_bitmap(GLUT_BITMAP_HELVETICA_12, "Direction Angle=");
draw_string_bitmap(GLUT_BITMAP_HELVETICA_12, a);
}

void parallelepiped(int width, int height, int deep, axval angle, axval center)
{
glColor3f(0.153,0.525,0.616);
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
 case 'a':
           if ((direction>90)&&(direction<270))
           direction--;
           if ((direction>=0)&&(direction<90)||(direction>270)&&(direction<360))
          {
            direction++;
            if (direction>359)
             direction=0;
             }
            break;
 case 'd':
          if ((direction>90)&&(direction<270))
           direction++;
           if ((direction>=0)&&(direction<=90)||(direction>270)&&(direction<360))
             {
             direction--;
             if (direction==-1)
                direction=359;
                }
            break;
 case 'w':
           if ((direction>0)&&(direction<180))
           direction--;
           if ((direction>180)&&(direction<360))
          {
            direction++;
            if (direction==360)
            direction=0;
            }
 break;
 case 's':
           if ((direction>0)&&(direction<180))
           direction++;
           if ((direction>180)&&(direction<360))
            direction--;
 break;
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
void output(char title[], float value, int x, int y)
{
glPushMatrix();
glScalef(1, (float) bodywidth/bodyheight, (float) bodywidth/bodylong);
glRotatef(360-angleZ,0,0,1);
glRotatef(360-angleY,1,0,0);
glRotatef(360-angleX,0,1,0);
glTranslatef(-WinWid/2+70,-WinHei/2,0);
glScalef(WinWid/w,WinHei/h,1);
glColor3f(1,1,1);
glRasterPos3f(x,y,0);
      char *a=to_char(value);
draw_string_bitmap(GLUT_BITMAP_HELVETICA_10, title);
draw_string_bitmap(GLUT_BITMAP_HELVETICA_10, a);
glPopMatrix();
}
void microsec(int n)
{
 for (int i=0;i<n;i++)
 {
 step_angles[1][i].Q1=grad_to_microsec(step_angles[0][i].Q1);
 step_angles[1][i].Q2=grad_to_microsec(step_angles[0][i].Q2);
 step_angles[1][i].Q3=grad_to_microsec(step_angles[0][i].Q3);
}
}
void intof(leg_angles a[2][500], int n)
{
  std::ofstream f("/home/diana/file.txt");
  for (int i=0;i<n;i++)
   {
   f<<"Q1="<<a[0][i].Q1<<"   "<<"Qm1="<<a[1][i].Q1<<"   "<<"Q2="<<a[0][i].Q2<<"   "<<"Qm2="<<a[1][i].Q2<<"   "<<"Q3="<<a[0][i].Q3<<"   "<<"Qm3="<<a[1][i].Q3<<"\n";
   f<<create_string(a[1][i].Q1,a[1][i].Q2,a[1][i].Q3,1,2,3,500);
   }
   f.close();
 };
void step(float direction, float steplong, axval p0)        //от primary direction
{
static float angle;
angle=direction;
float anr=grad_to_rad(angle);
float x1, y1;
int rz=p0.z-40;
static bool dir=true;
struct state
 {
  bool up,down,stable;
 };
static state z={false, false, true};
glColor3f(0.153,0.525,0.616);
  if ((angle>=0)&&(angle<=90))                                            // I
      {
      x1=steplong*sin(anr)+p0.x;
      y1=-steplong*cos(anr)+p0.y;
         }
  if ((angle>90)&&(angle<=180))                                            // II
      {
      x1=steplong*sin(Pi-anr)+p0.x;
      y1=steplong*cos(Pi-anr)+p0.y;
      }
if ((angle>180)&&(angle<=270))                                            // III
      {
      x1=p0.x-steplong*sin(anr-Pi);
      y1=p0.y+steplong*cos(anr-Pi);
      }
 if ((angle>270)&&(angle<360))                                                 //IV
      {
      x1=p0.x-steplong*sin(2*Pi-anr);
      y1=p0.y-steplong*cos(2*Pi-anr);
      }
      //нашли точки конца шага.
static int i=0;
if (abs(x1-p0.x)>=abs(y1-p0.y))
{
        if ((angle>180)&&(angle<=359))
        {
           if ((point.x>x1)&&(dir==true)&&(z.stable))
         {
          point.x--;
          point.y=(y1-p0.y)*(float)(point.x-p0.x)/(x1-p0.x)+p0.y;
          point.z=p0.z;
          topoint(point);
          step_angles[0][i].Q1=ang[0][0].x;
          step_angles[0][i].Q2=-ang[1][0].z;
          step_angles[0][i].Q3=ang[2][0].z+ang[1][0].z;
          i++;
          if (point.x<=x1)
           {
            dir=false;
            z.up=true;
            z.stable=false;
            }
             }
        if ((point.x<=p0.x)&&(dir==false)&&(z.stable))
         {
         point.x++;
         point.y=(y1-p0.y)* (float) (point.x-p0.x)/(x1-p0.x)+p0.y;
         point.z=rz;
         topoint(point);
          step_angles[0][i].Q1=ang[0][0].x;
          step_angles[0][i].Q2=-ang[1][0].z;
          step_angles[0][i].Q3=ang[2][0].z+ang[1][0].z;
          i++;
         if (point.x>=p0.x)
          {
            dir=true;
            z.down=true;
            z.stable=false;
           }
           }
        }
if ((angle>0)&&(angle<=180))
        {
           if ((point.x<x1)&&(dir==true)&&(z.stable))
         {
         point.x++;
         point.y=(y1-p0.y)*(float)(point.x-p0.x)/(x1-p0.x)+p0.y;
         point.z=p0.z;
         topoint(point);
          step_angles[0][i].Q1=ang[0][0].x;
         step_angles[0][i].Q2=-ang[1][0].z;
          step_angles[0][i].Q3=ang[2][0].z+ang[1][0].z;
          i++;
         if (point.x>=x1)
          {
            dir=false;
            z.up=true;
            z.stable=false;
           }
           }
           if ((point.x>p0.x)&&(dir==false)&&(z.stable))
         {
          point.x--;
          point.y=(y1-p0.y)*(float)(point.x-p0.x)/(x1-p0.x)+p0.y;
          point.z=rz;
          topoint(point);
          step_angles[0][i].Q1=ang[0][0].x;
         step_angles[0][i].Q2=-ang[1][0].z;
          step_angles[0][i].Q3=ang[2][0].z+ang[1][0].z;
          i++;
           if (point.x<=p0.x)
            {
             dir=true;
             z.down=true;
             z.stable=false;
            }
          }
        }
}
else
if (abs(x1-p0.x)<abs(y1-p0.y))
{
        if ((angle>=90)&&(angle<270))
    {
         if ((point.y<y1)&&(dir==true)&&(z.stable))
        {
          point.y++;
          point.x=(x1-p0.x)*(float)(point.y-p0.y)/(y1-p0.y)+p0.x;
          point.z=p0.z;
          topoint(point);
          step_angles[0][i].Q1=ang[0][0].x;
         step_angles[0][i].Q2=-ang[1][0].z;
          step_angles[0][i].Q3=ang[2][0].z+ang[1][0].z;
          i++;
         if (point.y>=y1)
          {
            dir=false;
            z.up=true;
            z.stable=false;
           }
        }
         if ((point.y>p0.y)&&(dir==false)&&(z.stable))
        {
              point.z=rz;
              point.y--;
              point.x=(x1-p0.x)*(float)(point.y-p0.y)/(y1-p0.y)+p0.x;
              topoint(point);
          step_angles[0][i].Q1=ang[0][0].x;
          step_angles[0][i].Q2=-ang[1][0].z;
          step_angles[0][i].Q3=ang[2][0].z+ang[1][0].z;
          i++;
              if (point.y<=p0.y)
              {
               dir=true;
               z.stable=false;
               z.down=true;
              }
        }
    }
    if (((angle>=0)&&(angle<90))||((angle>=270)&&(angle<359)))
        {
         if ((point.y>y1)&&(dir==true)&&(z.stable))
         {
          point.y--;
          point.x=(x1-p0.x)*(float)(point.y-p0.y)/(y1-p0.y)+p0.x;
          point.z=p0.z;
          topoint(point);
          step_angles[0][i].Q1=ang[0][0].x;
          step_angles[0][i].Q2=-ang[1][0].z;
          step_angles[0][i].Q3=ang[2][0].z+ang[1][0].z;
          i++;
         if (point.y<=y1)
          {
            dir=false;
            z.up=true;
            z.stable=false;
           }
           }
         if ((point.y<p0.y)&&(dir==false)&&(z.stable))
        {
              point.z=rz;
              point.y++;
              point.x=(x1-p0.x)*(float)(point.y-p0.y)/(y1-p0.y)+p0.x;
              topoint(point);
          step_angles[0][i].Q1=ang[0][0].x;
          step_angles[0][i].Q2=-ang[1][0].z;
          step_angles[0][i].Q3=ang[2][0].z+ang[1][0].z;
          i++;
              if (point.y>=p0.y)
              {
               dir=true;
               z.stable=false;
               z.down=true;
              }
        }
    }
}
if (z.down)
  {
     point.z++;
     topoint(point);
      step_angles[0][i].Q1=ang[0][0].x;
          step_angles[0][i].Q2=-ang[1][0].z;
          step_angles[0][i].Q3=ang[2][0].z+ang[1][0].z;
    i++;
       if (point.z>=p0.z)
        {
        z.down=false;
        z.stable=true;
        microsec(i);
        intof(step_angles,i);
        i=0;
        }
    }
 if (z.up)
{
  point.z--;
  topoint(point);
   step_angles[0][i].Q1=ang[0][0].x;
   step_angles[0][i].Q2=-ang[1][0].z;
   step_angles[0][i].Q3=ang[2][0].z+ang[1][0].z;
   i++;
    if (point.z<=rz)
    {
        z.up=false;
        z.stable=true;
     }
}
}

void coordsys(axval o)
{
//point0={180,0,190};   x,y,z
glColor3f(0.5,0.5,0.5);
glBegin(GL_LINES);
 glVertex3f(290-200,697,o.y);         //x
 glVertex3f(290+200, 697, o.y);
 glVertex3f(290,697-900,o.y);           //y
 glVertex3f(290,697+100,o.y);
 glVertex3f(290,697,o.y-100);           //z
 glVertex3f(290,697,o.y+100);
glEnd();
glRasterPos2f(290+200,690);
draw_string_bitmap(GLUT_BITMAP_HELVETICA_12, "x");
glRasterPos3f(290,697,-100);
draw_string_bitmap(GLUT_BITMAP_HELVETICA_12, "y");
glRasterPos2f(290,697-890);
draw_string_bitmap(GLUT_BITMAP_HELVETICA_12, "z");
}
void draw()
{
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// Очистка экрана и буфера глубины
glPushMatrix();           //сохранить текущую матрицу
glScalef(w/WinWid,h/WinHei,1);
setka();
navigation(direction);
glColor3f(0.153,0.525,0.616);
glTranslatef(WinWid/2-70,WinHei/2,0);
glRotatef(angleX,0,1,0);
glRotatef(angleY,1,0,0);
glRotatef(angleZ,0,0,1);
glScalef(1, (float) bodyheight/bodywidth, (float) bodylong/bodywidth);
glutWireCube(bodywidth);
topoint(point);
coordsys(point0);
step(direction,50,point0);
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
{
  w=ax;
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
	glutMainLoop(); //завершающая функция
	return 0;
}
