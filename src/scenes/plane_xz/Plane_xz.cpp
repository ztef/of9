//
//  Plane_xz.cpp
//  vib
//
//  Created by Esteban on 20/03/23.
//

#include "Plane_xz.hpp"

Plane_xz::Plane_xz(){
}

void Plane_xz::setup(){
    
    o_x = 0;
    o_y = 0;
    
    dim_x = 20;
    dim_y = 20;
    
    delta = 100;
    
    color.set(ofRandom(255),ofRandom(255),ofRandom(255)); // one way of defining digital color is by addressing its 3 components individually (Red, Green, Blue) in a value from 0-255, in this example we're setting each to a random value
}

void Plane_xz::update(){
   
}

void Plane_xz::draw(){
    //ofDrawAxis(100);
    // ofDrawGrid(0.5, 10, true, false,false, true);
    ofNoFill();
    
    
    for (int i = 0; i < dim_x ; i += 1) {
      for (int j = 0; j < dim_y ; j += 1) {
        ofDrawRectangle(i*delta-((dim_x *delta/2)),j*delta-((dim_y *delta/2)),0, 100, 100);
      }
    }
}
