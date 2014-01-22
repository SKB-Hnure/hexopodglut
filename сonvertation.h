#ifndef CONVERTATION_H_INCLUDED
#define CONVERTATION_H_INCLUDED
#include <string.h>
using namespace std;
int grad_to_microsec(int angle)
{
return 1856*angle/180;
}
double rad_to_grad(double angle)
{
int P=3.1415;
return (double)180*angle/P;
}
double grad_to_rad(double angle)
{
int P=3.1415;
return (double)P*angle/180;
}
char* to_char(int val) //возвращает число, преобразованное в строку
{
 char *buff=new char[5];
 sprintf(buff, "%d", val);
 return buff;
}
string to_string(int val)
{
 char buff[32];
 sprintf(buff,"%d",val);
 return string(buff);
}

double abs(double a)
{
    if (a>=0) return a;
    else return (-a);
}
#endif // СONVERTATION_H_INCLUDED
