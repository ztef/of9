#ifndef __Map
#define __Map

#include "ofMain.h"
#include "TileLoader.h"
#include "FeatureNode.h"

class Map {
    
public:
    Map();
    void Load(string url);
    
    void tileReady(FeatureNode* tile);

    virtual void draw();

    void update();

    void setup();



    TileLoader tileLoader;
    vector<FeatureNode*> tiles;
    ofMutex mutex;
    bool newTile;
};

#endif 
