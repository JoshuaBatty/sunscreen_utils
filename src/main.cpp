#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"

// The number of mirrors in the installation.
const int NUMBER_OF_MIRRORS = 1;



//========================================================================
int main(){
    
    //---- GUI WINDOW
    ofGLFWWindowSettings settings;
    settings.setGLVersion(4,1);
    settings.width = 1920;
    settings.height = 1080;
    settings.setPosition(ofVec2f(0,0));
    settings.resizable = true;
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
    mainWindow->setVerticalSync(true);
    
    // Create the main `ofApp` and set the windows.
    shared_ptr<ofApp> mainApp(new ofApp);
    
    
    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();
}
