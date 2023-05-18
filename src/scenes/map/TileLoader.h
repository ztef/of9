#ifndef __TileLoader
#define __TileLoader

#include "ofThread.h"
#include "ofMain.h"
#include "FeatureLeafNode.h"
#include "FeatureCollectionNode.h"
#include "JsonLoader.h"
#include "Poco/Condition.h"

 
class Map;

class TileLoader: public ofThread{
    
  public:

    TileLoader();
    
    void start();
    
    void setCallBack(Map* map);
    
    void load(string tilePath);

    void threadedFunction();

    FeatureNode* tile;
    JsonLoader jsonLoader;

    string path;
    bool loading;
    bool loaded;
    
    queue<string> queueUrls;
    Poco::Condition condition;
    
    ofMutex mutex;
    Map* map;
       
};

#endif 
