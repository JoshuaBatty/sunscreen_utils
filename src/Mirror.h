//
//  Mirror.hpp
//  sunscreen
//
//  Created by Mitchell Nordine on 14/11/2016.
//
//

#ifndef Mirror_hpp
#define Mirror_hpp

#include <stdio.h>
#include "ofMain.h"
#include "FaceDetector.h"
#include "Person.h"
#include "PreDetectionProcessor.h"


// Contains all data and methods related to a single Mirror.
//
// The `ofApp` contains a `vector<Mirror>`, i.e. one per mirror in the installation.
//
// The `Mirror` includes:
//     - Two video grabbers: one for RGB and one for UV (cameras mounted on top of the mirror).
//     - A list of the people currently visible within the window at any point in time.
//     - A handle to the window upon which the visualisation will be displayed.
class Mirror {

public:

    // Construct the `Mirror` with the window to which its visualisations will be drawn.
    Mirror(shared_ptr<ofAppBaseWindow> _window);
    ~Mirror();
    
    // Setup the video grabbers to use the devices specified via the given DeviceIDs.
    void setupVideoGrabbers(int rgbDeviceID, int uvDeviceID, int w, int h);
    
    // Setup the Affectiva Detector.
    void setupDetector(unsigned long maximumFaces, unsigned int faceMode);
    
    // Start the face detection.
    void startFaceDetector();
    
    // Stop the face detection.
    void stopFaceDetector();
    
    // Update the state of the `Mirror`.
    void update();

    // Draw the current state of the `Mirror`.
    void draw(ofEventArgs & args);

    // The window used for drawing to the mirror's display.
    shared_ptr<ofAppBaseWindow> window;

    // The list of people who are currently visible within this mirror.
    vector<Person> currentlyVisiblePeople;
    
    // All faces that are currently visible to the detector.
    map<FaceId, DetectedFace> currentlyDetectedFaces;
    
    // Shader for adjusting the brightness and contrast of the incoming frame before
    // It is passed onto afectiva for analysis.
    PreDetectionProcessor preProcesser;
    void testShaderDraw();

    // Each `Mirror` installation will have two cameras mounted on top of it: one for
    // capturing RGB, the other for capturing UV. Below are the video grabbers we use
    // to retrieve these frames.
    ofVideoGrabber rgbGrabber;
    ofVideoGrabber uvGrabber;

private:
    FaceDetector * faceDetector;

};


#endif /* Mirror_hpp */