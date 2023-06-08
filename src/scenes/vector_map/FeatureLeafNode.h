//
//  FeatureLeafNode.h
//  vectorTileExperiment
//
//  Created by Matt Blair on 6/26/14.
//
//

#pragma once

#ifndef __vectorTileExperiment__FeatureLeafNode__
#define __vectorTileExperiment__FeatureLeafNode__

#include <iostream>
#include <string>
#include "ofMain.h"
#include "FeatureNode.h"
#include "VShaders.hpp"
 

#endif /* defined(__vectorTileExperiment__FeatureLeafNode__) */

class FeatureLeafNode : public FeatureNode {
    
public:
    ofMesh geometry;
    vector<ofPolyline> verts;
    string idString;
    string type;
    int level;
    int zoom;
    string name;
     
   
    
    FeatureLeafNode(vector<ofPolyline> verts, string nodetype);
    FeatureLeafNode(ofMatrix4x4 newTransform);
    
    virtual void customDraw();
    virtual void printPosition(const string &prefix);
    
private:
    ofMaterial material;
    
};
