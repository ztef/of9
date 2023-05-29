#include "Map.h"

Map::Map() {
    
}

Map::~Map(){
    tileLoader.stopThread();
}

void Map::setup(double _width, double _height){
    
    projection.setMode(PROJ_SCREEN);
    //projection.setMode(PROJ_MERCATOR);
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
    
     mutex.lock();
     testTile = mvtLoader.loadTile("/Users/esteban/of/apps/myApps/of9/bin/data/0.mvt", &projection);
     mutex.unlock();
    
}


void Map::Load(string url){
    tileLoader.load(url);
}

void Map::tileReady(FeatureNode* tile){
    mutex.lock();
             
            tiles.push_back(tile);
             
            loadedtiles[current_tile] = tile;
    
            newTile = true;
    

    mutex.unlock();
}

void Map::setmarker(glm::vec3 _c){
    cursor = _c;
}

void Map::update(float _zoom){
    
    
    zoom = _zoom;
    tile_zoom = calcTileZoom(zoom);
    
    
    Coordinate c = projection.getCoordinate(cursor);
    current_tile = tilefunctions::tile(c.longitude, c.latitude, tile_zoom);
    target_tile =  tilefunctions::tile(c.longitude, c.latitude, tile_zoom + 1);
    
    // Checa si target_tile ya esta cargado
    
    //c340cb34-fcb6-4449-a444-2bc6937c112a
    
    //map tiler
    // rnYxzTtjDTE0HOr9GI4b
    
    //net toolkit test_wPj4EPYSmgj9JjbZhKgxC2yODtCvqSgBxvDwdzI3
    
    // HERE   giC-ZDlq0kW_bCQoKne7_Un-oiUnQcyI63HqvfoiVjw
    
   /* curl -X "GET" \
    "https://vector.hereapi.com/v2/vectortiles/proto?apiKey=giC-ZDlq0kW_bCQoKne7_Un-oiUnQcyI63HqvfoiVjw" \
    -H "accept: application/json"
    
    
    curl -X "GET" \
    "https://vector.hereapi.com/v2/vectortiles/proto/vector_tile.proto?apiKey=giC-ZDlq0kW_bCQoKne7_Un-oiUnQcyI63HqvfoiVjw" \
    -H "accept: text/plain"
    
  */
    
    /*
     https://tile.nextzen.org/tilezen/vector/v1/all/0/0/0.json?api_key=HjxoLw7IQJWSTo4lgErmIQ
     https://tile.nextzen.org/tilezen/vector-tiles-prod/20171221/0/0/0.zip?api_key=HjxoLw7IQJWSTo4lgErmIQ
     */
    
   
    if(loadedtiles.count(current_tile) == 0){
        loadedtiles[current_tile] = nullptr;
        stringstream url;
        url << "https://tile.nextzen.org/tilezen/vector/v1/all/" << current_tile.z << "/" << current_tile.x << "/" << current_tile.y << ".json?api_key=HjxoLw7IQJWSTo4lgErmIQ";
        target_url = url.str();
        
        Load(target_url);
    }
    
   
    
}

int Map::calcTileZoom(float z){
    if(z > 0.63){
        return 0;
    }
    if(z > 0.28){
        return 1;
    }
    if(z > 0.2){
        return 2;
    }
    if(z > 0.08){
        return 3;
    }
    if(z > 0.07){
        return 4;
    }
    if(z > 0.06){
        return 5;
    }
    if(z > 0.05){
        return 6;
    }
    if(z > 0.04){
        return 7;
    }
    if(z > 0.03){
        return 8;
    }
    if(z > 0.02){
        return 9;
    }
    if(z > 0.01){
        return 10;
    }
    return 11;
}
    

/* float zoomout;
 
 */

void Map::draw(){
    /*
    for(int i=0;i<tiles.size();i++){
        bool visible=true;
        visible = (tile_zoom == 0 && i==0);
        visible = visible || (tile_zoom == 1 && i> 0);
        if(visible){
            tiles[i]->draw();
        }
    }
    */
    
    testTile->draw();
    
    map<tilefunctions::Tile, FeatureNode*>::iterator iter = loadedtiles.begin();
    map<tilefunctions::Tile, FeatureNode*>::iterator endIter = loadedtiles.end();
    for (; iter != endIter;) {
        tilefunctions::Tile tile = iter->first;
        FeatureNode* node = iter->second;
        if (tile.z == tile_zoom ) {   // || (tile.z == 0)
            if(node != nullptr){
                  //  node->draw();
            } else {   // No ha sido cargado, dibuja el 0 (debe ser el anterior)
                FeatureNode* node0;
                node0 = loadedtiles[tilefunctions::Tile{0,0,0}];
                if(node0 != nullptr){
                  //  node0->draw();
                }
            }
        }
        
        ++iter;
        
    }
    
    
}

int Map::bestZoomForScale(float scale) {
    // TODO get min/max zoom level from provider?
    // TODO log(2) const?
    return (int)min(20, max(1, (int)round(log(scale) / log(2))));
}

