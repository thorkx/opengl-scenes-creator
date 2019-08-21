// Projet Session/main.cpp

#include "ofMain.h"
#include "application.h"

// point d'entrée de l'application
int main()
{
    ofGLWindowSettings windowSettings;
    windowSettings.setSize(1024, 720);
    windowSettings.setGLVersion(3, 3);
    ofCreateWindow(windowSettings);
    ofRunApp(new Application());
}
