#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include "three_angles.h"
#include "сonvertation.h"
#include "hexopod.h"
#include "send_data.h"
#include "littlehexconfig.h"
//#include "bighexconfig.h"
using namespace std;

float WinWid=1300.0;
float WinHei=800;
float WinDeep=800;
float w=WinWid;   //текущие размеры экрана. изменяются в процессе выполнения программы при растяжении или сжатии окна
float h=WinHei;
float angleX=0;
float angleY=0;
float angleZ=0;
int direction=0;
bool stop=false;
hexopod H(bodysize,dcenleg,L,ang,servo_border,pin,I0,dir);
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
draw_string_bitmap(GLUT_BITMAP_HELVETICA_12, "Direction Angle=");
draw_string_bitmap(GLUT_BITMAP_HELVETICA_12, to_char(direction));
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
 case (int)32:
 if (!stop)
  stop=true;
  else stop=false;
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


void output(char title[], float value, int x, int y)
{
glPushMatrix();
glScalef(1, (float) bodysize.x/bodysize.y, (float) bodysize.x/bodysize.z);
glRotatef(360-angleZ,0,0,1);
glRotatef(360-angleY,1,0,0);
glRotatef(360-angleX,0,1,0);
glTranslatef(-WinWid/2+70,-WinHei/2,0);
glScalef(WinWid/w,WinHei/h,1);
glColor3f(1,1,1);
glRasterPos3f(x,y,0);
draw_string_bitmap(GLUT_BITMAP_HELVETICA_10, title);
draw_string_bitmap(GLUT_BITMAP_HELVETICA_10, to_char(value));
glPopMatrix();
}


void coordsys()
{
//point0={180,0,190};   x,y,z
glColor3f(0.5,0.5,0.5);
glBegin(GL_LINES);
 glVertex3f(-400,697,0);         //x
 glVertex3f(290+200, 697, 0);
 glVertex3f(0,697-900,0);           //z
 glVertex3f(0,697+100,0);
 glVertex3f(0,697,-300);           //y
 glVertex3f(0,697,100);
glEnd();
glRasterPos2f(290+200,690);
draw_string_bitmap(GLUT_BITMAP_HELVETICA_12, "x");
glRasterPos3f(5,697,-300);
draw_string_bitmap(GLUT_BITMAP_HELVETICA_12, "y");
glRasterPos2f(5,697-890);
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
glScalef(1, (float) bodysize.y/bodysize.x, (float) bodysize.z/bodysize.x);
glutWireCube(bodysize.x);
glBegin(GL_TRIANGLES);
        glVertex3f(bodysize.x/2, -bodysize.y/2, -bodysize.z/4);
        glVertex3f(bodysize.x/2, -bodysize.y/2, bodysize.z/4);
        glVertex3f(bodysize.x/2+dcenleg, -bodysize.y/2, 0);
glEnd();
glBegin(GL_TRIANGLES);
        glVertex3f(-bodysize.x/2, -bodysize.y/2, -bodysize.z/4);
        glVertex3f(-bodysize.x/2, -bodysize.y/2, bodysize.z/4);
        glVertex3f(-bodysize.x/2-dcenleg, -bodysize.y/2, 0);
glEnd();
coordsys();
for (int i=0;i<6;i++)
{
H.step(direction,70,steptime,i,stop);
//usleep(1000);
}
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
       // glutTimerFunc(H.delay, Timer, 0);
       glutTimerFunc(0, Timer, 0);
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
{
glClearColor(0.0, 0.0, 0.0, 1.0); //установка цвета очистки
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
