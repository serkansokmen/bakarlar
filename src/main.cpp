#include "ofMain.h"
#include "ofApp.h"


int main(){
    
    int windowWidth = 980;
    int windowHeight = 980;
    
#ifdef TARGET_OPENGLES
    ofGLESWindowSettings settings;
    settings.width = windowWidth;
    settings.height = windowHeight;
    settings.setGLESVersion(2);
    ofCreateWindow(settings);
#else
    ofGLWindowSettings settings;
    settings.width = windowWidth;
    settings.height = windowHeight;
    settings.setGLVersion(4, 1);
    settings.windowMode = OF_WINDOW;
    ofCreateWindow(settings);
#endif
    ofRunApp(new ofApp());
}
