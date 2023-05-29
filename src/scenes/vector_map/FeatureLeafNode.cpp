//
//  FeatureLeafNode.cpp
//  vectorTileExperiment
//
//  Created by Matt Blair on 6/26/14.
//
//

#include "FeatureLeafNode.h"

FeatureLeafNode::FeatureLeafNode(ofMesh newGeometry, string nodetype) {
    
    geometry = newGeometry;
    material = ofMaterial();
    material.setDiffuseColor(ofColor(50, 0, 0));
    material.setSpecularColor(ofColor(255, 0, 0));
    idString = "";
    type = nodetype;

     

    setPosition(geometry.getCentroid());
    
}

FeatureLeafNode::FeatureLeafNode(ofMatrix4x4 newTransform) {
    
    

    //setTransformMatrix(newTransform);
    geometry = ofMesh();
    material = ofMaterial();
    idString = "";
    
}

void FeatureLeafNode::customDraw() {
    
   // material.begin();
    if(type == "Point"){
      ofDrawBox(0.1, 0.1, 0.1);
        ofPushMatrix();
        ofColor(255,0,0);
              ofTranslate(anchor);
              ofRotateXDeg(90);
              ofDrawCylinder(10, 2000);
        ofPopMatrix();
    }
    
    if(type == "Line"){
        dolly(200);
    }
        geometry.draw();
     
  //  material.end();

}

void FeatureLeafNode::printPosition(const string &prefix) {
    cout << "FeatureLeafNode at: " + ofToString(getPosition()) << endl;
}
