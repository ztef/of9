//
//  TileDB.hpp
//  of9
//
//  Created by Esteban on 10/06/23.
//

#ifndef TileDB_hpp
#define TileDB_hpp

#include <stdio.h>
#include <iostream>
#include <cstring>
#include <map>
#include <cstring>
#include <fstream>
#include "DecodeGzip.h"
#include "MBTileReader.h"
#include <math.h>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include "VectorTile.h"

using namespace std;




class TileDataBase{
public:
    TileDataBase();
    ~TileDataBase();
    void getInfo();
    
    MBTileReader reader;
    
    
};


#endif /* TileDB_hpp */
