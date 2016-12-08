//
//  Mirror.cpp
//  sunscreen
//
//  Created by Mitchell Nordine on 14/11/2016.
//
//

#include "Mirror.h"


//--------------------------------------------------------------
Mirror::Mirror(): faceDetector(NULL) {

}

//--------------------------------------------------------------
Mirror::~Mirror() {
    if (faceDetector) {
        delete faceDetector;
        faceDetector = NULL;
    }
}

//--------------------------------------------------------------
void Mirror::setupVideoGrabbers(int rgbDeviceID, int uvDeviceID, int w, int h) {

    rgbGrabber.setDeviceID(rgbDeviceID);
    uvGrabber.setDeviceID(uvDeviceID);
    
    rgbGrabber.setDesiredFrameRate(60);
    uvGrabber.setDesiredFrameRate(60);
    
    rgbGrabber.initGrabber(w, h);
    uvGrabber.initGrabber(w, h);
}

//--------------------------------------------------------------
void Mirror::setupDetector(unsigned long maximumFaces, unsigned int faceMode) {
    faceDetector = new FaceDetector();
    faceDetector->init(maximumFaces, faceMode);
}

//--------------------------------------------------------------
void Mirror::startFaceDetector() {
    faceDetector->startDetector();
}

//--------------------------------------------------------------
void Mirror::stopFaceDetector() {
    faceDetector->stopDetector();
}

//--------------------------------------------------------------
void Mirror::update() {
    
    // Update the video grabbers.
    rgbGrabber.update();
    uvGrabber.update();
    
    // If we have a new frame, pass the pixels to the Detector for processing.
    if (rgbGrabber.isFrameNew()) {
        ofPixels pixels = rgbGrabber.getPixels();
//        ofPixels pixels = preProcesser.process(rgbGrabber.getTexture());

        unsigned char* pixelData = pixels.getData();
        if (pixelData != NULL) {
            size_t width = pixels.getWidth();
            size_t height = pixels.getHeight();
            faceDetector->processPixels(pixelData, width, height);
        }
    }
    
    // Update our map of currently visible faces.
    currentlyDetectedFaces = faceDetector->currentlyDetectedFaces();

}

//--------------------------------------------------------------
void Mirror::testShaderDraw(){
    ofSetColor(255,255,255);
    preProcesser.process(rgbGrabber.getTexture());
    preProcesser.draw(0, 0, ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void Mirror::draw(ofEventArgs & args) {
    ofBackground(0, 0, 0);
    
}