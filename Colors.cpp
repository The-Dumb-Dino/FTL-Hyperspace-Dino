#include "Colors.h"


GL_Color COLOR_BUTTON_TEXT = GL_Color(25 / 255.f, 49 / 255.f, 51 / 255.f, 1.f); // #193133
GL_Color COLOR_BUTTON_ON = GL_Color(235 / 255.f, 245 / 255.f, 229 / 255.f, 1.f); // #EBF5E5
GL_Color COLOR_BUTTON_ACTIVE = GL_Color(167 / 255.f, 203 / 255.f, 255 / 255.f, 1.f); // #A7CBFF
GL_Color COLOR_BUTTON_HOVER = GL_Color(225 / 255.f, 244 / 255.f, 255 / 255.f, 1.f); // #E1F4FF

GL_Color COLOR_TINT = GL_Color(0.5f, 0.5f, 0.5f, 1.f); // #808080
GL_Color COLOR_WHITE = GL_Color(1.f, 1.f, 1.f, 1.f); // #FFFFFF
GL_Color COLOR_YELLOW = GL_Color(1.f, 1.f, 0.196f, 1.f); // #FFFF32


void GL_Color::SaveState(int fd)
{
    FileHelper::writeFloat(fd, r);
    FileHelper::writeFloat(fd, g);
    FileHelper::writeFloat(fd, b);
    FileHelper::writeFloat(fd, a);
}

void GL_Color::LoadState(int fd)
{
    r = FileHelper::readFloat(fd);
    g = FileHelper::readFloat(fd);
    b = FileHelper::readFloat(fd);
    a = FileHelper::readFloat(fd);
}
