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


bool Intersector::intersectNode(vector<FeatureNode*> * elements, unsigned int &indexIntersectedPrimitive,glm::vec3 &intersection, FeatureLeafNode* intersectedElement){
    
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

bool Intersector::intersectPlane(ofxraycaster::Plane  plane, glm::vec3 &intersection){
    
     
              
    bool intersects = mousepicker.getRay().intersectsPlane(plane, distance);
       
    
    if (intersects) {
       
        intersection = mousepicker.getRay().getOrigin() +
                            mousepicker.getRay().getDirection() *
                            distance;
        
    }
    
    
    
    return intersects;
}

void Intersector::mouseMoved(ofCamera* camera, int x, int y){
    
     glm::vec3 const camera_origin(0, 0, 7000);
     glm::vec3 const delta_camera(camera->getPosition() - camera_origin);
     glm::vec3 const camera_offset(delta_camera.x, delta_camera.y, 0.0f);
     
    float const zoom_factor_from_default = camera->getPosition().z / 7000;
    
    
    glm::vec3 cp =camera->getPosition();
     
    
    //camera->getImagePlaneDistance()
    ofVec3f mouse = ofVec3f(x,y,0);
   // mouse.x *= zoom_factor_from_default;
   // mouse.y *= zoom_factor_from_default;
    
    //mouse += camera_offset;
    ofVec3f screenToWorld = camera->screenToWorld(mouse);
    mousepicker.getRay().setup(cp, screenToWorld - cp);

    
    
    
   
}
