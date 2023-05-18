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
    mapa.setup();
   


   // ofSetDataPathRoot("/home/esteban/of/apps/myApps/of8/bin/data/");

    shader.load("myshader.vert", "myshader.frag"); 


    m.rotate(90,0,0,1);

   //JsonLoader jsonLoader = JsonLoader("mx_states.geojson");
    //JsonLoader jsonLoader = JsonLoader("24642.json");
    
    
    /*
    ofURLFileLoader fl = ofURLFileLoader();
    string url = "https://tile.nextzen.org/tilezen/vector/v1/all/16/19293/24642.json?api_key=HjxoLw7IQJWSTo4lgErmIQ";
    
    string url1 = "https://forum.openframeworks.cc/uploads/default/original/2X/7/75b52b04984d6a0a060f48bc8523e9e6deef1618.svg";
    
    ofHttpRequest request(url,url);
    
    //request.contentType = "application/json";

    
    
    ofHttpResponse resp;
    resp = fl.handleRequest(request);
    
    
    cout << resp.status << endl;
    cout << resp.error << endl;
    cout << resp.data.getText() << endl;
    
    //JsonLoader jsonLoader = JsonLoader();

   // JsonLoader jsonLoader = JsonLoader("https://tile.nextzen.org/tilezen/vector/v1/all/0/0/0.json?api_key=HjxoLw7IQJWSTo4lgErmIQ");
   // JsonLoader jsonLoader1 = JsonLoader("https://tile.nextzen.org/tilezen/vector/v1/all/1/0/0.json?api_key=HjxoLw7IQJWSTo4lgErmIQ");
  
    //JsonLoader jsonLoader = JsonLoader("https://tile.nextzen.org/tilezen/vector/v1/all/16/19293/24642.json?api_key=HjxoLw7IQJWSTo4lgErmIQ");
    
   // rootNode = jsonLoader.loadNodeGraph();
   // rootNode = jsonLoader.loadTile("https://tile.nextzen.org/tilezen/vector/v1/all/0/0/0.json?api_key=HjxoLw7IQJWSTo4lgErmIQ");

   // rootNode->setPosition(0,0, 0);
   // rootNode->shader = shader;
   // rootNode->printPosition("");

   // rootNode1 = jsonLoader1.loadNodeGraph();
   //   rootNode1 = jsonLoader.loadTile("https://tile.nextzen.org/tilezen/vector/v1/all/3/3/3.json?api_key=HjxoLw7IQJWSTo4lgErmIQ");

   //rootNode1->setPosition(0,0, 200);
    //rootNode1->shader = shader;
   // rootNode1->printPosition("");

*/
    mapa.Load("https://tile.nextzen.org/tilezen/vector/v1/all/0/0/0.json?api_key=HjxoLw7IQJWSTo4lgErmIQ");
    mapa.Load("https://tile.nextzen.org/tilezen/vector/v1/all/3/3/3.json?api_key=HjxoLw7IQJWSTo4lgErmIQ");
   //pois = (FeatureCollectionNode*) jsonLoader.getCollection()->children.at(0);
    
   // cout << "Hijos :";
   // int hijos =  pois->children.size();
   // cout << hijos;
    
   // building0 = (FeatureLeafNode*) pois->children.at(0);
    
    
    
    //for (int i = 0; i < buildings->children.size(); i++) {
         
    //}
    
    //[-100.2971238,25.6519369]
   // x = jsonLoader.getXX(-100.2971238,1024);
    //y = jsonLoader.getYY(25.6519369,2024,1024);
    //pos.set(x,y);
    
    
    // General graphics setup
   
    ofBackground(0, 0, 0);
    ofSetFrameRate(60);
    ofEnableDepthTest();
    
    //camera.removeAllInteractions();
    //camera.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_XY, OF_MOUSE_BUTTON_LEFT);
    //camera.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_Z, OF_MOUSE_BUTTON_RIGHT);
    //camera.enableOrtho();
    //camera.setNearClip(-1000000);
    //camera.setFarClip(1000000);
    //camera.setVFlip(true);
    
   // camera.setPosition(rootNode->getGlobalPosition());
    camera.setNearClip(0.001);
    camera.setFarClip(1000000);
    camera.move(0, 0, 300);
   
   // camera.setTarget(rootNode->getGlobalPosition());
    
    once = false;
    
    // Lighting setup
    mainLight = ofLight();
    mainLight.setup();
    mainLight.setPosition(-100, 200,0);
   
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

    escena_zy.update();
    escena_circulo.update();
    mapa.update();


}

//--------------------------------------------------------------
void ofApp::draw(){

    ofSetColor(255);
    
   // ofEnableLighting();
    
    
    
    zoom = camera.getZ();
    
    //cout << zoom;
    //cout << "/";
    if (zoom < 3.0 ){
        //camera.move(camera.getX(), camera.getY(), zoom);
        //camera.lookAt();
       
        //camera.setPosition(buildings->getGlobalPosition());
        
        if(not once){
            
            once = true;
            //camera.move(buildings->getX(),buildings->getY(),zoom);
            //camera.lookAt( buildings->getGlobalPosition());
       
            //camera.setPosition(buildings->getGlobalPosition());
            //camera.move(0, 0, zoom);
            //camera.setTarget(pois->getGlobalPosition());
            
            //camera.setDrag(0.99);
           // camera.setRotationSensitivity(0.1,0.1, 0.1);
            //camera.setTranslationSensitivity(0.1,0.1, 0.1);
        }
    } else {
        //camera.setDrag(0);
       // camera.setRotationSensitivity(1,1, 1);
        //camera.setTranslationSensitivity(1,1,1);
        
    }
    
    camera.begin();
    
    
    
    //mainLight.enable();
    

        
    if(ESCENA_XYZ == sceneSelect){
        escena_zy.draw();
    }
    if(ESCENA_CIRCULO == sceneSelect){
         escena_circulo.draw();
    }
    if(sceneSelect == ESCENA_MAPA_VECTORIAL){
       // rootNode->draw();
        //rootNode1->draw();
        //rootNode1->setScale(1000);
        mapa.draw();
      
    }



    
 //   mainLight.disable();
    
   camera.end();
    
    if(sceneSelect == ESCENA_MAPA_VECTORIAL){
        
        /*
         int nn = buildings->children.size();
                  for(int i = 0; i < nn; i++) {
                      glm::vec3 cur = buildings->children.at(i)->getGlobalPosition();
                      ofSetColor(ofColor::cyan);//TOP
                      ofDrawCylinder(cur,0.001,  0.01);
                  }
        */
        
      //  ofPushMatrix();
      //     ofMultMatrix(rootNode->getLocalTransformMatrix());
           
         
           
         //  building0->transformGL();
        
        
        //for (int i = 0; i < buildings->children.size(); i++) {
             
        //}
        
        
        //int n = building0->geometry.getNumVertices();
        
        /*
        int n = pois->children.size();
        float nearestDistance = 0;
        glm::vec2 nearestVertex;
        int nearestIndex = 0;
        glm::vec3 mouse(mouseX, mouseY,0);
        for(int i = 0; i < n; i++) {
           //  glm::vec3 cur = camera.worldToScreen(pois->children.at(i)->getGlobalPosition());
            glm::vec3 cur = camera.worldToScreen(pois->children.at(i)->anchor - pois->anchor) ;
            ofSetColor(ofColor::red);
            //ofDrawCylinder(cur.x,cur.y,0,0.5,  10);
            //glm::vec3 cur = building0->geometry.getVertex(i);
            //glm::vec3 cur = building0->getPosition();
            float distance = glm::distance(cur, mouse);
            if(i == 0 || distance < nearestDistance) {
                nearestDistance = distance;
                nearestVertex = cur;
                nearestIndex = i;
            }
        }
         
        
       
        
            ofSetColor(ofColor::gray);
            ofDrawLine(nearestVertex, mouse);
        
            ofNoFill();
            ofSetColor(ofColor::yellow);
            ofSetLineWidth(2);
            ofDrawCircle(nearestVertex, 4);
            ofSetLineWidth(1);
        
            glm::vec2 offset(0, 0);
            ofDrawBitmapStringHighlight(ofToString(nearestIndex), mouse + offset);
       */
        //building0->restoreTransformGL();
        
       // ofPopMatrix();
    }
    
    
    
    
  // ofDisableLighting();
    
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
