#pragma once

#include "ofMain.h"


namespace eyegrid {
    struct EyeImageSet {
        shared_ptr<ofImage> surface;
        shared_ptr<ofImage> white;
        shared_ptr<ofImage> pupil;
        shared_ptr<ofImage> shade;
    };
}