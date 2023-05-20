//
//  Projection.hpp
//  of9
//
//  Created by Esteban on 19/05/23.
//

#ifndef Projection_hpp
#define Projection_hpp

#include <stdio.h>
#include <iostream>
#include "ofMain.h"

typedef struct Coordinate {
    float latitude;
    float longitude;
} COORDINATE;


enum projection_mode {
    PROJ_MERCATOR = 0,
    PROJ_EQUIRECTANGULAR,
    PROJ_AZIMUTHAL_EQUALAREA,
    PROJ_STEREOGRAPHIC,
    PROJ_SPHERICAL,
};


class Projection  {
public:
    
    Projection();
    ofPoint getProjection(Coordinate _coordinate);
    ofPoint mercator(Coordinate _coordinate);
    ofPoint equirectangular(Coordinate _coordinate);
    ofPoint azimuthal(Coordinate _coordinate);
    ofPoint spherical(Coordinate _coordinate);
    void setMode(projection_mode _mode);
    void setScale(float scale);
    void setTranslate(float _transelateX, float _transelateY);

private:
    const float E_R = 6378.137; // Earth radius
    projection_mode mode;
    float scale;
    float translateX;
    float translateY;
    float pvRadians(float degrees);

};
#endif /* Projection_hpp */
