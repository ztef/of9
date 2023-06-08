#include "TileLoader.h"
#include "Map.h"


    TileLoader::TileLoader(){
        loading = false;
    }



    void TileLoader::setCallBack(Map* mapx){
        map = mapx;
    }
 
    void TileLoader::clear(){
        
    }

    void TileLoader::start(){
        startThread();
    }

    void TileLoader::load(string url, tilefunctions::Tile pos){
         mutex.lock();
        tile2queue tq = {url,pos};
        queueUrls.push(tq);
               condition.signal();
         mutex.unlock();
       // cout << "Iniciando thread";
        
    }

    void TileLoader::threadedFunction(){
        
        while(isThreadRunning()){
            mutex.lock();
            if (queueUrls.empty()){
                condition.wait(mutex);
            }
            tile2queue url = queueUrls.front();
            queueUrls.pop();
            mutex.unlock();

            //tile = jsonLoader.loadTile(url, projection);
            
            tile = mvtLoader.loadTile(url.url, projection, url.pos);
            //tile->setPosition(0,0,0);
            map->tileReady(tile);
           
            
        }
        
    }

    
