#include "Map.h"

Map::Map() {
    
}

void Map::setup(){
    
     
    tileLoader.setCallBack(this);
    tileLoader.start();
}


void Map::Load(string url){
    tileLoader.load(url);
}

void Map::tileReady(FeatureNode* tile){
    mutex.lock();
            tiles.push_back(tile);
            newTile = true;
    mutex.unlock();
}



void Map::update(){
    
    /*for(int i=0;i<tileLoaders.size();i++){
        if(tileLoaders[i]->loaded){
            if(tiles.size()<=i) tiles.resize(i+1);

            tiles[i] = tileLoaders[i]->tile; 
            //tileLoaders[i]->loaded = false;
        }
    }
    */
    
}



void Map::draw(){
    for(int i=0;i<tiles.size();i++){
            tiles[i]->draw();
    }
}


