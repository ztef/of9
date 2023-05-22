//
//  Sphere.hpp
//  of9
//
//  Created by Esteban on 21/05/23.
//

#ifndef Sphere_hpp
#define Sphere_hpp

#include <stdio.h>
#include "ofMain.h"
class Sphere {

    public: // place public functions or variables declarations here

    // methods, equivalent to specific functions of your class objects
    void setup();    // setup method, use this to setup your object's initial state
    void update();  // update method, used to refresh your objects properties
    void draw();    // draw method, this where you'll do the object's drawing

    // variables
    float o_x;        // posicion del origen
    float o_y;
    
    int dim;        // size
    ofColor color;  // color using ofColor type

    Sphere();  // constructor - used to initialize an object, if no properties are passed the program sets them to the default value
    private: // place private functions or variables declarations here
};


#endif /* Sphere_hpp */
