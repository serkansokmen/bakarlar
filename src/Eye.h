#pragma once

#include "ofMain.h"
#include "ofxAnimatableOfPoint.h"


namespace eye {
    class Eye
    {
      
      float eyeWidth;
      float eyeHeight;
      float eyeRadius;
      
      ofPath  debugCircle, debugPlus;
      ofxAnimatableOfPoint pupilPos;
      
    public:
      
      void setup(const ofVec2f& pos, float w, float h);
      void update();
      void draw(const bool& debugMode);
      
      void lookAt(const ofVec2f& pos);
      void rest();
      
      ofVec2f centerPos;
      
      ofImage surfaceImg;
      ofImage whiteImg;
      ofImage pupilImg;
      ofImage shadeImg;
    };
}