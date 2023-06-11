#include "Map.h"

Map::Map() {
    
}

Map::~Map(){
    tileLoader.stopThread();
}

//string Map::mapType;

void Map::setup(double _width, double _height){
    
    db.reader.setDB("/Users/esteban/of/apps/myApps/of9/bin/data/naturalearth.mbtiles");
    tileLoader.setDB(db);
    db.getInfo();
    
    projection.setMode(PROJ_SCREEN);
    //projection.setMode(PROJ_MERCATOR);
    //projection.setMode(PROJ_SPHERICAL);
    projection.setScale(1);
    projection.setTranslate(-4096,  4096); // Max extent / 2
     
    tileLoader.setProjection(&projection);
    
    //fbo.allocate(1024, 768, GL_RGBA);
    
    VShaders::lineShader.setGeometryInputType(GL_LINES);
    VShaders::lineShader.setGeometryOutputType(GL_TRIANGLE_STRIP);
    VShaders::lineShader.setGeometryOutputCount(4);
    VShaders::lineShader.load("shaders/vert.glsl", "shaders/frag.glsl", "shaders/geom.glsl");

    cout << "Maximum number of output vertices support is: " << VShaders::lineShader.getGeometryMaxOutputCount() << endl;

   // Map::mapType = "CONTENIDO ESTATICO";
   // VShaders::shaderType = "SHADER";
    
     width = _width;
     height = _height;
     tx = -TILE_SIZE/2.0; // half the world width, at zoom 0
     ty = -TILE_SIZE/2.0; // half the world height, at zoom 0
     // fit to screen
     sc = ceil(min(height/TILE_SIZE, width/TILE_SIZE));
    
     tileLoader.setCallBack(this);
     tileLoader.start();
    
    // mutex.lock();
    // testTile = mvtLoader.loadTile("/Users/esteban/of/apps/myApps/of9/bin/data/0.mvt", &projection);
    // mutex.unlock();
    
}


void Map::Load(string url,tilefunctions::Tile position){
    tileLoader.load(url, position);
}

void Map::tileReady(FeatureNode* tile, tilefunctions::Tile t){
    mutex.lock();
             
            tiles.push_back(tile);
             
            loadedtiles[t] = tile;
    
            newTile = true;
    
    tileLoader.clear();
    

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
    
    // https://vector.hereapi.com/v2/vectortiles/base/mc/12/2200/1343/omv?apikey=giC-ZDlq0kW_bCQoKne7_Un-oiUnQcyI63HqvfoiVjw
    
    //https://api.maptiler.com/tiles/v3/0/0/0.pbf?key=rnYxzTtjDTE0HOr9GI4b
    
    curl -X "GET" \
    "https://vector.hereapi.com/v2/vectortiles/proto/vector_tile.proto?apiKey=giC-ZDlq0kW_bCQoKne7_Un-oiUnQcyI63HqvfoiVjw" \
    -H "accept: text/plain"
    
  */
    
    /*
     https://tile.nextzen.org/tilezen/vector/v1/all/0/0/0.json?api_key=HjxoLw7IQJWSTo4lgErmIQ
     https://tile.nextzen.org/tilezen/vector-tiles-prod/20171221/0/0/0.zip?api_key=HjxoLw7IQJWSTo4lgErmIQ
     */
    
    //current_tile = {0,0,0};
    
    
    if ( requestTile(current_tile)){
      /*
       vector<tilefunctions::Tile> vecinos = tilefunctions::brothers(current_tile,0);
           for(auto v: vecinos){
              requestTile(v);
          }
       */
    }
    
    // Borra Tiles fuera de SCOPE
    /*
    map<tilefunctions::Tile, FeatureNode*>::iterator iter = loadedtiles.begin();
    map<tilefunctions::Tile, FeatureNode*>::iterator endIter = loadedtiles.end();
    for (; iter != endIter;) {
        tilefunctions::Tile tile = iter->first;
        FeatureNode* node = iter->second;
        if (tile.z > tile_zoom  ) {   // || (tile.z == 0)
            if(node != nullptr){
                    node->draw();
            }
        }
        ++iter;
    }
     */
    
    // Vecinos :
   
    
    
}

bool Map::requestTile(tilefunctions::Tile  _tile){
    if(loadedtiles.count(_tile) == 0){
           loadedtiles[_tile] = nullptr;
           stringstream url;
           //url << "https://tile.nextzen.org/tilezen/vector/v1/512/all/" << _tile.z << "/" << _tile.x << "/" << _tile.y << ".mvt?api_key=HjxoLw7IQJWSTo4lgErmIQ";
        
           //url << "https://vector.hereapi.com/v2/vectortiles/core/mc/" << _tile.z << "/" << _tile.x << "/" << _tile.y << "/omv?apikey=giC-ZDlq0kW_bCQoKne7_Un-oiUnQcyI63HqvfoiVjw";
        
          url << "https://api.maptiler.com/tiles/v3/" << _tile.z << "/" << _tile.x << "/" << _tile.y << ".pbf?key=rnYxzTtjDTE0HOr9GI4b";
        
         //  url << "/Users/esteban/of/apps/myApps/of9/bin/data/ne0.pbf";
        
          // url << "db";
        
           string target_url = url.str();
           
           Load(target_url,_tile);   // Solicita la carga del actual
        return true;
    }
    return false;
}

int Map::calcTileZoom(float z){
    if(z > 3500){
        return 0;
    }
    if(z > 1000){
        return 1;
    }
    if(z > 500){
        return 2;
    }
    if(z > 200){
        return 3;
    }
    
    if(z > 100){
        return 4;
    }
    if(z > 60){
        return 5;
    }
    if(z > 50){
        return 6;
    }
    if(z > 25){
        return 7;
    }
    if(z > 15){
        return 8;
    }
    if(z > 12){
        return 9;
    }
    if(z > 10){
        return 10;
    }
    if(z > 8){
         return 11;
     }
    if(z > 5){
        return 12;
    }
    return 13;
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
    
     
        //testTile->draw();
    ofSetColor(0, 255, 0);
    ofFill();
    ofDrawPlane(glm::vec3(0,0,-1), 8192, 8192);
    
    map<tilefunctions::Tile, FeatureNode*>::iterator iter = loadedtiles.begin();
    map<tilefunctions::Tile, FeatureNode*>::iterator endIter = loadedtiles.end();
    for (; iter != endIter;) {
        tilefunctions::Tile tile = iter->first;
        FeatureNode* node = iter->second;
        if (tile.z == tile_zoom  ) {   // || (tile.z == 0)
            if(node != nullptr){
                
                    node->draw();
            } else {   // No ha sido cargado, dibuja el 0 (debe ser el anterior)
                
            }
        } else {   // Resto del Mundo
            FeatureNode* node0;
            node0 = loadedtiles[tilefunctions::Tile{0,0,0}];
            if(node0 != nullptr){
        //        node0->draw();
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

