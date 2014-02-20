#ifndef SEND_DATA_H_INCLUDED
#define SEND_DATA_H_INCLUDED
#include <iostream>
#include <unistd.h>  /* Объявления стандартных функций UNIX */
#include <fcntl.h>   /* Объявления управления файлами */
#include <errno.h>   /* Объявления кодов ошибок */
#include <termios.h> /* Объявления управления POSIX-терминалом */
#include <string.h>
using namespace std;

int sends(char port[], char st[], int length) //отправляет через последовательный порт "/dev/ttyACM0"
{
  int fd=0; /* Файловый дескрип
  тор для порта */
  fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == 0)
  {
       perror("open_port: Unable to open ");
  }
  else
      fcntl(fd, F_SETFL, 0);   // Установить значение флагов описателя, заданное в arg
/* *** Configure Port *** */
struct termios tty;
memset (&tty, 0, sizeof tty);

/* Error Handling */
if ( tcgetattr ( fd, &tty ) != 0 )
{
cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << endl;
}
cfsetospeed (&tty, (speed_t)B9600);
cfsetispeed (&tty, (speed_t)B9600);
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
Q[0]=Q1; Q[1]=Q2; Q[2]=Q3;
S[0]=S1; S[1]=S2; S[2]=S3;
char *f=new char[50];
*f=0;
for (int i=0;i<3;i++)
{
strcat(f,"#");
strcat(f,to_char(S[i]));
strcat(f,"P");
strcat(f,to_char(Q[i]));
}
strcat(f,"T");
strcat(f,to_char(T));
strcat(f,"\n\r");
return f;
}

void clean(char path[64])                                                         //очистка файла
{
ofstream f(path, ios_base::trunc);
f.close();
}
void append(int Q1, int Q2, int Q3, int pin[3],int delay, char path[64])           //добавление строки в конец файла
{
ofstream f(path, ios_base::app);
char *st=new char[50];
st=create_string(Q1,Q2,Q3,pin[0],pin[1],pin[2],delay);
char port[64];
memset(port,'\0',64);
//strcpy(port,"/dev/serial/by-id/usb-FTDI_FT232R_USB_UART_AE014ME2-if00-port0");
strcpy(port,"/dev/rfcomm0");
sends(port,st,strlen(st));
f<<st<<"\n";
st=NULL;
delete st;
f.close();
}

#endif // SEND_DATA_H_INCLUDED
