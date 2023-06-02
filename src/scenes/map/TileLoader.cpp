#include "TileLoader.h"
#include "Map.h"


    TileLoader::TileLoader(){
        loading = false;
    }



    void TileLoader::setCallBack(Map* mapx){
        map = mapx;
    }
 
    void TileLoader::clear(){
        mvtLoader.clear();
    }

    void TileLoader::start(){
        startThread();
    }

    void TileLoader::load(string url){
         mutex.lock();
               queueUrls.push(url);
               condition.signal();
         mutex.unlock();
        cout << "Iniciando thread";
        
    }

    void TileLoader::threadedFunction(){
        
        while(isThreadRunning()){
            mutex.lock();
            if (queueUrls.empty()){
                condition.wait(mutex);
            }
            string url = queueUrls.front();
            queueUrls.pop();
            mutex.unlock();

            //tile = jsonLoader.loadTile(url, projection);
            
            tile = mvtLoader.loadTile(url, projection);
            //tile->setPosition(0, 0, 0);
            map->tileReady(tile);
            
            
        }
        
    }

    
