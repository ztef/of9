//
//  MercatorTile.hpp
//  of9
//
//  Created by Esteban on 18/05/23.
//

#ifndef TileFunctions_hpp
#define TileFunctions_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include <deque>


namespace tilefunctions
{

// An XYZ web mercator tile
struct Tile
{
    int x;
    int y;
    int z;
    
    bool operator < (const Tile &t) const
       {
           //cout << *this << " compared to " << c << endl;
           return z < t.z || (z == t.z && x < t.x) || (z == t.z && x == t.x && y < t.y);
       }
    
    bool operator == (const Tile &t) const
    {
        return t.x == x && t.y == y && t.z == z;
    }
    
    
    
};

// A longitude and latitude pair
struct LngLat
{
    double lng;
    double lat;
};

// A x and y pair of web mercator
struct XY
{
    double x;
    double y;
};

// A geographic bounding box
struct LngLatBbox
{
    double west;
    double south;
    double east;
    double north;
};

// A web mercator bounding box
struct Bbox
{
    double left;
    double bottom;
    double right;
    double top;
};

void truncate_lonlat(double *lng, double *lat);

// Returns the upper left longitude and latitude of a tile
LngLat ul(const Tile &tile);

// Get the lonlat bounding box of a tile
LngLatBbox bounds(const Tile &tile);

// Convert longtitude and latitude to web mercator x, y.
XY xy(const double &lng, const double &lat);

// Convert  web mercator x, y to longtitude and latitude.
LngLat lnglat(const double &x, const double &y);

// Get the web mercator bounding box of a tile
Bbox xy_bounds(const Tile &tile);

// Get the tile containing a longitude and latitude
Tile tile(const double &lng, const double &lat, const int &zoom);

// Get the parent of a tile
Tile parent(const Tile &tile);
Tile parent(const Tile &tile, const int &zoom);

// Get the child of a tile
std::vector<Tile> children(const Tile &tile);
std::vector<Tile> children(const Tile &tile, const int &zoom);

// Get the tiles intersecting a geographic bounding box
std::vector<Tile> tiles(const LngLatBbox &bbox, const int &zoom);

// Get the quadkey of a tile
std::string quadkey(const Tile &tile);

// Get the tile corresponding to a quadkey
Tile quadkey_to_tile(const std::string &qk);


} //namespace tilefunctions



#endif /* TileFunctions_hpp */
