#pragma once

#include "ofMain.h"
#include "Constants.h"
#include "Mirror.h"


class ofApp : public ofBaseApp {

	public:


        //////////////////////
        ///// oF Methods /////
        //////////////////////

		void setup();
		void update();
		void draw();
    
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);


        //////////////////////////
        ///// Custom Methods /////
        //////////////////////////

        // Grab shared pointes from main.cpp about our windows so we can query them.
        // 
        // Creates a new empty `Mirror` for each of the given `mirrorWindow`s.
        //
        // This method is called just before `setup()`.
        void setWindows(shared_ptr<ofAppBaseWindow> _mainWindow,
                        shared_ptr<ofAppBaseWindow> _ledWindow,
                        vector<shared_ptr<ofAppBaseWindow> > mirrorWindows);

        void drawLedWindow(ofEventArgs & args);


        //////////////////
        ///// Fields /////
        //////////////////

        // A handle to each window.
        shared_ptr<ofAppBaseWindow> mainWindow;
        shared_ptr<ofAppBaseWindow> ledWindow;

        // A `Mirror` for each mirror in the installation.
        //
        // Each mirror contains its own `ofAppBaseWindow`, `ofVideoGrabber`s
        // and list of currently visible `Person`s.
        vector<Mirror> mirrors;

};
