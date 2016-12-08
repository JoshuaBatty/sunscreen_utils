//
//  DetectedFace.h
//  sunscreen
//
//  Created by Mitchell Nordine on 22/11/2016.
//
//

#ifndef DetectedFace_hpp
#define DetectedFace_hpp

#include <vector>

typedef unsigned long FaceId;

// ENUM DECLARATIONS
//
// Unfortunately we can't use the actual enum types as:
// 1. We're unable to import obj-c code into our c++ wrapper class and
// 2. We're unable to forward declare `enum`s in c++.
//
// Thus we declare our own enums with the same values along with the `Detected`
// prefix to avoid name conflicts.

enum DetectedAge {
    AGE_UNKNOWN = 0,
    AGE_UNDER_18 = 1,
    AGE_18_24 = 2,
    AGE_25_34 = 3,
    AGE_35_44 = 4,
    AGE_45_54 = 5,
    AGE_55_64 = 6,
    AGE_65_PLUS = 7,
};

enum DetectedEthnicity {
    ETHNICITY_UNKNOWN = 0,
    ETHNICITY_CAUCASIAN = 1,
    ETHNICITY_BLACK_AFRICAN = 2,
    ETHNICITY_SOUTH_ASIAN = 3,
    ETHNICITY_EAST_ASIAN = 4,
    ETHNICITY_HISPANIC = 5,
};

enum DetectedGender {
    GENDER_UNKNOWN = 0,
    GENDER_MALE = 1,
    GENDER_FEMALE = 2,
};

enum DetectedGlasses {
    GLASSES_NO = 0,
    GLASSES_YES = 1,
};

struct Appearance {
    DetectedAge age;
    DetectedEthnicity ethnicity;
    DetectedGender gender;
    DetectedGlasses glasses;
};

// The probability of each emotion being present in the face, 0 to 100.
struct Emotions {
    float anger;
    float contempt;
    float disgust;
    float engagement;
    float fear;
    float joy;
    float sadness;
    float surprise;
    float valence;
};

// The probability of each expression being present on the face, 0 to 100.
struct Expressions {
    float attention;
    float browFurrow;
    float browRaise;
    float cheekRaise;
    float chinRaise;
    float dimpler;
    float eyeClosure;
    float eyeWiden;
    float innerBrowRaise;
    float jawDrop;
    float lipTighten;
    float lipCornerDepressor;
    float lipPress;
    float lipPucker;
    float lipStretch;
    float lipSuck;
    float mouthOpen;
    float noseWrinkle;
    float smile;
    float smirk;
    float upperLipRaise;
};

// The face's bounding rectangle in pixels.
struct FaceBounds {
    float x;
    float y;
    float width;
    float height;
};

struct FacePoint {
    float x;
    float y;
};

// Encapsulates all orientation metrics for the face.
struct Orientation {
    // The left/right head angle for the face (-30 degrees to 30 degrees).
    float yaw;
    // The up/down head angle for the face (-30 degrees to 30 degrees).
    float pitch;
    // The roll of the head angle for the face (-60 degrees to 60 degrees).
    float roll;
    // The distance between the eyes in pixels.
    float interocularDistance;
};

struct DetectedFace {
    // Unique identifier for this face.
    FaceId faceId;
    Appearance appearance;
    Emotions emotions;
    Expressions expressions;
    FaceBounds faceBounds;
    Orientation orientation;
    std::vector<FacePoint> facePoints;
};

#endif /* DetectedFace_h */
