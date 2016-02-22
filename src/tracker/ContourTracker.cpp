#include "ContourTracker.h"


using namespace cv;
using namespace ofxCv;
using namespace tracker;


//--------------------------------------------------------------
ContourTracker::ContourTracker(){
    params.setName("Contour Tracker");
    params.add(bUseGrabber.set("Use Video Grabber", false));
    params.add(bUseKinect.set("Use Kinect", false));
    params.add(minAreaRadius.set("Min Area Radius", 10, 0, 400));
    params.add(maxAreaRadius.set("Max Area Radius", 200, 0, 400));
    params.add(nearThreshold.set("Near Threshold", 230, 0, 255));
    params.add(farThreshold.set("Far Threshold", 70, 0, 255));
    params.add(threshold.set("Threshold", 50, 0, 255));
    params.add(persistence.set("Persistance", 15, 1, 50));
    params.add(maxDistance.set("Max Distance", 50, 1, 200));
    params.add(bDraw.set("Draw Contour Tracker", true));
    
    bUseGrabber.addListener(this, &ContourTracker::toggleGrabber);
    bUseKinect.addListener(this, &ContourTracker::toggleKinect);
}

//--------------------------------------------------------------
ContourTracker::~ContourTracker(){
    bUseGrabber.removeListener(this, &ContourTracker::toggleGrabber);
    bUseKinect.removeListener(this, &ContourTracker::toggleKinect);
    kinect.close();
}

#pragma mark - Setup

//--------------------------------------------------------------
void ContourTracker::setup(){
    
    kinect.init();
    kinect.setRegistration(true);
    
    grayImage.allocate(GRABBER_WIDTH, GRABBER_HEIGHT, OF_IMAGE_GRAYSCALE);
    grayThreshNear.allocate(GRABBER_WIDTH, GRABBER_HEIGHT, OF_IMAGE_GRAYSCALE);
    grayThreshFar.allocate(GRABBER_WIDTH, GRABBER_HEIGHT, OF_IMAGE_GRAYSCALE);
    grayPreprocImage.allocate(GRABBER_WIDTH, GRABBER_HEIGHT, OF_IMAGE_GRAYSCALE);
    
    contourFinder.setFindHoles(false);
    trackerFbo.allocate(GRABBER_WIDTH, GRABBER_HEIGHT);
    trackerFbo.begin();
    ofClear(0, 0, 0, 0);
    trackerFbo.end();
}

#pragma mark - Update

//--------------------------------------------------------------
void ContourTracker::update(){
    
    if (bUseGrabber) {
        updateGrabber();
    }
    if (bUseKinect) {
        updateKinect();
    }
}

//--------------------------------------------------------------
void ContourTracker::updateGrabber(){
    
    grabber.update();
    
    if (grabber.isFrameNew()) {
        
        contourFinder.setMinAreaRadius(minAreaRadius);
        contourFinder.setMaxAreaRadius(maxAreaRadius);
        contourFinder.setThreshold(threshold);
        
        tracker.setPersistence(persistence);
        tracker.setMaximumDistance(maxDistance);
        
//        imitate(grayImage, grabber, CV_8UC1);
        contourFinder.findContours(grabber);
        tracker.track(contourFinder.getBoundingRects());
    }
}

//--------------------------------------------------------------
void ContourTracker::updateKinect(){
    
    kinect.update();
    
    // there is a new frame and we are connected
    if (kinect.isFrameNew()) {
        
        convertColor(kinect, grayImage, CV_RGB2GRAY);
        imitate(grayThreshNear, grayImage);
        imitate(grayThreshFar, grayImage);
        
        //            threshold(grayImage, grayThreshNear, &nearThreshold.get());
        //            threshold(grayImage, grayThreshFar, &farThreshold.get());
        
        // Convert to CV to perform AND operation
        Mat grayThreshNearMat = toCv(grayThreshNear);
        Mat grayThreshFarMat = toCv(grayThreshFar);
        Mat grayImageMat = toCv(grayImage);
        
        // cvAnd to get the pixels which are a union of the two thresholds
        bitwise_and(grayThreshNearMat, grayThreshFarMat, grayImageMat);
        
        // Save pre-processed image for drawing it
        grayPreprocImage = grayImage;
        
        // Process image
        dilate(grayImage);
        dilate(grayImage);
        //erode(grayImage);
        
        // Mark image as changed
        grayImage.update();
        
        contourFinder.findContours(grayImage);
        tracker.track(contourFinder.getBoundingRects());
    }
}

#pragma mark - Draw

//--------------------------------------------------------------
void ContourTracker::draw(float x, float y, float scale){
    if (bDraw) {
        
        trackerFbo.begin();
        ofClear(0, 0, 0, 0);
        if (bUseGrabber || bUseKinect) {
            ofSetColor(ofColor::white);
            if (bUseGrabber) {
                grabber.draw(0, 0);
            }
            if (bUseKinect) {
                kinect.drawDepth(0, 0, GRABBER_WIDTH, GRABBER_HEIGHT);
                //    drawPointCloud();
            }
            contourFinder.draw();
            vector<Glow>& followers = tracker.getFollowers();
            for (auto & f : followers) {
                f.draw();
            }
        }
        trackerFbo.end();
        trackerFbo.draw(x, y, GRABBER_WIDTH*scale, GRABBER_HEIGHT*scale);
    }
}

//--------------------------------------------------------------
void ContourTracker::drawPointCloud(){
    int w = 640;
    int h = 480;
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_POINTS);
    int step = 2;
    for(int y = 0; y < h; y += step) {
        for(int x = 0; x < w; x += step) {
            if(kinect.getDistanceAt(x, y) > 0) {
                mesh.addColor(kinect.getColorAt(x,y));
                mesh.addVertex(kinect.getWorldCoordinateAt(x, y));
            }
        }
    }
    glPointSize(3);
    ofPushMatrix();
    // the projected points are 'upside down' and 'backwards'
    ofScale(1, -1, -1);
    ofTranslate(0, 0, -1000); // center the points a bit
    ofEnableDepthTest();
    mesh.drawVertices();
    ofDisableDepthTest();
    ofPopMatrix();
}

#pragma mark - Param Listeners

//--------------------------------------------------------------
void ContourTracker::toggleGrabber(bool& yes) {
    ofLog(OF_LOG_NOTICE, ofToString(yes));
    if (yes) {
        grabber.setup(GRABBER_WIDTH, GRABBER_HEIGHT);
    } else {
        if (grabber.isInitialized()) {
            grabber.close();
        }
    }
}

//--------------------------------------------------------------
void ContourTracker::toggleKinect(bool& yes) {
    if (yes) {
        kinect.init();
        kinect.setRegistration(true);
        kinect.open();
    } else {
        kinect.close();
    }
}
