//
//  Intersector.hpp
//  of9
//
//  Created by Esteban on 20/05/23.
//

#ifndef Intersector_hpp
#define Intersector_hpp

#include <stdio.h>
#include "FeatureNode.h"
#include "FeatureLeafNode.h"
#include "ofxRaycaster.h"

class Intersector {
    
    public:
    Intersector();
    ~Intersector();
    
    void setup();
    void update();
    void draw();
    bool intersect(vector<FeatureNode*> * elements, unsigned int &indexIntersectedPrimitive,glm::vec3 &intersection, FeatureLeafNode* intersectedElement);
    
    void keyPressed(int key);
    void keyReleased(int key);
    
    void mouseMoved(ofCamera* camera, int x, int y);
    
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    
           // elements with geometry
    ofxraycaster::Mousepicker mousepicker;
    float distanceToClosestIntersection;
    glm::vec2 baricentricCoordinates;
    float distance;
    bool found;
    glm::vec3 surfaceNormal;
    
    
};

#endif /* Intersector_hpp */
