//
//  Projection.cpp
//  of9
//
//  Created by Esteban on 19/05/23.
//

#include "ofMain.h"
#include "Projection.hpp"

Projection::Projection() {
    mode = PROJ_MERCATOR;
};

void Projection::setMode(projection_mode _mode) {
    mode = _mode;
};

ofPoint Projection::getProjection (Coordinate _coordinate) {
  ofPoint position;
  switch (mode) {
    case PROJ_SCREEN:
          position = this->screen(_coordinate);
          break;
    case PROJ_EQUIRECTANGULAR:
          position = this->equirectangular(_coordinate);
          break;
    case PROJ_MERCATOR:
          position = this->mercator(_coordinate);
          break;
    case PROJ_STEREOGRAPHIC:
    case PROJ_AZIMUTHAL_EQUALAREA:
          position = this->azimuthal(_coordinate);
          break;
    case PROJ_SPHERICAL:
          position = this->spherical(_coordinate);
          break;
    default:
      break;
  }
  return position;
}

Coordinate Projection::getCoordinate(ofPoint projected_point){
    Coordinate coordinate;
    switch (mode) {
      case PROJ_EQUIRECTANGULAR:
             
      case PROJ_MERCATOR:
            coordinate = this->mercator_inverse(projected_point);
            break;
      case PROJ_STEREOGRAPHIC:
      case PROJ_AZIMUTHAL_EQUALAREA:
            
      case PROJ_SPHERICAL:
             
      default:
        break;
    }
    return coordinate;
}



ofPoint Projection::mercator(Coordinate _coordinate) {
    ofPoint position;
    position.x = (_coordinate.longitude / 180.0) * scale + translateX;
    position.y = /*_coordinate.latitude > 85 ? 1 : _coordinate.latitude < -85 ? -1 //<- we should consider about polar regions converting..
    : */ ( log(tan(PI / 4.0 + this->pvRadians(_coordinate.latitude) / 2.0)) / PI ) * scale - translateY;
    return position;
};


ofPoint Projection::screen(Coordinate _coordinate) {
    ofPoint position;
    position.x = _coordinate.longitude * scale + translateX;
    position.y = _coordinate.latitude * scale - translateY;
    position.z = 0;
    return position;
};

Coordinate Projection::mercator_inverse(ofPoint position) {
    Coordinate coordinate;
     
    coordinate.longitude = ((position.x - translateX ) / scale ) * 180.0;
    coordinate.latitude = pvDegrees((atan(exp(PI * (position.y + translateY ) / scale)) - PI/4) * 2);
    
    
    return coordinate;
};



ofPoint Projection::equirectangular(Coordinate _coordinate) {
    ofPoint position;
    position.x = _coordinate.longitude / 360 * scale + translateX;
    position.y = _coordinate.latitude / 360  * scale - translateY;
    return position;
};

ofPoint Projection::azimuthal(Coordinate _coordinate) {
    ofPoint position;
    float cy0 = cos(this->pvRadians(0));
    float sy0 = sin(this->pvRadians(0));
    float radian = PI / 180.0;
    float x1 = _coordinate.longitude * radian - this->pvRadians(0);
    float y1 = _coordinate.latitude * radian;
    float cx1 = cos(x1);
    float sx1 = sin(x1);
    float cy1 = cos(y1);
    float sy1 = sin(y1);
    float cc = sy0 * sy1 + cy0 * cy1 * cx1;
    float k;
    switch(mode) {
        case PROJ_STEREOGRAPHIC:
            k = 1 / (1 + cc);
            break;
        case PROJ_AZIMUTHAL_EQUALAREA:
            k = sqrt(2 / (1 + cc));
            break;
        default:
            break;
    }
    position.x = k * cy1 * sx1 * scale + translateX;
    position.y = -1 * k * (sy0 * cy1 * cx1 - cy0 * sy1) * scale + translateY;
    return position;
}

ofPoint Projection::spherical(Coordinate _coordinate) {
    ofPoint position;
    float lat = this->pvRadians(_coordinate.latitude);
    float lon = this->pvRadians(_coordinate.longitude);
    position.x = E_R * cosf(lat) * cosf(lon) * scale;
    position.y = E_R * cosf(lat) * sinf(lon) * scale;
    position.z = E_R * sinf(lat) * scale;
    return position;
}

float Projection::pvRadians(float _degrees) {
    float radians = PI / 180.0;
    return radians * _degrees;
};

float Projection::pvDegrees(float _rad){
    float degrees = 180.0 / PI;
    return _rad * degrees;
}

void Projection::setScale(float _scale) {
    scale = _scale;
};

void Projection::setTranslate(float _transelateX, float _transelateY) {
    translateX = _transelateX;
    translateY = _transelateY;
};

