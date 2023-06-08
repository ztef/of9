//
//  FeatureCollectionNode.cpp
//  vectorTileExperiment
//
//  Created by Matt Blair on 6/26/14.
//
//

#include "FeatureCollectionNode.h"

FeatureCollectionNode::FeatureCollectionNode(vector<FeatureNode*> newChildren) {
    
    //ofVec3f anchorx = getAnchorFromChildren(newChildren);

    
    //setGlobalPosition(anchorx);

   // anchor = anchorx;
    
    for (int i = 0; i < newChildren.size(); i++) {
        addChild(newChildren[i]);
    }

}





FeatureCollectionNode::FeatureCollectionNode(ofMatrix4x4 newTransform) {
    
    //setTransformMatrix(newTransform);
    
    children = vector<FeatureNode*>();
    
}

void FeatureCollectionNode::customDraw() {
    
    //ofSetColor(255);
    //material.setDiffuseColor(layerColor);
    //material.setDiffuseColor(ofFloatColor(ofRandom(255),ofRandom(100,255), ofRandom(255)));
    
    
    //material.begin();
     //shader.begin();
       // shader.setUniform1f("time", ofGetElapsedTimef());
        
       //ofDrawCircle(0,0,550);
       // shader.end();
       
     
    if(layerName == "boundaries"){
        
     
       for (int i = 0; i < children.size(); i++) {
           
           //shader.begin();
           
            
          // shader.setUniform1f("thickness", 100);

            
          // shader.setUniform3f("lightDir", sin(ofGetElapsedTimef()/10), cos(ofGetElapsedTimef()/10), 0);
            
           children[i]->draw();
          
           //shader.end();
                
          
       }
        
        
    
    } else {
       
        for (int i = 0; i < children.size(); i++) {
           // material.setDiffuseColor(layerColor);
               
               
           // material.begin();
                children[i]->draw();
           // material.end();
        }
       
    }

   
    
  // material.end();

    if(layerName == "roads"){
      
    } else {
       
    }

    
    
}

void FeatureCollectionNode::addChild(FeatureNode* newChild) {
    ofVec3f oldPos = newChild->getGlobalPosition();
    newChild->setParent(*this, false);
    newChild->setGlobalPosition(oldPos);
    children.push_back(newChild);
}

bool FeatureCollectionNode::removeChild(FeatureNode* childToRemove) {
    int childIndex = -1;
    
    for (int i = 0; i < children.size(); i++) {
        if (children[i] == childToRemove) {
            childIndex = i;
        }
    }
    
    if (childIndex == -1) {
        return false;
    } else {
        children.erase(children.begin()+childIndex);
        return true;
    }
}

ofVec3f FeatureCollectionNode::getAnchorFromChildren(vector<FeatureNode*> children) {
    
    ofVec3f anchor;
    
    for (int i = 0; i < children.size(); i++) {
      //  anchor += children[i]->getGlobalPosition() / children.size();
          anchor += children[i]->anchor / children.size();
    }
    
    return anchor;
}

void FeatureCollectionNode::printPosition(const string &prefix) {
    
    cout << "FeatureCollectionNode at: " + ofToString(getPosition()) << endl;
    
    string newPrefix = prefix + "-";
    
    for (int i = 0; i < children.size(); i++) {
        cout << newPrefix;
        children[i]->printPosition(newPrefix);
    }
}
