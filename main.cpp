#include "Viewer.h"

Viewer viewer(VideoMode(WINDOW_SIZE[0], WINDOW_SIZE[1]), "B&W Chess", Style::Titlebar | Style::Close);

int main()
{
    viewer.initialise();

    while (viewer.isOpen())
    {
        viewer.loop();
    }
}