#ifndef __TileLoader
#define __TileLoader

#include "ofThread.h"
#include "ofMain.h"
#include "FeatureLeafNode.h"
#include "FeatureCollectionNode.h"

#include "MVTLoader.hpp"
#include "Poco/Condition.h"
#include "Projection.hpp"
#include "TileFunctions.hpp"
 

 
class Map;

struct tile2queue {
  string url;
  tilefunctions::Tile pos;
};




class TileLoader: public ofThread{
    
  public:

    TileLoader();
    
    void setProjection(Projection* _p){
        projection = _p;
    }
    
    void start();
    
    void setCallBack(Map* map);
    
    void load(string tilePath, tilefunctions::Tile pos);
    
    void clear();

    void threadedFunction();

    FeatureNode* tile;
    //JsonLoader jsonLoader;
    MVTLoader mvtLoader;

    string path;
    bool loading;
    bool loaded;
    
    queue<tile2queue> queueUrls;
    Poco::Condition condition;
    
    ofMutex mutex;
    Map* map;
    
    
    
    Projection* projection;
       
};

#endif 
