#pragma once

#include "ofMain.h"
#include "ofxButtons.h"
#include "ofxGrabCam.hpp"

//#include "FeatureNode.h"
#include "FeatureLeafNode.h"
#include "FeatureCollectionNode.h"
#include "Map.h"
#include "Intersector.hpp"
#include "MVTLoader.hpp"

#include <fstream>
#include <stdexcept>
#include <iostream>



enum {
ESCENA_XYZ,
ESCENA_CIRCULO,
ESCENA_MAPA_VECTORIAL,
ESCENA_ESFERA,
};


class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ButtonManager buttons;
    
    Map mapa;
    
    float fps;
    float fScale;
    float fRotation;
    int sceneSelect;
    bool bReset;
    float zoom;
    bool once;
    
    
    float x;
    float y;
    ofVec3f pos;
    ofMesh interestPoints;
    
    FeatureLeafNode* building0;
    
    FeatureCollectionNode* pois;
    
    //FeatureCollectionNode* rootNode;
    FeatureNode* rootNode;
    FeatureNode* rootNode1;
    
    ofLight mainLight;
    ofEasyCam camera;
    ofShader shader;
    
    ofMatrix4x4 m;
    
    //ofxraycaster::Ray ray;
    //ofxraycaster::Mousepicker mousepicker;
    ofVec3f mouse_picker_pos;
    
    Intersector intersector;
    glm::vec3 intersection;
    
    
};
