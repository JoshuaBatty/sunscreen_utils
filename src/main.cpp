#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"

// Creates and returns a new `ofAppBaseWindow` for a Mirror display.
shared_ptr<ofAppBaseWindow> createMirrorWindow(shared_ptr<ofAppBaseWindow> mainWindow, ofVec2f position) {
    ofGLFWWindowSettings settings;
    settings.setGLVersion(4,1);
    settings.width = 400;
    settings.height = 680;
    settings.setPosition(position);
    settings.resizable = true;
    settings.shareContextWith = mainWindow;
    shared_ptr<ofAppBaseWindow> window = ofCreateWindow(settings);
    window->setVerticalSync(true);
    return window;
}

// Creates and returns an `ofAppBaseWindow` for the number of `Mirror` displays given.
vector<shared_ptr<ofAppBaseWindow> > createMirrorWindows(shared_ptr<ofAppBaseWindow> mainWindow, int numMirrors) {
    ofVec2f position(0, 0);
    vector<shared_ptr<ofAppBaseWindow> > windows;
    for (int i = 0; i < numMirrors; i++) {
        windows.push_back(createMirrorWindow(mainWindow, position));
        position.x += 30.0;
        position.y += 30.0;
    }
    return windows;
}

// The number of mirrors in the installation.
const int NUMBER_OF_MIRRORS = 2;



//========================================================================
int main(){
    
    //---- GUI WINDOW
    ofGLFWWindowSettings settings;
    settings.setGLVersion(4,1);
    settings.width = 920;
    settings.height = 480;
    settings.setPosition(ofVec2f(510,700));
    settings.resizable = true;
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
    mainWindow->setVerticalSync(true);
    
    //---- MAIN LED WALL WINDOW
    settings.width = 936;
    settings.height = 530;
    settings.setPosition(ofVec2f(500,0));
    settings.resizable = true;
    settings.shareContextWith = mainWindow;
    shared_ptr<ofAppBaseWindow> ledWindow = ofCreateWindow(settings);
    ledWindow->setVerticalSync(true);
    
    //---- MIRROR WINDOWS
    vector<shared_ptr<ofAppBaseWindow> > mirrorWindows = createMirrorWindows(mainWindow, NUMBER_OF_MIRRORS);
    
    // Create the main `ofApp` and set the windows.
    shared_ptr<ofApp> mainApp(new ofApp);
    mainApp->setWindows(mainWindow, ledWindow, mirrorWindows);
    
    // Set callback for drawing to the LED window via the `ofApp::drawLedWindow` method.
    ofAddListener(ledWindow->events().draw, mainApp.get(), &ofApp::drawLedWindow);

    // Set callback for drawing to each mirror via the `Mirror::draw` method.
    for (int i = 0; i < mirrorWindows.size(); i++) {
        ofAddListener(mirrorWindows[i]->events().draw, &mainApp->mirrors[i], &Mirror::draw);
    }
    
    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();
}
