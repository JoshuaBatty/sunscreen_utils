#pragma once

#include "ofMain.h"
#include "Constants.h"
#include "FaceDetector.h"


class ofApp : public ofBaseApp {

	public:
    
        ~ofApp();

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


  

        //////////////////
        ///// Fields /////
        //////////////////

        // A handle to each window.
        shared_ptr<ofAppBaseWindow> mainWindow;

        // A `Mirror` for each mirror in the installation.
        //
        // Each mirror contains its own `ofAppBaseWindow`, `ofVideoGrabber`s
        // and list of currently visible `Person`s.
        //Mirror mirror;

    
        ///------ MIRROR
        ofVideoGrabber rgbGrabber;
        FaceDetector * faceDetector;

    // All faces that are currently visible to the detector.
    map<FaceId, DetectedFace> currentlyDetectedFaces;
    
    // Setup the Affectiva Detector.
    void setupDetector(unsigned long maximumFaces, unsigned int faceMode);
    
    // Start the face detection.
    void startFaceDetector();
    
    // Stop the face detection.
    void stopFaceDetector();
    
    // Update the state of the `Mirror`.
    void updateDetector();

};
