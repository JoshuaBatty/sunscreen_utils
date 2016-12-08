//
//  AffdexWrapperObjc.h
//  sunscreen
//
//  Created by Mitchell Nordine on 20/11/2016.
//
//

#ifndef AffdexWrapperObjc_h
#define AffdexWrapperObjc_h

#import <Foundation/Foundation.h>
#import <Affdex/Affdex.h>
#import "FaceDetector.h"


@interface AffdexWrapperObjc: NSObject <AFDXDetectorDelegate>
{
    // Stores all faces that are currently being detected. The key is the `faceId`.
    std::map<FaceId, DetectedFace> detectedFaces;
}

@property (strong) AFDXDetector *detector;
@property (assign) NSTimeInterval startTime;

- (void)setupFrameStreamDetector:(AFDXDetector*)detector;
- (void)processImage:(NSImage*)facePicture;
- (NSError*)startDetector;
- (NSError*)stopDetector;
- (void)resetDetector;
- (DetectedFace)convertToDetectedFace:(AFDXFace*)face;
- (std::map<FaceId, DetectedFace>)currentlyDetectedFaces;

@end

#endif /* AffdexWrapperObjc_h */
