#ifndef SEND_DATA_H_INCLUDED
#define SEND_DATA_H_INCLUDED
#include <unistd.h>  /* Объявления стандартных функций UNIX */
#include <fcntl.h>   /* Объявления управления файлами */
#include <errno.h>   /* Объявления кодов ошибок */
#include <termios.h> /* Объявления управления POSIX-терминалом */


char* to_char(int val)
{
 char *buff=new char[5];
 sprintf(buff, "%d", val);
 return buff;
}
int sends(char st[], int length)
{
  int fd=0; /* Файловый дескриптор для порта */
  fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == 0)
  {
       perror("open_port: Unable to open /dev/ttyACM0 - ");
  }
  else
      fcntl(fd, F_SETFL, 0);   // Установить значение флагов описателя, заданное в arg
   tcflush(fd,TCIFLUSH);
   int n = write(fd, st,length);
    if (n < 0)
  fputs("write() of 4 bytes failed!\n", stderr);
  close(fd);
  return (fd);
}
char *create_string(int Q1, int Q2, int Q3, int S1, int S2, int S3, float T)
{
int Q[3];
int S[3];
Q[0]=Q1; Q[2]=Q2; Q[3]=Q3;
S[0]=S1; S[1]=S2; S[2]=S3;
char *s=new char;
for (int i=0;i<3;i++)
 {
  s=strcat(s,"#");
  s=strcat(s,to_char(S[i]));
  s=strcat(s,"P");
  s=strcat(s,to_char(Q[i]));
 }
s=strcat(s,"#");
s=strcat(s,to_char(T));
s=strcat(s,"T");
s=strcat(s,"\n\r");
return s;
}

#endif // SEND_DATA_H_INCLUDED
