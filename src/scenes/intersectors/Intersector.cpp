//
//  Intersector.cpp
//  of9
//
//  Created by Esteban on 20/05/23.
//

#include "Intersector.hpp"

Intersector::Intersector() {
    
}

Intersector::~Intersector(){
     
}


bool Intersector::intersect(vector<FeatureNode*> * elements, unsigned int &indexIntersectedPrimitive,glm::vec3 &intersection, FeatureLeafNode* intersectedElement){
    
    distanceToClosestIntersection = numeric_limits<float>::max();
    found = false;
    
    for(int i = 0; i < elements->size(); i++) {
        FeatureLeafNode* elementx = (FeatureLeafNode*) elements->at(i);
        if(elementx->geometry.getMode() == OF_PRIMITIVE_TRIANGLES){
               
               bool intersects = mousepicker.getRay().intersectsMesh(elementx->geometry, baricentricCoordinates, distance, surfaceNormal);
               if (intersects && (distance < distanceToClosestIntersection)) {
                   found = true;
                   distanceToClosestIntersection = distance;
                   indexIntersectedPrimitive = i;
               }
        }
    }
    
    if (found) {
       
        intersection = mousepicker.getRay().getOrigin() +
                            mousepicker.getRay().getDirection() *
                            distanceToClosestIntersection;
       
        intersectedElement = (FeatureLeafNode*) elements->at(indexIntersectedPrimitive);
        
      
    }
    
    
    
    return found;
}

void Intersector::mouseMoved(ofCamera* camera, int x, int y){
    ofVec3f screenToWorld = camera->screenToWorld(ofVec3f(x,y,0.0));
         
    mousepicker.getRay().setup(camera->getPosition(),screenToWorld - camera->getPosition());
}
