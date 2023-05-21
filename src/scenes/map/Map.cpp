#include "Map.h"

Map::Map() {
    
}

Map::~Map(){
    tileLoader.stopThread();
}

void Map::setup(double _width, double _height){
    
    projection.setMode(PROJ_MERCATOR);
    //projection.setMode(PROJ_SPHERICAL);
    projection.setScale(1);
    projection.setTranslate(0, 0);
    
    tileLoader.setProjection(&projection);
    
    
     width = _width;
     height = _height;
     tx = -TILE_SIZE/2.0; // half the world width, at zoom 0
     ty = -TILE_SIZE/2.0; // half the world height, at zoom 0
     // fit to screen
     sc = ceil(min(height/TILE_SIZE, width/TILE_SIZE));
    
     tileLoader.setCallBack(this);
     tileLoader.start();
}


void Map::Load(string url){
    tileLoader.load(url);
}

void Map::tileReady(FeatureNode* tile){
    mutex.lock();
            //tile->setPosition(0, 0, 0);
            tiles.push_back(tile);
            // 0,1
            if(tiles.size() == 2){
            //    tile->setPosition((0-TILE_SIZE/2)-TILE_SIZE * (3-2),(0+TILE_SIZE/PI),15);
                //tile->setScale(2);
                 //tile->setPosition(0, 0, 100);
                //tile->dolly(100);
                
            }
            //1,1
            if(tiles.size() == 3){
            //    tile->setPosition((0-TILE_SIZE/2)-TILE_SIZE * (3-3),(0+TILE_SIZE/PI),10);
                //tile->setScale(2);
                  //tile->setPosition(0, 0, 120);
                 //tile->dolly(200);
            }
    
            newTile = true;
    mutex.unlock();
}



void Map::update(float _zoom){
    
    
    zoom = _zoom;
    tile_zoom = calcTileZoom(zoom);
    /*for(int i=0;i<tileLoaders.size();i++){
        if(tileLoaders[i]->loaded){
            if(tiles.size()<=i) tiles.resize(i+1);

            tiles[i] = tileLoaders[i]->tile; 
            //tileLoaders[i]->loaded = false;
        }
    }
    */
    
    
    
    
}

int Map::calcTileZoom(float z){
    if(z > 0.45){
        return 0;
    }
    if(z <= 0.45){
        return 1;
    }
    return 0;
}


void Map::draw(){
    for(int i=0;i<tiles.size();i++){
        bool visible=true;
        visible = (tile_zoom == 0 && i==0);
        visible = visible || (tile_zoom == 1 && i> 0);
        if(visible){
            tiles[i]->draw();
        }
    }
    
    
    
}

int Map::bestZoomForScale(float scale) {
    // TODO get min/max zoom level from provider?
    // TODO log(2) const?
    return (int)min(20, max(1, (int)round(log(scale) / log(2))));
}

