#ifndef GLUTCONFIG_H_INCLUDED
#define GLUTCONFIG_H_INCLUDED

float WinWid=1300.0;
float WinHei=800;
float WinDeep=800;
void draw_string_bitmap(void *font, const char* string)
{
  while (*string)
    glutBitmapCharacter(font, *string++);
}
#endif // GLUTCONFIG_H_INCLUDED
