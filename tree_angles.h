#ifndef TREE_ANGLES_H_INCLUDED
#define TREE_ANGLES_H_INCLUDED
#define Pi 3.1415
double abs(double a)
{
    if (a>=0) return a;
    else return (-a);
}
string to_string(int val)
{
 char buff[32];
 sprintf(buff,"%d",val);
 return string(buff);
}
int grad_to_microsec(int angle)
{
return 1856*angle/180;
}
double rad_to_grad(double angle)
{
int P=3.1415;
return 180*angle/P;
}
double grad_to_rad(double angle)
{
int P=3.1415;
return P*angle/180;
}
double* angles(double L1, double L2, double L3, double x, double y, double dZ)
/*
   L1, L2, L3 - длины звеньев, где L1 поворачивается в горизонтальной плоскости, L2, L3 - в вертикальной
   x,y - координаты захвата манипулятора
   dZ - высота
   возвращает массив из трех углов в градусах
   [0] - угол между осью Х и L1, c y>0
   [1] - угол, ограниченный снизу прямой, параллельной плоскости XY, а сверху звеном L2
   [2] - угол, ограниченный сверху прямой, продолжающей звено L2, а снизу звеном L3
   */
{ double B,y2;
    double *Q = new double[3];
    if ((x>0)&&(y>=0))
Q[0]=atan(y/x);
else
 {
if (x==0)
Q[0]=Pi/2;
if ((x<0)&&(y>=0))
Q[0]=Pi-atan(y/x);
if ((x<0)&&(y<=0))
Q[0]=atan(y/x)-Pi;
if ((x>0)&&(y<0))
Q[0]=atan(y/x);
}
  y2=sqrt(pow(x,2)+pow(y,2))-L1;
 B=sqrt(dZ*dZ+pow(y2,2));
 if (y2>0)
 Q[1]=Pi/2-asin((L2*L2-L3*L3+B*B)/(2*B*L2))-atan(dZ/y2);
 else  {
     if (y2<0)  Q[1]=Pi/2-asin((L2*L2-L3*L3+B*B)/(2*B*L2))-(Pi-abs(atan(dZ/y2)));
     if (y2==0) Q[1]=Pi/2-asin((L2*L2-L3*L3+B*B)/(2*B*L2))-Pi/2;
 }
 Q[2]=Pi/2+asin((L2*L2+L3*L3-B*B)/(2*L2*L3));
 for (int i=0;i<3;i++)
 {
     Q[i]=180*Q[i]/Pi;
 }
    return Q;
}


#endif // THREE_ANGLES_H_INCLUDED
