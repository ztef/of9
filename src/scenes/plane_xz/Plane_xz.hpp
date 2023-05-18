//
//  Plane_xz.hpp
//  vib
//
//  Created by Esteban on 20/03/23.
//

#ifndef Plane_xz_hpp
#define Plane_xz_hpp

#include "ofMain.h"
class Plane_xz {

    public: // place public functions or variables declarations here

    // methods, equivalent to specific functions of your class objects
    void setup();    // setup method, use this to setup your object's initial state
    void update();  // update method, used to refresh your objects properties
    void draw();    // draw method, this where you'll do the object's drawing

    // variables
    float o_x;        // posicion del origen
    float o_y;
    
    int dim_x;        // size
    int dim_y;
    int delta;

    ofColor color;  // color using ofColor type

    Plane_xz();  // constructor - used to initialize an object, if no properties are passed the program sets them to the default value
    private: // place private functions or variables declarations here
};
#endif /* Plane_xz_hpp */
