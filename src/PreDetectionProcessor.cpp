//
//  PreDetectionProcessor.cpp
//  sunscreen
//
//  Created by Joshua Batty on 3/12/2016.
//
//

#include "PreDetectionProcessor.h"


//--------------------------------------------------------------
PreDetectionProcessor::PreDetectionProcessor(){

    brightness = 0.15;
    contrast = 1.2;
    saturation = 1.5;

    /// Load the brightness contrast and saturation shader
    if(brcosaShader.load("shaders/brcosa")){
        cout << "Shader succesfully loaded!" << endl;
    }
    
    shaderFbo.allocate(CAM_WIDTH,CAM_HEIGHT,GL_RGBA);
    pixelBufferBack.allocate(CAM_WIDTH*CAM_HEIGHT*3,GL_DYNAMIC_READ);
    pixelBufferFront.allocate(CAM_WIDTH*CAM_HEIGHT*3,GL_DYNAMIC_READ);
}

//--------------------------------------------------------------
ofPixels& PreDetectionProcessor::process(ofTexture &videoTexture){
    brcosaShader.load("shaders/brcosa");
    
    shaderFbo.begin();
    ofClear(0,0,0,0);
    brcosaShader.begin();
    
    brcosaShader.setUniform3f("iResolution", videoTexture.getWidth(), videoTexture.getHeight(), 0);
    brcosaShader.setUniformTexture("iChannel0", videoTexture, 0);
    brcosaShader.setUniform1f( "brightness", brightness );
    brcosaShader.setUniform1f( "contrast", contrast );
    brcosaShader.setUniform1f( "saturation", saturation );

    ofDrawRectangle(0, 0, videoTexture.getWidth(), videoTexture.getHeight());
    
    brcosaShader.end();
    shaderFbo.end();
    
    // copy the fbo texture to a buffer
    shaderFbo.getTexture().copyTo(pixelBufferBack);
    
    // map the buffer so we can access it from the cpu
    // and wrap the memory in an ofPixels to save it
    // easily. Finally unmap it.
    //pixelBufferFront.bind(GL_PIXEL_UNPACK_BUFFER);
    unsigned char * p = pixelBufferFront.map<unsigned char>(GL_READ_ONLY);
    pixels.setFromExternalPixels(p,shaderFbo.getWidth(),shaderFbo.getHeight(),OF_PIXELS_RGB);
    pixelBufferFront.unmap();
    
    // swap the front and back buffer so we are always
    // copying the texture to one buffer and reading
    // back from another to avoid stalls
    swap(pixelBufferBack,pixelBufferFront);
    
    return pixels;
}

//--------------------------------------------------------------
void PreDetectionProcessor::draw(int x, int y, int width, int height){
    shaderFbo.draw(x,y,width,height);
}

//--------------------------------------------------------------
void PreDetectionProcessor::setBrightness(float brightness){
    this->brightness = brightness;
}
void PreDetectionProcessor::setContrast(float contrast){
    this->contrast = contrast;
}
void PreDetectionProcessor::setSaturation(float saturation){
    this->saturation = saturation;
}
