#ifndef HEXOPOD_H_INCLUDED
#define HEXOPOD_H_INCLUDED
#include <string.h>
#include "three_angles.h"
#include "send_data.h"
#endif // HEXOPOD_H_INCLUDED
using namespace std;
struct leg_angles {
int Q1;
int Q2;
int Q3;
};
struct axval {
float x;
float y;
float z;
};
struct extreme_values
{
int min;
int max;
};

 class hexopod
{
      public:
      axval bodysize; //габариты корпуса, x-width, y-height, z-long
      axval L[3]; //габариты звеньев ноги
      axval ang0[3][6]; //углы поворота для нулевых точек
      axval ang[3][6]; //текущие углы наклона звеньев
      axval point[6]; //текущие точки конца ноги
      axval p0[6]; //начальные центры концов ног
      leg_angles step_angles[500];  //[i] grad;
      extreme_values servo_border[6][3];  //крайние значения серводвигателей
      axval p1[6];
      int pin[6][3];                      //номера пинов
      int I0[6][3];                       //нулевые положения серводвигателей (к чему прибавлять угол)
      float delay;
      int dir[6][3];
      int points_num;
      char path[64];
      bool firstcall;
      int dcenleg;
      int previos_angle;
axval find_point0(int n)
{
axval point0, legbeg;
switch (n)
{
case 0:
legbeg.x=bodysize.x/2-L[2].y;
legbeg.y=0;
legbeg.z=L[0].y;
point0.x=legbeg.x+cos(grad_to_rad(ang0[0][0].x))*(L[0].x+L[1].x*cos(grad_to_rad(ang0[1][0].z))+L[2].x*cos(grad_to_rad(ang0[2][0].z-ang0[1][0].z)));
point0.y=legbeg.y+sin(grad_to_rad(ang0[0][0].x))*(L[0].x+L[1].x*cos(grad_to_rad(ang0[1][0].z))+L[2].x*cos(grad_to_rad(ang0[2][0].z-ang0[1][0].z)));
point0.z=legbeg.z-L[1].x*sin(grad_to_rad(ang0[1][0].z))+L[2].x*sin(grad_to_rad(ang0[2][0].z-ang0[1][0].z));
break;
case 1:
legbeg.x=bodysize.x/2;
legbeg.y=0;
legbeg.z=L[0].y;
point0.x=legbeg.x-cos(grad_to_rad(ang0[0][0].x))*(L[0].x+L[1].x*cos(grad_to_rad(ang0[1][0].z))+L[2].x*cos(grad_to_rad(ang0[2][0].z-ang0[1][0].z)));
point0.y=legbeg.y+sin(grad_to_rad(ang0[0][0].x))*(L[0].x+L[1].x*cos(grad_to_rad(ang0[1][0].z))+L[2].x*cos(grad_to_rad(ang0[2][0].z-ang0[1][0].z)));
point0.z=legbeg.z-L[1].x*sin(grad_to_rad(ang0[1][0].z))+L[2].x*sin(grad_to_rad(ang0[2][0].z-ang0[1][0].z));
break;
case 2:
legbeg.x=bodysize.x/2-L[2].y;
legbeg.y=bodysize.z/2;
legbeg.z=L[0].y;
point0.x=legbeg.x+cos(grad_to_rad(ang0[0][0].x))*(L[0].x+L[1].x*cos(grad_to_rad(ang0[1][0].z))+L[2].x*cos(grad_to_rad(ang0[2][0].z-ang0[1][0].z)));
point0.y=legbeg.y-sin(grad_to_rad(ang0[0][0].x))*(L[0].x+L[1].x*cos(grad_to_rad(ang0[1][0].z))+L[2].x*cos(grad_to_rad(ang0[2][0].z-ang0[1][0].z)));
point0.z=legbeg.z-L[1].x*sin(grad_to_rad(ang0[1][0].z))+L[2].x*sin(grad_to_rad(ang0[2][0].z-ang0[1][0].z));
break;
case 3:
legbeg.x=bodysize.x/2-L[2].y;
legbeg.y=bodysize.z/2;
legbeg.z=L[0].y;
point0.x=legbeg.x+cos(grad_to_rad(ang0[0][0].x))*(L[0].x+L[1].x*cos(grad_to_rad(ang0[1][0].z))+L[2].x*cos(grad_to_rad(ang0[2][0].z-ang0[1][0].z)));
point0.y=legbeg.y+sin(grad_to_rad(ang0[0][0].x))*(L[0].x+L[1].x*cos(grad_to_rad(ang0[1][0].z))+L[2].x*cos(grad_to_rad(ang0[2][0].z-ang0[1][0].z)));
point0.z=legbeg.z-L[1].x*sin(grad_to_rad(ang0[1][0].z))+L[2].x*sin(grad_to_rad(ang0[2][0].z-ang0[1][0].z));
break;
case 4:
legbeg.x=bodysize.x/2;
legbeg.y=bodysize.z/2;
legbeg.z=L[0].y;
point0.x=legbeg.x-cos(grad_to_rad(ang0[0][0].x))*(L[0].x+L[1].x*cos(grad_to_rad(ang0[1][0].z))+L[2].x*cos(grad_to_rad(ang0[2][0].z-ang0[1][0].z)));
point0.y=legbeg.y-sin(grad_to_rad(ang0[0][0].x))*(L[0].x+L[1].x*cos(grad_to_rad(ang0[1][0].z))+L[2].x*cos(grad_to_rad(ang0[2][0].z-ang0[1][0].z)));
point0.z=legbeg.z-L[1].x*sin(grad_to_rad(ang0[1][0].z))+L[2].x*sin(grad_to_rad(ang0[2][0].z-ang0[1][0].z));
break;
case 5:
legbeg.x=bodysize.x/2;
legbeg.y=bodysize.z/2;
legbeg.z=L[0].y;
point0.x=legbeg.x-cos(grad_to_rad(ang0[0][0].x))*(L[0].x+L[1].x*cos(grad_to_rad(ang0[1][0].z))+L[2].x*cos(grad_to_rad(ang0[2][0].z-ang0[1][0].z)));
point0.y=legbeg.y+sin(grad_to_rad(ang0[0][0].x))*(L[0].x+L[1].x*cos(grad_to_rad(ang0[1][0].z))+L[2].x*cos(grad_to_rad(ang0[2][0].z-ang0[1][0].z)));
point0.z=legbeg.z-L[1].x*sin(grad_to_rad(ang0[1][0].z))+L[2].x*sin(grad_to_rad(ang0[2][0].z-ang0[1][0].z));
break;
}
return point0;
}
void endstep(float angle, float steplong, float anr)
{
for (int n=0;n<6;n++)
{
if ((angle>=0)&&(angle<=90))                                            // I
      {
      p1[n].x=steplong*sin(anr)+p0[n].x;
      p1[n].y=-steplong*cos(anr)+p0[n].y;
         }
  if ((angle>90)&&(angle<=180))                                            // II
      {
      p1[n].x=steplong*sin(Pi-anr)+p0[n].x;
      p1[n].y=steplong*cos(Pi-anr)+p0[n].y;
      }
if ((angle>180)&&(angle<=270))                                            // III
      {
      p1[n].x=p0[n].x-steplong*sin(anr-Pi);
      p1[n].y=p0[n].y+steplong*cos(anr-Pi);
      }
 if ((angle>270)&&(angle<360))                                                 //IV
      {
      p1[n].x=p0[n].x-steplong*sin(2*Pi-anr);
      p1[n].y=p0[n].y-steplong*cos(2*Pi-anr);
      }
}
}
      hexopod(axval body_size, int shcenleg, axval leg_element_size[3],axval angles[3][6], extreme_values b[6][3],int p[6][3], int o[6][3],int dir_servo[6][3])
              {
                  memset(path,'\0',64);
                  strcpy(path,"/home/diana/file.txt");
                  points_num=0;
              bodysize=body_size;
              dcenleg=shcenleg;
              for (int i=0;i<6;i++)
               for(int j=0;j<3;j++)
               {
                 ang[j][i]=angles[j][i];
                 ang0[j][i]=angles[j][i];
                  pin[i][j]=p[i][j];
                  I0[i][j]=o[i][j];
                  dir[i][j]=dir_servo[i][j];
                  servo_border[i][j]=b[i][j];
                  previos_angle=361;
              }
              for(int i=0;i<3;i++)
              L[i]=leg_element_size[i];
              for(int i=0;i<6;i++)
              {
                p0[i]=find_point0(i);
                point[i]=p0[i];
               }
               firstcall=true;
              }
void parallelepiped(axval psize, axval angle, axval center)
{
glColor3f(0.153,0.525,0.616);
glPushMatrix();
glTranslatef(center.x, center.y, center.z);
glRotatef(angle.x,0,1,0);
glRotatef(angle.y,1,0,0);
glRotatef(angle.z,0,0,1);
glScalef(1, (float) psize.y/psize.x, (float) psize.z/psize.x);
glutWireCube(psize.x);
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
int allow_val(int q, extreme_values a)        //находится ли угол q в диапазоне a
{
if ((q>=a.min)&&(q<=a.max))
     return q;
 else
     {
     if (q<a.min) return a.min;
     if (q>a.max) return a.max;
     }
}
void legs(axval bodysize)
{
axval dir[6]= {
{1,1,-1},{-1,-1,1},{1,1,-1},{1,1,-1},{-1,-1,1},{-1,-1,1},                            //вращательные звенья
};
axval o[3][6];
axval opoint[6];
for (int i=0;i<6;i++)
opoint[i].y=(float)-bodysize.y/2+L[0].y/2;
opoint[0].x=(float) bodysize.x/2+dcenleg;
opoint[0].z=0;
opoint[1].x=-(float) bodysize.x/2-dcenleg;
opoint[1].z=0;
opoint[2].x=(float) bodysize.x/2;
opoint[2].z=(float) bodysize.z/2;
opoint[3].x=(float) bodysize.x/2;
opoint[3].z=-(float) bodysize.z/2;;
opoint[4].x=-(float) bodysize.x/2;
opoint[4].z=(float) bodysize.z/2;
opoint[5].x=-(float) bodysize.x/2;
opoint[5].z=-(float) bodysize.z/2;
 for (int i=0;i<6;i++)                 //поворот по оси Х для поступательных звеньев равен повороту вращательного звена
 {
 ang[1][i].x=ang[0][i].x;
 ang[2][i].x=ang[0][i].x;
}
//расчет центра вращательного звена
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
glScalef(1, (float) bodysize.x/bodysize.y, (float) bodysize.x/bodysize.z);
for (int j=0;j<3;j++)
for (int i=0;i<6;i++)
parallelepiped(L[j],ang[j][i],o[j][i]);
glPopMatrix();
}
void topoint(axval point,int n)
{
float a[3];
if ((n==0)||(n==2)||(n==3))
for (int i=0;i<3;i++)
a[i]=angles(L[0].x,L[1].x,L[2].x,point.x,point.y,point.z)[i];
if ((n==5)||(n==4)||(n==1))
for (int i=0;i<3;i++)
a[i]=angles(L[0].x,L[1].x,L[2].x,-point.x,point.y,point.z)[i];
ang[0][n].x=a[0];
ang[1][n].z=-a[1];
ang[2][n].z=a[2]-a[1];
if ((n==5)||(n==4)||(n==1))
 {
ang[0][n].x=-ang[0][n].x;
ang[1][n].z=-ang[1][n].z;
ang[2][n].z=-ang[2][n].z;
}
legs(bodysize);
}
void append(int Q1, int Q2, int Q3, int pin[3])           //добавление строки в конец файла
{
ofstream f(path, ios_base::app);
char *st=new char[50];
st=create_string(Q1,Q2,Q3,pin[0],pin[1],pin[2],delay);
char port[32];
memset(port,'\0',32);
strcpy(port,"/dev/ttyACM0");
sends(port,st,strlen(st));
f<<st<<"\n";
st=NULL;
delete st;
f.close();
}
void clean()
{
ofstream f(path, ios_base::trunc);
f.close();
}
void servo_angles(float q1, float q2, float q3, int n)
{
int Q[3];
if ((n==0)||(n==2)||(n==3))
{
 Q[0]=I0[n][0]+dir[n][0]*grad_to_microsec(q1);
 Q[1]=I0[n][1]+dir[n][1]*grad_to_microsec(-q2);
 Q[2]=I0[n][2]+dir[n][2]*grad_to_microsec(q2+q3-90);
 }
if ((n==1)||(n==5)||(n==4))
{
 Q[0]=I0[n][0]+dir[n][0]*grad_to_microsec(-q1);
 Q[1]=I0[n][1]+dir[n][1]*grad_to_microsec(+q2);
 Q[2]=I0[n][2]+dir[n][2]*grad_to_microsec(-q2-q3-90);
 }
 for (int i=0;i<3;i++)
 {
 Q[i]=allow_val(Q[i],servo_border[n][i]);
 }
 append(Q[0], Q[1], Q[2], pin[n]);
}
axval transpoint(axval point, axval shift)
{
axval res;
res.x=point.x-shift.x;
res.y=point.y-shift.y;
res.z=point.z-shift.z;
return res;
}
void step(float angle, float steplong, float time, int n, bool stop)        //от primary direction
{
float anr=grad_to_rad(angle);
int rz=p0[n].z-40;
static bool dir[6]={true, true,true,true,true,true};
axval shift;
static int it[6];
struct state
 {
  bool up,down,stable;
 };
static state z[6]={{false, false, true},
                    {false, false, true},
                    {false, false, true},
                    {false, false, true},
                    {false, false, true},
                    {false, false, true}};
glColor3f(0.153,0.525,0.616);
if (n<2)
{
shift.x=bodysize.x/2-L[2].y/2;
shift.y=0;
shift.z=L[0].y;
}
if (n>1)
{
shift.x=bodysize.x/2-L[2].y/2;
shift.y=bodysize.z/2;
shift.z=L[0].y;
}
if ((int)angle!=(int)previos_angle)
endstep(angle,steplong,anr);
      //нашли точки концов шага.
if ((n>0)&&(n<4)&&(firstcall==true))
{
point[n].x=p1[n].x;
point[n].y=p1[n].y;
dir[n]=false;
z[n].up=true;
z[n].stable=false;
if (n==3) firstcall=false;
}
float dstep;
//cout<<"n="<<n<<";  ("<<p0[n].x<<";"<<p0[n].y<<";"<<p0[n].z<<")\n";
if (abs(p1[n].x-p0[n].x)>=abs(p1[n].y-p0[n].y))
{
delay=(float) 2*time/(abs(p1[n].x-p0[n].x));
dstep=(float)time/(2*(abs(p1[n].x-p0[n].x)+2*abs(p0[n].z-rz)));
        if ((angle>180)&&(angle<360))
        {
           if ((point[n].x>=p1[n].x)&&(dir[n]==true)&&(z[n].stable))
         {
          if (!stop)
          point[n].x--;
          point[n].y=(p1[n].y-p0[n].y)*(float)(point[n].x-p0[n].x)/(p1[n].x-p0[n].x)+p0[n].y;
          point[n].z=p0[n].z;
          topoint(transpoint(point[n],shift),n);
          if ((int)point[n].x%3==0)
          servo_angles(ang[0][n].x, ang[1][n].z,ang[2][n].z, n);
          if (point[n].x<=p1[n].x)
           {
            dir[n]=false;
            z[n].up=true;
            z[n].stable=false;
            if ((n==0)||(n==4)||(n==5))
             {
             for (int i=1;i<=3;i++)
             {
                z[i].up=false;
                z[i].down=false;
                z[i].stable=true;
                dir[i]=true;
                point[i].x=p0[i].x;
                point[i].y=p0[i].y;
             }
             }
           if ((n==1)||(n==2)||(n==3))
             {
             for (int i=0;i<6;i++)
             {
              if ((i==0)||(i==4)||(i==5))
              {
                z[i].up=true;
                z[i].down=false;
                z[i].stable=false;
                dir[i]=true;
                point[i].x=p1[i].x;
                point[i].y=p1[i].y;
               }
             }
             }
            }
             }
        if ((point[n].x<=p0[n].x)&&(dir[n]==false)&&(z[n].stable))
         {
         if (!stop)
         point[n].x+=dstep;
         point[n].y=(p1[n].y-p0[n].y)* (float) (point[n].x-p0[n].x)/(p1[n].x-p0[n].x)+p0[n].y;
         point[n].z=rz;
          topoint(transpoint(point[n],shift),n);
         if ((int)point[n].x%3==0)
          servo_angles(ang[0][n].x, ang[1][n].z,ang[2][n].z, n);
         if (point[n].x>=p0[n].x)
          {
            dir[n]=true;
            z[n].down=true;
            z[n].stable=false;

           }
           }
        }
if ((angle>0)&&(angle<=180))
        {
           if ((point[n].x<p1[n].x)&&(dir[n]==true)&&(z[n].stable))
         {
         if (!stop)
         point[n].x++;
         point[n].y=(p1[n].y-p0[n].y)*(float)(point[n].x-p0[n].x)/(p1[n].x-p0[n].x)+p0[n].y;
         point[n].z=p0[n].z;
          topoint(transpoint(point[n],shift),n);
           if ((int)point[n].x%3==0)
          servo_angles(ang[0][n].x, ang[1][n].z,ang[2][n].z, n);
         if (point[n].x>=p1[n].x)
          {
            dir[n]=false;
            z[n].up=true;
            z[n].stable=false;
             if ((n==0)||(n==4)||(n==5))
             {
             for (int i=1;i<=3;i++)
             {
                z[i].up=false;
                z[i].down=false;
                z[i].stable=true;
                dir[i]=true;
                point[i].x=p0[i].x;
                point[i].y=p0[i].y;
             }
             }
           if ((n==1)||(n==2)||(n==3))
             {
             for (int i=0;i<6;i++)
             {
              if ((i==0)||(i==4)||(i==5))
              {
                z[i].up=true;
                z[i].down=false;
                z[i].stable=false;
                dir[i]=true;
                point[i].x=p1[i].x;
                point[i].y=p1[i].y;
               }
             }
             }
           }
           }
           if ((point[n].x>p0[n].x)&&(dir[n]==false)&&(z[n].stable))
         {
         if (!stop)
          point[n].x-=dstep;
          point[n].y=(p1[n].y-p0[n].y)*(float)(point[n].x-p0[n].x)/(p1[n].x-p0[n].x)+p0[n].y;
          point[n].z=rz;
          topoint(transpoint(point[n],shift),n);
           if ((int)point[n].x%3==0)
          servo_angles(ang[0][n].x, ang[1][n].z,ang[2][n].z, n);
           if (point[n].x<=p0[n].x)
            {
             dir[n]=true;
             z[n].down=true;
             z[n].stable=false;
            }
          }
        }
}
else
if (abs(p1[n].x-p0[n].x)<abs(p1[n].y-p0[n].y))
{
delay=(float)2*time/(abs(p1[n].y-p0[n].y));
dstep=(float)time/(2*(abs(p1[n].y-p0[n].y)+2*abs(p0[n].z-rz)));
        if ((angle>=90)&&(angle<270))
    {
         if ((point[n].y<p1[n].y)&&(dir[n]==true)&&(z[n].stable))
        {
        if (!stop)
          point[n].y++;
          point[n].x=(p1[n].x-p0[n].x)*(float)(point[n].y-p0[n].y)/(p1[n].y-p0[n].y)+p0[n].x;
          point[n].z=p0[n].z;
          topoint(transpoint(point[n],shift),n);
           if ((int)point[n].y%3==0)
          servo_angles(ang[0][n].x, ang[1][n].z,ang[2][n].z, n);
         if (point[n].y>=p1[n].y)
          {
            dir[n]=false;
            z[n].up=true;
            z[n].stable=false;
             if ((n==0)||(n==4)||(n==5))
             {
             for (int i=1;i<=3;i++)
             {
                z[i].up=false;
                z[i].down=false;
                z[i].stable=true;
                dir[i]=true;
                point[i].x=p0[i].x;
                point[i].y=p0[i].y;
             }
             }
           if ((n==1)||(n==2)||(n==3))
             {
             for (int i=0;i<6;i++)
             {
              if ((i==0)||(i==4)||(i==5))
              {
                z[i].up=true;
                z[i].down=false;
                z[i].stable=false;
                dir[i]=true;
                point[i].x=p1[i].x;
                point[i].y=p1[i].y;
               }
             }
             }
           }
        }
         if ((point[n].y>p0[n].y)&&(dir[n]==false)&&(z[n].stable))
        {
              point[n].z=rz;
              if (!stop)
              point[n].y-=dstep;
              point[n].x=(p1[n].x-p0[n].x)*(float)(point[n].y-p0[n].y)/(p1[n].y-p0[n].y)+p0[n].x;
          topoint(transpoint(point[n],shift),n);
           if ((int)point[n].y%3==0)
              servo_angles(ang[0][n].x, ang[1][n].z,ang[2][n].z, n);
              if (point[n].y<=p0[n].y)
              {
               dir[n]=true;
               z[n].stable=false;
               z[n].down=true;
              }
        }
    }
    if (((angle>=0)&&(angle<90))||((angle>=270)&&(angle<360)))
        {
         if ((point[n].y>p1[n].y)&&(dir[n]==true)&&(z[n].stable))
         {
         if (!stop)
          point[n].y--;
          point[n].x=(p1[n].x-p0[n].x)*(float)(point[n].y-p0[n].y)/(p1[n].y-p0[n].y)+p0[n].x;
          point[n].z=p0[n].z;
          topoint(transpoint(point[n],shift),n);
           if ((int)point[n].y%3==0)
         servo_angles(ang[0][n].x, ang[1][n].z,ang[2][n].z, n);
         if (point[n].y<=p1[n].y)
          {
            dir[n]=false;
            z[n].up=true;
            z[n].stable=false;
             if ((n==0)||(n==4)||(n==5))
             {
             for (int i=1;i<=3;i++)
             {
                z[i].up=false;
                z[i].down=false;
                z[i].stable=true;
                dir[i]=true;
                point[i].x=p0[i].x;
                point[i].y=p0[i].y;
             }
             }
           if ((n==1)||(n==2)||(n==3))
             {
             for (int i=0;i<6;i++)
             {
              if ((i==0)||(i==4)||(i==5))
              {
                z[i].up=true;
                z[i].down=false;
                z[i].stable=false;
                dir[i]=true;
                point[i].x=p1[i].x;
                point[i].y=p1[i].y;
               }
             }
             }
           }
           }
         if ((point[n].y<p0[n].y)&&(dir[n]==false)&&(z[n].stable))
        {
              point[n].z=rz;
              if (!stop)
              point[n].y+=dstep;
              point[n].x=(p1[n].x-p0[n].x)*(float)(point[n].y-p0[n].y)/(p1[n].y-p0[n].y)+p0[n].x;
          topoint(transpoint(point[n],shift),n);
           if ((int)point[n].y%3==0)
              servo_angles(ang[0][n].x, ang[1][n].z,ang[2][n].z, n);
              if (point[n].y>=p0[n].y)
              {
               dir[n]=true;
               z[n].stable=false;
               z[n].down=true;
              }
        }
    }
}
if (z[n].down)
  {
  if (!stop)
     point[n].z+=dstep;
          topoint(transpoint(point[n],shift),n);
       if ((int)point[n].z%3==0)
        servo_angles(ang[0][n].x, ang[1][n].z,ang[2][n].z, n);
       if (point[n].z>=p0[n].z)
        {
        z[n].down=false;
        z[n].stable=true;
<<<<<<< HEAD
           clean();
=======
         if ((int)point[n].z%3==0)
        servo_angles(ang[0][n].x, ang[1][n].z,ang[2][n].z, n);
        clean();
>>>>>>> c5e0ce7d9e212b84325c9eb115e31e701f0d4879
        it[n]=0;
         if ((n==0)||(n==4)||(n==5))
             {
             for (int i=1;i<=3;i++)
             {
                    z[i].up=true;
                    z[i].down=false;
                    z[i].stable=false;
                    dir[i]=false;
                    point[i].x=p1[i].x;
                    point[i].y=p1[i].y;
                    point[i].z=p0[i].z;
             }
             }
        if ((n==1)||(n==2)||(n==3))
             {
             for (int i=0;i<6;i++)
             {
              if ((i==0)||(i==4)||(i==5))
              {
                    z[i].up=true;
                    z[i].down=false;
                    z[i].stable=false;
                    dir[i]=false;
                    point[i].x=p1[i].x;
                    point[i].y=p1[i].y;
               }
             }
             }
        }
    }
 if (z[n].up)
{
if (!stop)
  point[n].z-=dstep;
  topoint(transpoint(point[n],shift),n);
   if ((int)point[n].z%3==0)
  servo_angles(ang[0][n].x, ang[1][n].z,ang[2][n].z, n);
    if (point[n].z<=rz)
    {
        z[n].up=false;
        z[n].stable=true;
     }
}
previos_angle=angle;
}
};



