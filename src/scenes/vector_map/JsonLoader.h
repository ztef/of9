#pragma once
//
//  JsonLoader.h
//  vectorTileExperiment
//
//  Created by Matt Blair on 6/26/14.
//
//

#ifndef __vectorTileExperiment__JsonLoader__
#define __vectorTileExperiment__JsonLoader__
#define R_EARTH 6378137
#define PI 3.14159265359

#include <iostream>
#include "ofxJSONElement.h"
#include "FeatureCollectionNode.h"
#include "FeatureLeafNode.h"

#endif /* defined(__vectorTileExperiment__JsonLoader__) */

class JsonLoader {
    
public:
    ofxJSONElement jsonRoot;
    
    JsonLoader();
    JsonLoader(std::string fileName);
    FeatureNode* loadTile(std::string fileName);
    
    FeatureNode* loadNodeGraph();
    FeatureCollectionNode* getCollection();
    
    FeatureCollectionNode* rootNode;
    
    FeatureNode* parseNode(ofxJSONElement nodeJson);
    FeatureCollectionNode* parseFeatureCollectionNode(ofxJSONElement collectionJson);
    FeatureNode* parseFeatureNode(ofxJSONElement featureJson);
    glm::vec3 parsePointInProjectedCoords(ofxJSONElement pointJson);
    vector<ofDefaultVec3> parsePointArrayInProjectedCoords(ofxJSONElement pointArrayJson);
    
    double y2lat(double y) { return ofRadToDeg(2 * atan(exp(ofDegToRad(y))) - PI / 2); }
    double x2lon(double x) { return ofRadToDeg(x / R_EARTH); }
    double lat2y(double lat) { return R_EARTH * log(tan(PI / 4 + ofDegToRad(lat) / 2)); }
    double lon2x(double lon) { return ofDegToRad(lon) * R_EARTH; }
   
    double getXX(double lon, int width)
    {
        // width is map width
        double x = fmod((width*(180+lon)/360), (width +(width/2)));

        return x;
    }

    double getYY(double lat, int height, int width)
    {
        // height and width are map height and width
        
        double latRad = lat*PI/180;

        // get y value
        double mercN = log(tan((PI/4)+(latRad/2)));
        double y     = (height/2)-(width*mercN/(2*PI));
        y = height -y;
        return y;
    }
    
    
    
    
    
private:
    ofTessellator tessellator;
    ofColor layerColor;
    
    ofVec3f getCentroidFromPoints(vector<glm::vec3> pts);
    void parseLineGeometry(ofxJSONElement lineJson, ofxJSONElement propsJson, ofMesh* meshToFill, glm::vec3* anchor);
    void parsePolygonGeometry(ofxJSONElement polygonJson, ofxJSONElement propsJson, ofMesh* meshToFill, glm::vec3* anchor, bool move);
    void movePoints(vector<glm::vec3> *pts, glm::vec3 offset);
    
};
