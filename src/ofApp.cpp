#include "ofApp.h"

//--------------------------------------------------------------
// Grab shared pointes from main.cpp about our windows so we can query them.
//
// Creates a new empty `Mirror` for each of the given `mirrorWindow`s.
//
// Gets called just before `ofApp::setup`.
void ofApp::setWindows(shared_ptr<ofAppBaseWindow> _mainWindow,
                       shared_ptr<ofAppBaseWindow> _ledWindow,
                       vector<shared_ptr<ofAppBaseWindow> > mirrorWindows)
{
    // The GUI window, used for testing.
    mainWindow = _mainWindow;
    mainWindow->setWindowTitle("Test GUI Window");
    
    // LED window - displays the aggregate visualisation.
    ledWindow = _ledWindow;
    ledWindow->setWindowTitle("LED Window");
    
    // A window for each mirror in the installation.
    for (int i = 0; i < mirrorWindows.size(); i++) {
        Mirror mirror(mirrorWindows[i]);
        mirror.window->setWindowTitle("Mirror " + ofToString(i));
        mirrors.push_back(mirror);
    }
}

//--------------------------------------------------------------
void ofApp::setup(){
    
    // Setup the face detector and video grabbers for each `Mirror`.
    //
    // TODO: This should be updated once we discover the actual
    // device names for the cameras on each mirror.
    for (int i=0; i < mirrors.size(); i++) {
        
        // Detector params.
        const unsigned long MAX_FACES = 2;
        const unsigned int FACE_MODE = FACE_MODE_LARGE_FACES;
        
        // Video grabber params.
        int rgbDeviceID = 0;
        int uvDeviceID = 0;
        
        mirrors[i].setupVideoGrabbers(rgbDeviceID, uvDeviceID, CAM_WIDTH, CAM_HEIGHT);
        mirrors[i].setupDetector(MAX_FACES, FACE_MODE);
    }

    // Now that the grabbers and detectors are initialised,
    // we're ready to start detection.
    for (int i=0; i < mirrors.size(); i++) {
        mirrors[i].startFaceDetector();
    }
    
    // Resetting Arb Tex Back as it was disabled for the mirrors.
    ofEnableArbTex();
    
}

//--------------------------------------------------------------
void ofApp::update(){

    // Update the state of each mirror.
    for (int i=0; i < mirrors.size(); i++) {
        mirrors[i].update();
    }
    
}

//--------------------------------------------------------------
// Helper function for drawing an emotion value.
void drawEmotion(string emotion, float value, ofVec2f emotionPos, float dataPadding, int num) {
    const float MAX_WIDTH = 100.0;
    const float RECT_HEIGHT = 10.0;
    ofVec2f rectOffset = ofVec2f(0.0, -7.0);
    ofVec2f pos = emotionPos + ofVec2f(0.0, dataPadding * (float)num);
    ofSetColor(128);
    ofNoFill();
    ofDrawRectangle(pos + rectOffset, MAX_WIDTH, RECT_HEIGHT);
    ofSetColor(200, 100, 100);
    ofFill();
    ofDrawRectangle(pos + rectOffset, value * MAX_WIDTH / 100.0, RECT_HEIGHT);
    ofSetColor(255);
    ofDrawBitmapString(emotion, pos);
}

//--------------------------------------------------------------
// Only draw test GUI stuff in here.
//
// To draw to LED display, use `ofApp::drawLedWindow` method.
// To draw to Mirror displays, use `Mirror::draw` method.
void ofApp::draw(){

    ofBackground(0, 0, 0);
    ofSetColor(255);

    // Display the input from the RGB and UV video grabbers
    // owned by each `Mirror` (just for testing and debugging).
    ofVec2f pos(20, 20);
    for (int i=0; i < mirrors.size(); i++) {
        Mirror* mirror = &mirrors[i];
        ofDrawBitmapString("Mirror " + ofToString(i), pos.x, pos.y);
        
        // Draw the video grabbers.
        const float grabberHeight = 180;
        float ratio = grabberHeight / mirrors[i].rgbGrabber.getHeight();
        float grabberWidth = mirrors[i].rgbGrabber.getWidth() * ratio;
        ofVec2f rgbPos = pos + ofVec2f(0.0, 20.0);
        ofVec2f uvPos = rgbPos + ofVec2f(grabberWidth, 0.0);
        ofDrawBitmapString("RGB", rgbPos);
        mirror->rgbGrabber.draw(rgbPos, grabberWidth, grabberHeight);
        ofDrawBitmapString("UV", uvPos);
        mirror->uvGrabber.draw(uvPos, grabberWidth, grabberHeight);
        
        // Draw the face data.
        const float faceDataWidth = grabberWidth;
        const float dataPadding = 12.0;
        ofVec2f facePos = uvPos + ofVec2f(grabberWidth + 10.0, 0.0);
        typedef std::map<FaceId, DetectedFace>::iterator it_type;
        it_type begin = mirror->currentlyDetectedFaces.begin();
        it_type end = mirror->currentlyDetectedFaces.end();
        for (it_type it = begin; it != end; it++) { 
            DetectedFace* face = &it->second;
            
            // Draw the Face ID.
            ofDrawBitmapString("FACE " + ofToString(face->faceId), facePos);
            
            // Draw the face's bounding rectangle.
            FaceBounds bounds = face->faceBounds;
            ofNoFill();
            ofRectangle faceRect(rgbPos.x + bounds.x * ratio,
                                 rgbPos.y + bounds.y * ratio,
                                 bounds.width * ratio,
                                 bounds.height * ratio);
            ofDrawRectangle(faceRect);
            ofDrawBitmapString(ofToString(face->faceId), faceRect.position);
            
            // Draw the detected Age.
            string ageRange;
            switch (face->appearance.age) {
                case AGE_UNDER_18: ageRange = "Under 18"; break;
                case AGE_18_24: ageRange = "18 to 24"; break;
                case AGE_25_34: ageRange = "25 to 34"; break;
                // Start taking decaded off. 
                case AGE_35_44: ageRange = "25 to 34"; break;
                case AGE_45_54: ageRange = "35 to 44"; break;
                case AGE_55_64: ageRange = "45 to 54"; break;
                case AGE_65_PLUS: ageRange = "55 to 64"; break;
                    
//                case AGE_35_44: ageRange = "35 to 44"; break;
//                case AGE_45_54: ageRange = "45 to 54"; break;
//                case AGE_55_64: ageRange = "55 to 64"; break;
//                case AGE_65_PLUS: ageRange = "65 or over"; break;
                    
                default: ageRange = ""; break;
            }
            ofVec2f agePos = facePos + ofVec2f(0.0, dataPadding);
            ofDrawBitmapString("Age = " + ageRange, agePos);
            
            // Draw the detected Ethnicity.
            string ethnicity;
            switch (face->appearance.ethnicity) {
                case ETHNICITY_UNKNOWN: ethnicity = "Unknown"; break;
                case ETHNICITY_CAUCASIAN: ethnicity = "Caucasian"; break;
                case ETHNICITY_BLACK_AFRICAN: ethnicity = "Black African"; break;
                case ETHNICITY_EAST_ASIAN: ethnicity = "East Asian"; break;
                case ETHNICITY_HISPANIC: ethnicity = "Hispanic"; break;
                case ETHNICITY_SOUTH_ASIAN: ethnicity = "Souht Asian"; break;
                default: ethnicity = ""; break;
            }
            ofVec2f ethnicityPos = agePos + ofVec2f(0.0, dataPadding);
            ofDrawBitmapString("Ethnicity = " + ethnicity, ethnicityPos);
            
            // Draw the detected Gender.
            string gender;
            switch (face->appearance.gender) {
                case GENDER_UNKNOWN: gender = "Unknown"; break;
                case GENDER_FEMALE: gender = "Female"; break;
                case GENDER_MALE: gender = "Male"; break;
                default: gender = ""; break;
            }
            ofVec2f genderPos = ethnicityPos + ofVec2f(0.0, dataPadding);
            ofDrawBitmapString("Gender = " + gender, genderPos);
            
            // Draw the detected Emotions (except for valence as it is -100 to 100).
            ofVec2f emotionPos = genderPos + ofVec2f(0.0, dataPadding + 2.0);
            drawEmotion("anger", face->emotions.anger, emotionPos, dataPadding, 1);
            drawEmotion("contempt", face->emotions.contempt, emotionPos, dataPadding, 2);
            drawEmotion("disgust", face->emotions.disgust, emotionPos, dataPadding, 3);
            drawEmotion("engagement", face->emotions.engagement, emotionPos, dataPadding, 4);
            drawEmotion("fear", face->emotions.fear, emotionPos, dataPadding, 5);
            drawEmotion("joy", face->emotions.joy, emotionPos, dataPadding, 6);
            drawEmotion("sadness", face->emotions.sadness, emotionPos, dataPadding, 7);
            drawEmotion("surprise", face->emotions.surprise, emotionPos, dataPadding, 8);
            
            // Bump the x pos across for the next face.
            facePos.x += faceDataWidth;
            
        }
        
        // Bump the y pos down for the next mirror.
        pos.y += grabberHeight + 50;
    }
    
}

//--------------------------------------------------------------
void ofApp::drawLedWindow(ofEventArgs &args){
    ofBackground(0, 0, 0);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
