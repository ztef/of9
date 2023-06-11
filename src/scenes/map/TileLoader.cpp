#include "TileLoader.h"
#include "Map.h"


    TileLoader::TileLoader(){
        loading = false;
    }

    void TileLoader::setDB(TileDataBase &d){
        db = &d;
        mvtLoader.setDB(db);
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

            
            tile = mvtLoader.loadTile(url.url, projection, url.pos);
            
            map->tileReady(tile, url.pos);
           
            
        }
        
    }

    
