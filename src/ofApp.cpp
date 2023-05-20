#include "ofApp.h"

#include "JsonLoader.h"
#include "Plane_xz.hpp"
#include "Circle_xy.hpp"

Plane_xz escena_zy;
Circle_xy escena_circulo;

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetLogLevel(OF_LOG_NOTICE);

    buttons.setup(); // this sets up the events etc..
    escena_zy.setup();
    escena_circulo.setup();
    mapa.setup(1024,768);
   


   

    shader.load("myshader.vert", "myshader.frag"); 


    m.rotate(90,0,0,1);

   
    
    
    mapa.Load("https://tile.nextzen.org/tilezen/vector/v1/all/0/0/0.json?api_key=HjxoLw7IQJWSTo4lgErmIQ");
    mapa.Load("https://tile.nextzen.org/tilezen/vector/v1/all/2/0/1.json?api_key=HjxoLw7IQJWSTo4lgErmIQ");
    mapa.Load("https://tile.nextzen.org/tilezen/vector/v1/all/2/1/1.json?api_key=HjxoLw7IQJWSTo4lgErmIQ");
   
    
    
    
    // General graphics setup
   
    ofBackground(0, 0, 0);
    ofSetFrameRate(60);
    ofEnableDepthTest();
    
    
    camera.setNearClip(0.001);
    camera.setFarClip(100000);
    camera.move(0, 0, 100);
   
   // camera.setTarget(rootNode->getGlobalPosition());
    
    once = false;
    
    // Lighting setup
    mainLight = ofLight();
    mainLight.setup();
    mainLight.setPosition(-100, 200,100);
   
    mainLight.setPointLight();
    mainLight.setGlobalPosition(-100, 0, 500);
    mainLight.setDiffuseColor(ofColor(35, 35, 35));
    mainLight.setSpecularColor(ofColor(170, 170, 170));
    
    ofLog(OF_LOG_VERBOSE, "Camera at " + ofToString(camera.getX()) + ", " + ofToString(camera.getY()) + ", " + ofToString(camera.getZ()));

    ButtonPanel * panel = buttons.addButtonPanel("Settings");
    panel->addListItem("Select a color:");
    panel->addSelectionItem("Plano XYZ", sceneSelect, ESCENA_XYZ);
    panel->addSelectionItem("Circulo Angulo-Altura", sceneSelect, ESCENA_CIRCULO);
    panel->addSelectionItem("Mapa Vectorial", sceneSelect, ESCENA_MAPA_VECTORIAL);
    panel->addListItem("Dimensions");
    panel->addSliderItem("scale", 1, 5, fScale);
    panel->addSliderItem("rotation", 0, 90, fRotation);
    panel->addSliderItem("fps", 0, 300, fps);
    panel->addFlashItem("Reset", bReset);

    buttons.addSliderItem("FPS", 0, 300, fps);


}



//--------------------------------------------------------------
void ofApp::update(){

    
    zoom = camera.getZ();
    
    escena_zy.update();
    escena_circulo.update();
    mapa.update(zoom);


}

//--------------------------------------------------------------
void ofApp::draw(){

    ofSetColor(255);
    
   
    
    
    camera.begin();
    
    
    
    //mainLight.enable();
    

        
    if(ESCENA_XYZ == sceneSelect){
        escena_zy.draw();
    }
    if(ESCENA_CIRCULO == sceneSelect){
         escena_circulo.draw();
    }
    if(sceneSelect == ESCENA_MAPA_VECTORIAL){
       
        mapa.draw();
        
        // INTERSECTOR
         float distanceToClosestIntersection = numeric_limits<float>::max();
        
        
           if(mapa.tiles.size()>0){
             
             bool found=false;
             glm::vec2 baricentricCoordinates;
             float distance;
             glm::vec3 surfaceNormal;
             unsigned int indexIntersectedPrimitive = 0;
             
            
             
             FeatureCollectionNode* currentWorld = (FeatureCollectionNode*) mapa.tiles[0];
             FeatureCollectionNode* layer = (FeatureCollectionNode*) currentWorld->children.at(0);
             vector<FeatureNode *> elements = layer->children;
             //cout << layer->layerName << endl;
             for(int i = 0; i < elements.size(); i++) {
                 FeatureLeafNode* elementx = (FeatureLeafNode*) elements[i];
                 if(elementx->geometry.getMode() == OF_PRIMITIVE_TRIANGLES){
                        //currentWorld->getGlobalTransformMatrix(),
                        bool intersects = mousepicker.getRay().intersectsMesh(elementx->geometry, baricentricCoordinates, distance, surfaceNormal);
                        if (intersects && (distance < distanceToClosestIntersection)) {
                            found = true;
                            distanceToClosestIntersection = distance;
                            indexIntersectedPrimitive = i;
                        }
                 }
             }
             
             
             if (found) {
                 ofPushStyle();
                 ofSetColor(255);
                 auto intersection = mousepicker.getRay().getOrigin() +
                                     mousepicker.getRay().getDirection() *
                                     distanceToClosestIntersection;
                 ofDrawSphere(intersection, 0.01);
                 FeatureLeafNode* elementx = (FeatureLeafNode*) elements[indexIntersectedPrimitive];
                 //elementx->geometry.
                 ofPopStyle();
             }
             ofSetColor(255);
             //mousepicker.draw(5);
             mousepicker.getRay().draw();
             
         }
            
         // END INTERSECTOR
      
    }
    

    
 //   mainLight.disable();
    
   camera.end();
    
    if(sceneSelect == ESCENA_MAPA_VECTORIAL){
        
        // INTERSECTOR2
        /*
         
         float nearestDistance = numeric_limits<float>::max();
        
           if(mapa.tiles.size()>0){
             
             bool found=false;
             glm::vec2 baricentricCoordinates;
             float distance;
             glm::vec3 surfaceNormal;
             unsigned int indexIntersectedPrimitive = 0;
             
             glm::vec2 nearestVertex;
             int nearestIndex;
             
              glm::vec3 mouse(ofGetMouseX(), ofGetMouseY(),0);
               
             
             FeatureCollectionNode* currentWorld = (FeatureCollectionNode*) mapa.tiles[0];
             FeatureCollectionNode* layer = (FeatureCollectionNode*) currentWorld->children.at(0);
             vector<FeatureNode *> elements = layer->children;
             //cout << layer->layerName << endl;
             for(int i = 0; i < elements.size(); i++) {
                 FeatureLeafNode* elementx = (FeatureLeafNode*) elements[i];
                 
                     glm::vec3 cur = camera.worldToScreen(elementx->anchor);
                     //glm::vec3 cur = elementx->anchor ;
                 
                 
                     float distance = glm::distance (cur,mouse);
                     if(distance < nearestDistance) {
                         nearestDistance = distance;
                         nearestVertex = cur;
                         nearestIndex = i;
                     }
             }
             
             // Muestra linea al mas cercano
             
               ofSetColor(ofColor::gray);
               ofDrawLine(nearestVertex.x,nearestVertex.y , mouse.x, mouse.y);
                 
             
                 ofNoFill();
                 ofSetColor(ofColor::yellow);
                 ofSetLineWidth(2);
                 ofDrawCircle(nearestVertex, 4);
                 ofSetLineWidth(1);
             
                 glm::vec2 offset(0, 0);
                 ofDrawBitmapStringHighlight(ofToString(nearestIndex), mouse + offset);
             
             
         }
          
         */
         // END INTERSECTOR 2
   

     
        
    }
    
    
    
    
  // ofDisableLighting();
    
    ofSetColor(255);
    ofDrawBitmapString("ZOOM: " + ofToString(zoom), 20,80);
    ofDrawBitmapString("TILE ZOOM: " + ofToString(mapa.tile_zoom), 20,95);
    
    
    //glm::vec3 worldXYZ = camera.screenToWorld(glm::vec3(ofGetMouseX(),ofGetMouseY(),0));
    
    //ofDrawBitmapString("WORLD XYZ: " + ofToString(worldXYZ), 20,110);
    
    
    //mercatortile::LngLat rlnglat = mercatortile::lnglat(worldXYZ.x ,worldXYZ.y);
    
    ofDrawBitmapString("MOUSE X: " + ofToString(ofGetMouseX()), 20,110);
    ofDrawBitmapString("MOUSE Y: " + ofToString(ofGetMouseY()), 20,125);
    
    
    
    /*
     ofSetColor(255);
    ofSetLineWidth(2);
    glm::vec3 screenMouse (ofGetMouseX(),ofGetMouseY(),0);
    auto worldMouse = camera.screenToWorld(screenMouse);
    auto worldMouseEnd = camera.screenToWorld(glm::vec3(screenMouse.x, screenMouse.y, 1.0f));
    
    auto worldMouseTransmissionVector = worldMouseEnd - worldMouse;
    //ofDrawLine(worldMouse,worldMouseEnd);
    ofDrawLine(worldMouse,worldMouseEnd);
     */
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
   //  cout << "FeatureLeafNode at: " + ofToString(getPosition()) << endl;
  
   
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){
  // mapa.Load("https://tile.nextzen.org/tilezen/vector/v1/all/3/3/2.json?api_key=HjxoLw7IQJWSTo4lgErmIQ");
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
     if(sceneSelect == ESCENA_MAPA_VECTORIAL){
           // mousepicker.setFromCamera(glm::vec2(x,y), camera);
         
         mousepicker.getRay().setup(camera.screenToWorld(glm::vec3(x,y,0)), mapa.tiles[0]->getZAxis() * -1);
     }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
  
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

/*
#include "ofApp.h"
#include "Plane_xz.hpp"
#include "Circle_xy.hpp"
#include "JsonLoader.h"


Plane_xz escena_zy;
Circle_xy escena_circulo;
ofEasyCam cam;

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetVerticalSync(true);
    
    buttons.setup(); // this sets up the events etc..
    escena_zy.setup();
    escena_circulo.setup();


     ofSetDataPathRoot("/home/esteban/of/apps/myApps/ofVector/bin/data/");


    //JsonLoader jsonLoader = JsonLoader("https://tile.nextzen.org/tilezen/vector/v1/all/16/19293/24642.json?api_key=HjxoLw7IQJWSTo4lgErmIQ");
    JsonLoader jsonLoader = JsonLoader("vectorTile.json");
 
    rootNode = jsonLoader.loadNodeGraph();
    rootNode->setPosition(0, 0, 0);
    rootNode->printPosition("");

    ofBackground(0, 0, 0);
    ofSetFrameRate(60);
    ofEnableDepthTest();



   
   mainLight = ofLight();
    //mainLight.setup();
    //mainLight.setPosition(-100, 200,0);
   
cam.setPosition(rootNode->getGlobalPosition());
    cam.move(0, 0, 300);
    cam.setTarget(rootNode->getGlobalPosition());



    mainLight.setPointLight();
    mainLight.setGlobalPosition(-100, 0, 500);



    
    ButtonPanel * panel = buttons.addButtonPanel("Settings");
    panel->addListItem("Select a color:");
    panel->addSelectionItem("Plano XYZ", sceneSelect, ESCENA_XYZ);
    panel->addSelectionItem("Circulo Angulo-Altura", sceneSelect, ESCENA_CIRCULO);
    panel->addSelectionItem("Mapa Vectorial", sceneSelect, ESCENA_MAPA_VECTORIAL);
    panel->addListItem("Dimensions");
    panel->addSliderItem("scale", 1, 5, fScale);
    panel->addSliderItem("rotation", 0, 90, fRotation);
    panel->addSliderItem("fps", 0, 300, fps);
    panel->addFlashItem("Reset", bReset);

    buttons.addSliderItem("FPS", 0, 300, fps);
}

//--------------------------------------------------------------
void ofApp::update(){
    fps += (ofGetFrameRate() - fps)/20; // update the fps
    
    escena_zy.update();
    escena_circulo.update();

    if(bReset){ // check for the value of a "Flash item"
        fScale = 1;
        fRotation = 0;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofEnableLighting();
    
    cam.begin();
 mainLight.enable();
    if(ESCENA_XYZ == sceneSelect){
         escena_zy.draw();
    }
    if(ESCENA_CIRCULO == sceneSelect){
         escena_circulo.draw();
    }
    if(sceneSelect == ESCENA_MAPA_VECTORIAL){
         rootNode->draw();
    }
     mainLight.disable();
    cam.end();
    ofDisableLighting();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
*/
