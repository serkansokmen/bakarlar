#pragma once

#include "ofMain.h"


namespace eyegrid {
    struct EyeImageSet {
        ofImage surface;
        ofImage white;
        ofImage pupil;
        ofImage shade;
    };
}