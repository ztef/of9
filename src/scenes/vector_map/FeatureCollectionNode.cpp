//
//  FeatureCollectionNode.cpp
//  vectorTileExperiment
//
//  Created by Matt Blair on 6/26/14.
//
//

#include "FeatureCollectionNode.h"

FeatureCollectionNode::FeatureCollectionNode(vector<FeatureNode*> newChildren) {
    
    ofVec3f anchorx = getAnchorFromChildren(newChildren);

    
    setGlobalPosition(anchorx);

    anchor = anchorx;
    
    for (int i = 0; i < newChildren.size(); i++) {
        addChild(newChildren[i]);
    }

}

FeatureCollectionNode::FeatureCollectionNode(ofMatrix4x4 newTransform) {
    
    //setTransformMatrix(newTransform);
    
    children = vector<FeatureNode*>();
    
}

void FeatureCollectionNode::customDraw() {
    
    ofSetColor(255);
    material.setDiffuseColor(layerColor); 

    material.begin();
     //shader.begin();
       // shader.setUniform1f("time", ofGetElapsedTimef());
        
       //ofDrawCircle(0,0,550);
       // shader.end();
       
     
    if(layerName == "buildingxs"){
        
     
       for (int i = 0; i < children.size(); i++) {
           
           //ofPushMatrix();
           //ofMultMatrix(children[i]->getLocalTransformMatrix());
           
           //children[i]->transformGL();
                children[i]->draw();
           //children[i]->restoreTransformGL();
           // ofSetColor(ofColor::yellow);
           // ofSetLineWidth(1);
            //ofDrawCircle(children[i]->getPosition(), 4);
           // ofDrawCylinder(children[i]->getPosition(), 0.0001,  0.01);
           //ofDrawCylinder(children[i]->getPosition(), 0.001,  0.01);
           //
           
           //ofPopMatrix();
            
           
           /*OK
           ofPushMatrix();
                
                ofMultMatrix(children[i]->getGlobalTransformMatrix());
             
                
           
                ofSetColor(ofColor::cyan);//TOP
           //ofDrawCylinder(children[i]->getY(),children[i]->getZ(),children[i]->getX(),0.0001,  0.01);
                ofDrawBox(children[i]->getX(),children[i]->getY(),children[i]->getZ(),0.0001, 0.0001, 0.01);
           ofPopMatrix();
           */
           
           
           //ofPushMatrix();
           //ofMultMatrix(children[i]->getGlobalTransformMatrix());
           
           //ofSetColor(ofColor::cyan);
           //ofRotateXDeg(90);
           //ofRotateYDeg(90);
           //ofScale(0.1);
           // ofDrawCylinder(children[i]->getY(),children[i]->getZ(),children[i]->getX(),0.0001,  0.1);
           //ofDrawCylinder(children[i]->getGlobalPosition(),0.0001,  0.1);
           
           
           //ofPopMatrix();

       }
        
        
    
    } else {
        for (int i = 0; i < children.size(); i++) {
            
            children[i]->draw();
        }
    }

   
    
   material.end();

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
