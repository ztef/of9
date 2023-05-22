//
//  Tile.hpp
//  of9
//
//  Created by Esteban on 21/05/23.
//

#ifndef Tilec_hpp
#define Tilec_hpp

#include <stdio.h>
#include <math.h>
#include <iostream>

class Tilec {
    
public:
    
    int row;
    int column;
    int zoom;
    
    int & x = row;
    int & y = column;
    int & z = zoom;
    
    Tilec() : row(0), column(0), zoom(0) { }
    
    Tilec(int _row, int _column, int _zoom) : row(_row), column(_column), zoom(_zoom) {}
    
    Tilec(const Tilec &coord) : row(coord.row), column(coord.column), zoom(coord.zoom) {}
    
    friend std::ostream &operator<<(std::ostream &stream, Tilec c) {
        // TODO: number format
        stream << "(" << c.row << ", " << c.column << " @" << c.zoom << ")";
        return stream;
    }
    
    bool operator == (const Tilec &c) const
    {
        return c.row == row && c.column == column && c.zoom == zoom;
    }

    bool operator < (const Tilec &c) const
    {
        //cout << *this << " compared to " << c << endl;
        return zoom < c.zoom || (zoom == c.zoom && row < c.row) || (zoom == c.zoom && row == c.row && column < c.column);
    }
    
    Tilec container() {
        return Tilec(floor(row), floor(column), zoom);
    }
    
    Tilec zoomTo(int destination) {
        return Tilec(row * pow(2, destination - zoom),
                          column * pow(2, destination - zoom),
                          destination);
    }
    
    Tilec zoomBy(int distance) {
        return Tilec(row * pow(2, distance),
                          column * pow(2, distance),
                          zoom + distance);
    }
    
    Tilec up(int distance=1) {
        return Tilec(row - distance, column, zoom);
    }
    
    Tilec right(int distance=1) {
        return Tilec(row, column + distance, zoom);
    }
    
    Tilec down(int distance=1) {
        return Tilec(row + distance, column, zoom);
    }
    
    Tilec left(int distance=1) {
        return Tilec(row, column - distance, zoom);
    }
    
};



#endif /* Tile_hpp */
