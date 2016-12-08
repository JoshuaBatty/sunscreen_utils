//
//  PreDetectionProcessor.h
//  sunscreen
//
//  Created by Joshua Batty on 3/12/2016.
//
//

#pragma once 

#include "ofMain.h"
#include "Constants.h"

class PreDetectionProcessor{
public:
    
    PreDetectionProcessor();
    
    ofPixels& process(ofTexture &videoTexture);
    void draw(int x, int y, int width, int height);

    void setBrightness(float brightness);
    void setContrast(float contrast);
    void setSaturation(float saturation);
    
private:
    ofShader brcosaShader;
    ofVboMesh fsQuadVbo;
    ofFbo shaderFbo;
    ofBufferObject pixelBufferBack, pixelBufferFront;
    ofPixels pixels;

    float brightness;
    float contrast;
    float saturation;
};
