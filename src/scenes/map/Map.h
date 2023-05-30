#ifndef __Map
#define __Map

#include "ofMain.h"
#include "TileLoader.h"
#include "FeatureNode.h"
#include "Projection.hpp"
#include "ofxRaycaster.h"
#include "TileFunctions.hpp"
#include "Tilec.hpp"
#include "MVTLoader.hpp"
#include "VShaders.hpp"

#define TILE_SIZE 256.0

class Map {
    
public:
    Map();
    ~Map();
    
    void Load(string url);
    
    void tileReady(FeatureNode* tile);
    
    FeatureNode* testTile;

    virtual void draw();
    
    void setmarker(glm::vec3 cursor);

    void update(float zoom);
    int calcTileZoom(float z);

    void setup(double _width, double _height);
    
    int bestZoomForScale(float scale);
    
    // Coordenadas para tiles :
    double tx, ty, sc;
    
    double zoom;
    
   
    
    int    tile_zoom;
    
    double width, height;
    
    
    Projection projection;
    TileLoader tileLoader;
    vector<FeatureNode*> tiles;
    ofMutex mutex;
    bool newTile;
    
     glm::vec3 cursor;
    
    
    tilefunctions::Tile current_tile;
    tilefunctions::Tile target_tile;
    
    map<tilefunctions::Tile, FeatureNode*> loadedtiles;
     
    string target_url;
    MVTLoader mvtLoader;
    
    ofFbo fbo;
    
    //static string mapType;
    
};

#endif 
