//
//  FaceDetector.h
//  sunscreen
//
//  Created by Mitchell Nordine on 19/11/2016.
//
//

#ifndef FaceDetector_h
#define FaceDetector_h

#include <map>
#include "DetectedFace.h"

const unsigned int FACE_MODE_LARGE_FACES = 0;
const unsigned int FACE_MODE_SMALL_FACES = 1;

class FaceDetector {
public:
    FaceDetector();
    ~FaceDetector();
    
    // Initialise the inner objective-C objects.
    //
    // `maximumFaces` refers to the maximum number of faces detected at any point in
    // time. Affectiva uses a CPU core per face, so this should be taken into
    // consideration when calculating this value.
    //
    // `faceMode` refers to either `FACE_MODE_SMALL_FACES` or `FACE_MODE_LARGE_FACES`.
    // We'll have to experiment between both of these to discover the best suited
    // for the installation.
    void init(unsigned long maximumFaces, unsigned int faceMode);
    
    // Process the given `ofPixels` data.
    void processPixels(unsigned char* pixelData, size_t width, size_t height);
    
    // Returns a `map` of the currently detected faces.
    std::map<FaceId, DetectedFace> currentlyDetectedFaces();
    
    void startDetector();
    void stopDetector();
    void resetDetector();
    
private:
    // Pointer to the inner objective-C object.
    void * self;
};


#endif /* FaceDetector_h */
