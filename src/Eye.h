#pragma once

#include "ofMain.h"
#include "ofxAnimatableOfPoint.h"
#include "EyeImageSet.h"
#include "Constants.h"


namespace eye {
    class Eye
    {
     
    private:
      
      ofImage surfaceImg;
      ofImage whiteImg;
      ofImage pupilImg;
      ofImage shadeImg;
      
      float eyeWidth;
      float eyeHeight;
      float eyeRadius;
      
    public:
      
      Eye(){};
      Eye(shared_ptr<ImageSet> set){
          surfaceImg.setFromPixels(set->surface.getPixels());
          whiteImg.setFromPixels(set->white.getPixels());
          pupilImg.setFromPixels(set->pupil.getPixels());
          shadeImg.setFromPixels(set->shade.getPixels());
      };
      
      void setup(const ofVec2f& pos, float w, float h);
      void update();
      void draw(const bool& debugMode);
      
      ofxAnimatableOfPoint  pupilPos;
      ofVec2f               restPos;
    };
}