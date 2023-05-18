//
//  Circle_xy.cpp
//  vib
//
//  Created by Esteban on 24/03/23.
//

#include "Circle_xy.hpp"
Circle_xy::Circle_xy(){
}

void Circle_xy::setup(){
    
    dim = 20;

    color.set(ofRandom(255),ofRandom(255),ofRandom(255)); // one way of defining digital color is by addressing its 3 components individually (Red, Green, Blue) in a value from 0-255, in this example we're setting each to a random value
}

void Circle_xy::update(){
   
}

void Circle_xy::draw(){
    ofNoFill();
    ofSetCircleResolution(100);
    for (int i = 0; i < 200; i += 5) {
        ofDrawCircle(0,0,10*i);
    }
}
