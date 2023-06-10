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
#include "TileFunctions.hpp"

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
    PROJ_SCREEN,
};


class Projection  {
public:
    
    Projection();
    ofPoint getProjection(Coordinate _coordinate, tilefunctions::Tile tile_position);
    Coordinate getCoordinate(ofPoint projected_point);
    
    ofPoint mercator(Coordinate _coordinate);
    ofPoint screen(Coordinate _coordinate);
    Coordinate mercator_inverse(ofPoint projected_point);
    Coordinate screen_inverse(ofPoint position);
    

    ofPoint equirectangular(Coordinate _coordinate);
    ofPoint azimuthal(Coordinate _coordinate);
    ofPoint spherical(Coordinate _coordinate);
    
    
    tilefunctions::Tile tile_position;
    void setMode(projection_mode _mode);
    void setScale(float scale);
    void setTranslate(float _transelateX, float _transelateY);
    void setExtent(std::uint32_t extent);
    std::uint32_t extent;

private:
    const float E_R = 6378.137; // Radio de la Tierra
    projection_mode mode;
    float scale;
    float translateX;
    float translateY;
    float pvRadians(float degrees);
    float pvDegrees(float _rad);

};
#endif /* Projection_hpp */
