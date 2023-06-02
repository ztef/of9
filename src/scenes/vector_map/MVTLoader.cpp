//
//  MVTLoader.cpp
//  of9
//
//  Created by Esteban on 27/05/23.
//

#include "MVTLoader.hpp"


 


MVTLoader::MVTLoader(){
       
    tessellator = new ofTessellator();
    
}

 std::string MVTLoader::open_tile(std::string const& path) {
     std::ifstream stream(path.c_str(),std::ios_base::in|std::ios_base::binary);
     if (!stream.is_open())
     {
         throw std::runtime_error("could not open: '" + path + "'");
     }
     std::string message(std::istreambuf_iterator<char>(stream.rdbuf()),(std::istreambuf_iterator<char>()));
     stream.close();
     return message;
 }

FeatureNode* MVTLoader::loadTile(std::string fileName, Projection* _p){
    projection = _p;
    if(open(fileName)){
        // tile esta cargado con los datos.
        
        
        /*
        string layername = "earth";
        
        if(tile.isLayer(layername)){
            const mapbox::vector_tile::layer layer = tile.getLayer(layername);
        
            std::size_t feature_count = layer.featureCount();
            std::cout << "Layer '" << layername << "'\n";
            std::cout << "  Features: " << feature_count << "\n";
        }
         */
        
        return getNodes();
        
    }
}

FeatureNode* MVTLoader::getNodes() {
    
    vector<FeatureNode*> layers;
    
    ofLog(OF_LOG_VERBOSE, "Loading Layers:");
    
    std::string layerNames[9] = {
         
        //"administrative",
        "earth",
        "water",
        "boundaries",
        "landuse",
        "places",
        "roads",
        "transit",
        "pois",
        "buildings"
         
    };
    
    ofColor layerColors[9] = {
        
        
        ofFloatColor::lawnGreen,
        ofFloatColor::deepSkyBlue,
        ofFloatColor::white,
        ofFloatColor::maroon,
        ofFloatColor::red,
        ofFloatColor::violet,
        ofFloatColor::red,
        ofFloatColor::red,
        ofFloatColor::grey
    };
    
    float layerHeights[9] = {
        
        0.0f,
        0.00001f,
        10.1f,
        0.08f,
        0.1f,
        0.2f,
        0.3f,
        0.4f,
        0.5f
    };
    
    for (int i = 0; i < 9; i++) {
        if(tile.isLayer(layerNames[i])){
            layerColor = layerColors[i];
            layerHeight = layerHeights[i];
            
            FeatureCollectionNode* newLayer = parseFeatureCollectionNode(tile.getLayer(layerNames[i]));

            if(newLayer->children.size()>0){

                newLayer->move(0, 0, layerHeights[i]);
                newLayer->layerColor = layerColor;
                newLayer->layerName = layerNames[i];
                layers.push_back(newLayer);
                
            }
        }
    }
    
    rootNode = new FeatureCollectionNode(layers);
    
    return rootNode;
}


FeatureCollectionNode* MVTLoader::parseFeatureCollectionNode(mapbox::vector_tile::layer  layer) {
    
    //ofxJSONElement features = collectionJson["features"];
    std::size_t feature_count = layer.featureCount();
    
    vector<FeatureNode*> featureNodes;
    
    for (int i = 0; i < feature_count; i++) {
        auto const feature = mapbox::vector_tile::feature(layer.getFeature(i), layer);
        featureNodes.push_back(parseNode(feature));
    }
    
    FeatureCollectionNode* newFeatureCollection = new FeatureCollectionNode(featureNodes);
    
    // aqui
    newFeatureCollection->layerColor =ofFloatColor::deepSkyBlue;
    
    
    return newFeatureCollection;
    
}
    

FeatureNode* MVTLoader::parseNode(mapbox::vector_tile::feature feature) {
    
   auto const& feature_id = feature.getID();
     // if (feature_id.is<uint64_t>()) {
     //     std::cout << "    id: " << feature_id.get<uint64_t>() << "\n";
     // } else {
     //     std::cout << "    id: (no id set)\n";
     // }
    mapbox::vector_tile::GeomType nodeType = feature.getType();
     // std::cout << "    type: " << int(feature.getType()) << "\n";
      
    auto props = feature.getProperties();
     // std::cout << "    Properties:\n";
     // for (auto const& prop : props) {
     //     print_value printvisitor;
     //     std::string value = mapbox::util::apply_visitor(printvisitor, prop.second);
     //     std::cout << "      " << prop.first  << ": " << value << "\n";
     // }
    
     //  std::cout << "    Geometrias:\n";
      mapbox::vector_tile::points_arrays_type geom = feature.getGeometries<mapbox::vector_tile::points_arrays_type>(1.0);
    
     //     for (auto const& point_array : geom) {
     //              for (auto const& point : point_array) {
     //                  std::clog << point.x << "," << point.y;
     //              }
     //     }
          std::clog << "\n";
    
          return parseFeatureNode(feature);
    
    /*
    if(nodeType == mapbox::vector_tile::UNKNOWN) {
           return parseFeatureNode(nodeJson);
      } else if (nodeType == mapbox::vector_tile::UN) {
          return parseFeatureCollectionNode(nodeJson);
      } else {
          ofLog(OF_LOG_WARNING, "Couldn't match Feature type; regresando nodo vacio");
          return new FeatureNode();
      }
    */
    
}

FeatureNode* MVTLoader::parseFeatureNode(mapbox::vector_tile::feature feature) {
    
    //ofxJSONElement coords = featureJson["geometry"]["coordinates"];
    //ofxJSONElement props = featureJson["properties"];
    
    mapbox::vector_tile::points_arrays_type geom = feature.getGeometries<mapbox::vector_tile::points_arrays_type>(1.0);
    auto props = feature.getProperties();
    
    
    
    ofMesh newMesh = ofMesh();
    
    glm::vec3 anchor;
    
    //std::string type = featureJson["geometry"]["type"].asString();
    mapbox::vector_tile::GeomType type = feature.getType();
    
    string typeString = "";
    
    if(type == mapbox::vector_tile::POINT) {
        cout << "punto";
        typeString = "Point";
        
            
        
        anchor = parsePointInProjectedCoords(geom[0][0].x,geom[0][0].y);
              
               newMesh.addVertex(glm::vec3(0, 0, 0));
               newMesh.setMode(OF_PRIMITIVE_POINTS);
               newMesh.setupIndicesAuto();
        
        
        
    } else if (type == mapbox::vector_tile::LINESTRING){
        cout << "linea";
        typeString = "Line";
        
        parseLineGeometry(geom, props, &newMesh, &anchor);
        anchor = newMesh.getCentroid();
        
        
    } else if (type == mapbox::vector_tile::POLYGON){
            typeString = "Polygon";
            newMesh.setMode(OF_PRIMITIVE_TRIANGLES);
            parsePolygonGeometry(geom, props, &newMesh, &anchor, false);
        
    } else {
        
    }
    
    newMesh.setColorForIndices(0, newMesh.getNumIndices(), layerColor);
    
    FeatureLeafNode* newNode = new FeatureLeafNode(newMesh, typeString);
    /*
    newNode->idString = featureJson["id"].asString();
    newNode->level = featureJson["properties"]["level"].asInt();
    newNode->name = featureJson["properties"]["name"].asString();
    */
    
     newNode->anchor = anchor;
    
    return newNode;
    
}

void MVTLoader::parsePolygonGeometry(mapbox::vector_tile::points_arrays_type geom, mapbox::vector_tile::feature::properties_type props, ofMesh *meshToFill, glm::vec3 *anchor, bool multi) {
    
    float height = 0;
    float minHeight = 0;
    float heightFactor = 0.00001;
    string kind = "";
    
    
    vector<ofPolyline> polyLines;
    
     
      
     int i=0;
     for (auto const& point_array : geom) {
             vector<glm::vec3> verts = parsePointArrayInProjectedCoords(point_array);
             if (i == 0) {
                 *anchor = getCentroidFromPoints(verts);
             }
            polyLines.push_back(verts);
         i++;
     }
    
       
      
     tessellator->tessellateToMesh(polyLines, OF_POLY_WINDING_ODD, *meshToFill, false);
      
    
    // Add normals for the top surface
    for (int i = 0; i < meshToFill->getNumVertices(); i++) {
        meshToFill->addNormal(ofVec3f(0, 0, 1));
    }
    
    if (height != 0) {
        
        
        // Extrude outermost (first) polyLine down to minHeight
        vector<glm::vec3> outlineVerts = polyLines[0].getVertices();
        
        for (int i = 0; i < outlineVerts.size() - 1; i++) {
            
            int nV = meshToFill->getNumVertices();
            
            //glm::vec3 norm = (meshToFill->getNormals()[i]).crossed(outlineVerts[i+1] - outlineVerts[i]);
            glm::vec3 norm = glm::cross(meshToFill->getNormals()[i], outlineVerts[i+1] - outlineVerts[i]);

            meshToFill->addVertex(outlineVerts[i]);
            meshToFill->addNormal(norm);
            meshToFill->addVertex(outlineVerts[i+1]);
            meshToFill->addNormal(norm);
            meshToFill->addVertex(ofVec3f(outlineVerts[i].x, outlineVerts[i].y, minHeight));
            meshToFill->addNormal(norm);
            meshToFill->addTriangle(nV, nV + 1, nV + 2);
            meshToFill->addVertex(ofVec3f(outlineVerts[i+1].x, outlineVerts[i+1].y, minHeight));
            meshToFill->addNormal(norm);
            meshToFill->addTriangle(nV + 1, nV + 3, nV + 2);
            
        }

    }

}


void MVTLoader::parseLineGeometry(mapbox::vector_tile::points_arrays_type geom, mapbox::vector_tile::feature::properties_type props, ofMesh* meshToFill, glm::vec3* anchor) {
    
   
    meshToFill->setMode(OF_PRIMITIVE_LINE_STRIP);

     int i=0;
     for (auto const& point_array : geom) {
             vector<glm::vec3> verts = parsePointArrayInProjectedCoords(point_array);
             if (i == 0) {
                 *anchor = getCentroidFromPoints(verts);
             }
             //meshToFill->addVertices(verts);
             
         for(auto const& vert : verts){
             meshToFill->addColor(layerColor);
             meshToFill->addVertex(vert);
         }
         
         
            
         i++;
     }
     
    meshToFill->setupIndicesAuto();
    
    for (int i = 0; i < meshToFill->getNumVertices(); i++) {
        meshToFill->addNormal(glm::vec3(0, 0, 1));
    }
    
}

vector<glm::vec3> MVTLoader::parsePointArrayInProjectedCoords(mapbox::vector_tile::points_array_type point_array) {
    
    vector<glm::vec3> pts;
        for (auto const& point : point_array) {
        pts.push_back(parsePointInProjectedCoords(point.x, point.y));
    }

    return pts;
}

glm::vec3 MVTLoader::parsePointInProjectedCoords(std::int16_t x, std::int16_t y) {
    
    Coordinate coord;
    
    coord.longitude = x*1;
    coord.latitude = y*1;
    
    
    ofPoint p  = projection->getProjection(coord);
     
    
    
    return glm::vec3(p.x,p.y,layerHeight);
    
}

ofVec3f MVTLoader::getCentroidFromPoints(vector<glm::vec3> pts) {
    glm::vec3 centroid;
    
    for (int i = 0; i < pts.size(); i++) {
        centroid += pts[i] / pts.size();
    }
    
    return centroid;
}


bool MVTLoader::open(const std::string& filename)
{
    if (filename.find("http://") == 0 || filename.find("https://") == 0)
    {
        return openRemote(filename);
    }
    else
    {
        return openLocal(filename);
    }
}

void MVTLoader::clear(){
   // buffer = "";
    
}

bool MVTLoader::openRemote(const std::string& filename)
{
    
    buffer = ofLoadURL(filename).data.getText();

    tile.parse(buffer);
    
      
    return true;
}

bool MVTLoader::openLocal(const std::string& filename)
{
    buffer = open_tile(filename);
    tile.parse(buffer);

    /*
    std::cout << "Decoding tile: \n";
    
    for (auto const& name : tile.layerNames()) {
        
        for (std::size_t i=0;i<feature_count;++i) {
            auto const feature = mapbox::vector_tile::feature(layer.getFeature(i),layer);
            auto const& feature_id = feature.getID();
            if (feature_id.is<uint64_t>()) {
                std::cout << "    id: " << feature_id.get<uint64_t>() << "\n";
            } else {
                std::cout << "    id: (no id set)\n";
            }
            std::cout << "    type: " << int(feature.getType()) << "\n";
            auto props = feature.getProperties();
            std::cout << "    Properties:\n";
            for (auto const& prop : props) {
                print_value printvisitor;
                std::string value = mapbox::util::apply_visitor(printvisitor, prop.second);
                std::cout << "      " << prop.first  << ": " << value << "\n";
            }
            std::cout << "    Vertices:\n";
            mapbox::vector_tile::points_arrays_type geom = feature.getGeometries<mapbox::vector_tile::points_arrays_type>(1.0);
            for (auto const& point_array : geom) {
                for (auto const& point : point_array) {
                    std::clog << point.x << "," << point.y;
                }
            }
            std::clog << "\n";
        }
    }
 
     */
     
    return true;
}



