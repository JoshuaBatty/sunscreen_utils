//
//  Person.hpp
//  sunscreen
//
//  Created by Mitchell Nordine on 14/11/2016.
//
//

#ifndef Person_hpp
#define Person_hpp

#include <stdio.h>
#include "ofMain.h"
#include "DetectedFace.h"


// A snapshot of both the RGB and UV input at a single moment in time.
//
// This is used for visualising the person's face morphing with the aggregate face.
class Snapshot {
public:
    ofTexture rgb;
    ofTexture uv;
};


// Collection of all data relevant for a single person.
class Person {
public:
    
    // The snapshot that has had the most confident mean crowdsight metric.
    //
    // We have to work out how to select an optimal snapshot over the duration
    // that someone stands in front of the mirror. Perhaps if crowdsight's API produces
    // a confidence level for each feature, we can test the confidence of every frame
    // and use the snapshot that has the highest confidence and most suitable feature
    // positions.
    Snapshot mostConfidentSnapshot;
    // Ranging from 0.0 (least confident) to 1.0 (most confident).
    float meanConfidenceForSnapshot;

    // Facial features produced by a mirror's face detector.
    DetectedFace face;
};


#endif /* Person_hpp */
