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
      //ofDrawBox(0.0001, 0.0001, 0.01);
        ofPushMatrix();
              ofRotateXDeg(90);
              ofTranslate(glm::vec3(0,0.00005,0));
              ofDrawCylinder(0.000003, 0.0001);
        ofPopMatrix();
    }
    
    geometry.draw();
  //  material.end();

}

void FeatureLeafNode::printPosition(const string &prefix) {
    cout << "FeatureLeafNode at: " + ofToString(getPosition()) << endl;
}
