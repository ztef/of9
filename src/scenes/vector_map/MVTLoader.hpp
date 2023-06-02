//
//  MVTLoader.hpp
//  of9
//
//  Created by Esteban on 27/05/23.
//

#ifndef MVTLoader_hpp
#define MVTLoader_hpp

#include <stdio.h>
#include "vector_tile.hpp"
#include <fstream>
#include <stdexcept>
#include <iostream>
#include "FeatureNode.h"
#include "Projection.hpp"
#include "FeatureCollectionNode.h"
#include "FeatureLeafNode.h"

class MVTLoader {
public:
    MVTLoader();
    std::string open_tile(std::string const& path);
    FeatureNode* loadTile(std::string fileName, Projection* _p);
    bool open(const std::string& filename);
    bool openLocal(const std::string& filename);
    bool openRemote(const std::string& filename);
    FeatureCollectionNode* parseFeatureCollectionNode(mapbox::vector_tile::layer layer);
    FeatureNode* parseNode(mapbox::vector_tile::feature feature);
    FeatureNode* parseFeatureNode(mapbox::vector_tile::feature feature);
    void parsePolygonGeometry(mapbox::vector_tile::points_arrays_type geom, mapbox::vector_tile::feature::properties_type props, ofMesh *meshToFill, glm::vec3 *anchor, bool multi);
    void parseLineGeometry(mapbox::vector_tile::points_arrays_type geom, mapbox::vector_tile::feature::properties_type props, ofMesh* meshToFill, glm::vec3* anchor);
    vector<glm::vec3> parsePointArrayInProjectedCoords(mapbox::vector_tile::points_array_type pointArray);
    glm::vec3 parsePointInProjectedCoords(std::int16_t x, std::int16_t y);
    ofVec3f getCentroidFromPoints(vector<glm::vec3> pts);
    
    void clear();
    FeatureNode* getNodes();
    FeatureCollectionNode* rootNode;
    Projection* projection;
    std::string buffer = std::string(5000000, ' ');
    mapbox::vector_tile::buffer tile;
    ofColor layerColor;
    float layerHeight;
    
    
     ofTessellator* tessellator;
};

class print_value {

public:
    std::string operator()(std::vector<mapbox::feature::value> val) {
        return "vector";
    }

    std::string operator()(std::unordered_map<std::string, mapbox::feature::value> val) {
        return "unordered_map";
    }

    std::string operator()(mapbox::feature::null_value_t val) {
        return "null";
    }

    std::string operator()(std::nullptr_t val) {
        return "nullptr";
    }

    std::string operator()(uint64_t val) {
        return std::to_string(val);
    }
    std::string operator()(int64_t val) {
        return std::to_string(val);
    }
    std::string operator()(double val) {
        return std::to_string(val);
    }
    std::string operator()(std::string const& val) {
        return val;
    }

    std::string operator()(bool val) {
        return val ? "true" : "false";
    }
};

#endif /* MVTLoader_hpp */
