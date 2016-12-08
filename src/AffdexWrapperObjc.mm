//
//  AffdexWrapperObjc.m
//  sunscreen
//
//  Created by Mitchell Nordine on 20/11/2016.
//
//

#import <Foundation/Foundation.h>
#import "AffdexWrapperObjc.h"

@implementation AffdexWrapperObjc

///////////////////////////////
///// C++ Wrapper Methods /////
///////////////////////////////

//--------------------------------------------------------------
FaceDetector::FaceDetector(): self(NULL) {
}

//--------------------------------------------------------------
FaceDetector::~FaceDetector() {
    [(id)self dealloc];
}

//--------------------------------------------------------------
// Initialise the detector for handling the given `maximumFaces` and `faceMode`.
void FaceDetector::init(unsigned long maximumFaces, unsigned int faceMode) {
    
    // Create the Objective-C wrapper.
    self = [[AffdexWrapperObjc alloc] init];
    
    // Create our detector configured for processing individual consecutive frames.
    AFDXDetector *detector = [[AFDXDetector alloc] initWithDelegate:(id)self
                                                     discreteImages:NO
                                                       maximumFaces:maximumFaces
                                                           faceMode:(FaceDetectorMode)faceMode];
    
    // Set the detector within our Objective-C wrapper.
    [(id)self setupFrameStreamDetector:detector];
}

//--------------------------------------------------------------
void FaceDetector::processPixels(unsigned char* pixelData, size_t width, size_t height) {
    
    // Create an `NSBitmapImageRep` from our `ofPixels` data.
    // We'll need this to create our `NSImage*`.
    NSBitmapImageRep* imageRep =
        [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:&pixelData
                                                pixelsWide:width
                                                pixelsHigh:height
                                             bitsPerSample:8
                                           samplesPerPixel:3
                                                  hasAlpha:NO
                                                  isPlanar:NO
                                            colorSpaceName:NSDeviceRGBColorSpace
                                               bytesPerRow:0
                                              bitsPerPixel:0];
    
    // Create the `NSImage` from our bitmap representation.
    NSImage* image = [[NSImage alloc] initWithSize:NSMakeSize(width, height)];
    [image addRepresentation:imageRep];

    [(id)self processImage:image];
}

//--------------------------------------------------------------
std::map<FaceId, DetectedFace> FaceDetector::currentlyDetectedFaces() {
    return [(id)self currentlyDetectedFaces];
}

//--------------------------------------------------------------
void FaceDetector::startDetector() {
    // If starting the Detector would cause an Error, crash for now.
    NSError* error = [(id)self startDetector];
    if (error != nil) {
        NSLog(@"%@", [error localizedDescription]);
        assert(NO);
    }
}

//--------------------------------------------------------------
void FaceDetector::stopDetector() {
    // If stopping the Detector would cause an Error, crash for now.
    NSError* error = [(id)self stopDetector];
    if (error != nil) {
        NSLog(@"%@", [error localizedDescription]);
        assert(NO);
    }
}

//--------------------------------------------------------------
void FaceDetector::resetDetector() {
    [(id)self resetDetector];
}


///////////////////////////////////////
///// Objective-C Wrapper Methods /////
///////////////////////////////////////

//--------------------------------------------------------------
// Initialise our objective C wrapper with an `AFDXDetector`.
- (void)setupFrameStreamDetector:(AFDXDetector *)detector
{
    [detector setDetectAllAppearances:YES];
    [detector setDetectAllExpressions:YES];
    [detector setDetectAllEmotions:YES];
    [detector setDetectEmojis:NO];
    detector.gender = TRUE;
    detector.glasses = TRUE;
    detector.ethnicity = TRUE;
    detector.age = TRUE;
    
    self.detector = detector;
}

//--------------------------------------------------------------
// Process the video frame given in the form of an NSImage*.
//
// This is called from `AffdexWrapper::processPixels`.
- (void)processImage:(NSImage *)facePicture
{
    // TODO: There may be a more efficient method of determining the time interval,
    // but this seems to work fine for now.
    NSTimeInterval currentTime = [[NSDate date] timeIntervalSince1970];
    NSTimeInterval interval = currentTime - self.startTime;
    
    [self.detector processImage:facePicture atTime:interval];
}

//--------------------------------------------------------------
// Start the `AFDXDetector` and initialise the `startTime` for determining the interval.
- (NSError*)startDetector
{
    assert(self.detector);
    
    // Set the current time for determining the interval when processing images.
    self.startTime = [[NSDate date] timeIntervalSince1970];
    
    return [self.detector start];
}

//--------------------------------------------------------------
// Stop the `AFDXDetector`.
- (NSError*)stopDetector
{
    assert(self.detector);
    return [self.detector stop];
}

//--------------------------------------------------------------
// Reset the `AFDXDetector`.
- (void)resetDetector
{
    assert(self.detector);
    [self.detector reset];
}

//--------------------------------------------------------------
// This method is called in your code and signals when the detector detects a
// new face that has come into view. It is often used in conjunction with
// `detector:didStopDetectingFace`. The implementation of this delegate method
// is optional.
- (void)         detector:(AFDXDetector *)detector
    didStartDetectingFace:(AFDXFace *)face
{
    // Now that we've started detecting this face, add it to our map.
    DetectedFace detectedFace = [self convertToDetectedFace:face];
    self->detectedFaces[detectedFace.faceId] = detectedFace;
}

//--------------------------------------------------------------
// This method is called in your code and signals when the detector no longer
// detects a particular face. This is the converse of
// `detector:didStartDetectingFace`. Together, the two methods provide signals
// of when a face comes into or goes out of view. The implementation of this
// delegate method is also optional.
- (void)        detector:(AFDXDetector *)detector
    didStopDetectingFace:(AFDXFace *)face
{
    // Now that we've stopped detecting this face, remove it from our map.
    self->detectedFaces.erase(face.faceId);
}

//--------------------------------------------------------------
// This method is called in your code when the detector has processed a video
// frame from the camera, from a video file, or via a static image. There are
// four parameters sent to this method:
//
//  1. A reference to the detector
//  2. A dictionary of `AFDXFace` objects corresponding to the faces in the
//     image. They key for each object is the face identifier. If `nil` is
//     passed, then this is an unprocessed frame.
//  3. A reference to the image.
//  4. A timestamp (relative to 0) representing the point in time that the
//     image was processed.
//
// For camera and video cases, the number of frames that are processed to the
// detector is usually a subset of the available frames.
- (void)detector:(AFDXDetector*)detector
      hasResults:(NSMutableDictionary *)faces
        forImage:(NSImage *)image
          atTime:(NSTimeInterval)time
{
    // If we have no face dictionary there's nothing to do.
    if (faces == nil) {
        return;
    }
    
    // For each `ADFXFace` detected, update the `DetectedFace`.
    for (AFDXFace *face in [faces allValues]) {
        DetectedFace detectedFace = [self convertToDetectedFace:face];
        self->detectedFaces[detectedFace.faceId] = detectedFace;
    }
    
}

//--------------------------------------------------------------
// A method that the wrapper may call for retrieving a `map` of the currently
// detected faces.
- (std::map<FaceId, DetectedFace>)currentlyDetectedFaces
{
    return self->detectedFaces;
}

//--------------------------------------------------------------
// A custom method used to convert a `AFDXFace*` to our `DetectedFace` type.
- (DetectedFace)convertToDetectedFace:(AFDXFace *)face
{
    Appearance appearance = {};
    appearance.age       = (DetectedAge)face.appearance.age;
    appearance.ethnicity = (DetectedEthnicity)face.appearance.ethnicity;
    appearance.gender    = (DetectedGender)face.appearance.gender;
    appearance.glasses   = (DetectedGlasses)face.appearance.glasses;
    
    Emotions emotions = {};
    emotions.anger      = (float)face.emotions.anger;
    emotions.contempt   = (float)face.emotions.contempt;
    emotions.disgust    = (float)face.emotions.disgust;
    emotions.engagement = (float)face.emotions.engagement;
    emotions.fear       = (float)face.emotions.fear;
    emotions.joy        = (float)face.emotions.joy;
    emotions.sadness    = (float)face.emotions.sadness;
    emotions.surprise   = (float)face.emotions.surprise;
    emotions.valence    = (float)face.emotions.valence;

    Expressions expressions = {};
    expressions.attention          = (float)face.expressions.attention;
    expressions.browFurrow         = (float)face.expressions.browFurrow;
    expressions.browRaise          = (float)face.expressions.browRaise;
    expressions.cheekRaise         = (float)face.expressions.cheekRaise;
    expressions.chinRaise          = (float)face.expressions.chinRaise;
    expressions.dimpler            = (float)face.expressions.dimpler;
    expressions.eyeClosure         = (float)face.expressions.eyeClosure;
    expressions.eyeWiden           = (float)face.expressions.eyeWiden;
    expressions.innerBrowRaise     = (float)face.expressions.innerBrowRaise;
    expressions.jawDrop            = (float)face.expressions.jawDrop;
    expressions.lipTighten         = (float)face.expressions.lidTighten;
    expressions.lipCornerDepressor = (float)face.expressions.lipCornerDepressor;
    expressions.lipPress           = (float)face.expressions.lipPress;
    expressions.lipPucker          = (float)face.expressions.lipPucker;
    expressions.lipStretch         = (float)face.expressions.lipStretch;
    expressions.lipSuck            = (float)face.expressions.lipSuck;
    expressions.mouthOpen          = (float)face.expressions.mouthOpen;
    expressions.noseWrinkle        = (float)face.expressions.noseWrinkle;
    expressions.smile              = (float)face.expressions.smile;
    expressions.smirk              = (float)face.expressions.smirk;
    expressions.upperLipRaise      = (float)face.expressions.upperLipRaise;
    
    FaceBounds faceBounds = {};
    faceBounds.x      = (float)face.faceBounds.origin.x;
    faceBounds.y      = (float)face.faceBounds.origin.y;
    faceBounds.width  = (float)face.faceBounds.size.width;
    faceBounds.height = (float)face.faceBounds.size.height;
    
    Orientation orientation = {};
    orientation.yaw                 = (float)face.orientation.yaw;
    orientation.pitch               = (float)face.orientation.pitch;
    orientation.roll                = (float)face.orientation.roll;
    orientation.interocularDistance = (float)face.orientation.interocularDistance;
    
    std::vector<FacePoint> facePoints;
    
    DetectedFace detectedFace = {};
    detectedFace.faceId = (unsigned int)face.faceId;
    detectedFace.appearance = appearance;
    detectedFace.emotions = emotions;
    detectedFace.expressions = expressions;
    detectedFace.faceBounds = faceBounds;
    detectedFace.orientation = orientation;
    detectedFace.facePoints = facePoints;
    
    return detectedFace;
}

@end
