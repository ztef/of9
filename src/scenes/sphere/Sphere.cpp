//
//  Sphere.cpp
//  of9
//
//  Created by Esteban on 21/05/23.
//

#include "Sphere.hpp"

Sphere::Sphere(){
}

void Sphere::setup(){
    
    dim = 20;

    color.set(ofRandom(255),ofRandom(255),ofRandom(255)); // one way of defining digital color is by addressing its 3 components individually (Red, Green, Blue) in a value from 0-255, in this example we're setting each to a random value
}

void Sphere::update(){
   
}

void Sphere::draw(){
    ofBackground(0);
    ofDrawSphere(0, 0, 0, 300);
}
