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

FeatureNode* MVTLoader::loadTile(std::string fileName, Projection* _p, tilefunctions::Tile pos){
    projection = _p;
    position = pos;
    
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
    
    std::string layerNames[18] = {
         
        //"administrative",
        "earth",
        "water",
        "waterway",
        "boundary",
        "boundaries",
        "globallandcover",
        "park",
        "landuse",
        "place",
        "places",
        "roads",
        "transportation",
        "transit",
        "pois",
        "poi",
        "buildings",
        "building",
        "housenumber"
    };
    ofColor layerColors[18] = {
        ofFloatColor::lawnGreen,   // earth
        ofFloatColor::deepSkyBlue,  //water
        ofFloatColor::blueSteel,    //waterway
        ofFloatColor::white,        //boundary
        ofFloatColor::white,        //boundaries
        ofFloatColor::lawnGreen,  //globallandcover
        ofFloatColor::forestGreen,  //park
        ofFloatColor::maroon,       //landuse
        ofFloatColor::red,          //place
        ofFloatColor::red,          //places
        ofFloatColor::orange,       //roads
        ofFloatColor::grey,          //transportation
        ofFloatColor::red,          //transit
         ofFloatColor::red,         //pois
         ofFloatColor::red,         //poi
         ofFloatColor::grey,        //buildings
         ofFloatColor::grey,        //building
         ofFloatColor::red         //housenumber
    
    };
    
    float layerHeights[18] = {
        
        0.00f,
        0.03f,
        0.06f,
        0.09f,
        0.12f,
        0.15f,
        0.18f,
        0.20f,
        0.22f,
         0.24f,
         0.26f,
         0.28f,
         0.30f,
         1.33f,
         1.35f,
         1.39f,
         1.42f,
         1.47f
    };
    
    for (int i = 0; i < 9; i++) {
        
        if(tile->isLayer(layerNames[i])){
            if(i==8){
                cout << "BUILDINGS";
            }
            
            if((i == 0) && (position.z > 3)){
             //   continue;
            }
            if((i == 1) && (position.z > 3)){
             //   continue;
            }
            if(i == 2){
                cout << "Landuse";
            }

            
            layerColor = layerColors[i];
            layerHeight = layerHeights[i];
            
            FeatureCollectionNode* newLayer = parseFeatureCollectionNode(tile->getLayer(layerNames[i]));

            if(newLayer->children.size()>0){

                //newLayer->move(0, 0, layerHeights[i]);
                newLayer->layerColor = layerColor;
                newLayer->layerName = layerNames[i];
                layers.push_back(newLayer);
                
            }
        }
    }
    
    rootNode = new FeatureCollectionNode(layers);
    
    ofVec3f global_anchor;
    int global_children = rootNode->children.size();
    
    for(FeatureNode* nodo:rootNode->children){
        if(nodo->nodeType == "collection"){
            FeatureCollectionNode* fcn = (FeatureCollectionNode*)nodo;
            layerColor = fcn->layerColor;
            ofVec3f anchor;
            int childrenSize = fcn->children.size();
            for(auto childx : fcn->children){
                FeatureLeafNode* leaf = (FeatureLeafNode*)childx;
                leaf->zoom = position.z; // debe estar en el padre
                if(leaf->nodeType == "Point"){
                    leaf->anchor = parsePointInProjectedCoords(leaf->verts.at(0).getVertices().at(0).x, leaf->verts.at(0).getVertices().at(0).y);
                                 
                                  leaf->geometry.addVertex(glm::vec3(0, 0, 0));
                                  leaf->geometry.setMode(OF_PRIMITIVE_POINTS);
                                  leaf->geometry.setupIndicesAuto();
                    
                } else if(leaf->nodeType == "Line"){
                    
                    leaf->geometry.setMode(OF_PRIMITIVE_LINE_STRIP);

                    
                         for(auto poly : leaf->verts){
                             for(auto p: poly){
                                 leaf->geometry.addColor(layerColor);
                                 leaf->geometry.addVertex(p);
                             }
                         }
                         
                         leaf->geometry.setupIndicesAuto();
                    
                            for (int i = 0; i < leaf->geometry.getNumVertices(); i++) {
                                leaf->geometry.addNormal(glm::vec3(0, 0, 1));
                            }
                        
                         leaf->anchor = leaf->geometry.getCentroid();
                }
                 else if (leaf->nodeType == "Polygon"){
                        leaf->geometry.setMode(OF_PRIMITIVE_TRIANGLES);
                        ofMesh* m = new ofMesh();
                        tessellator->tessellateToMesh(leaf->verts, OF_POLY_WINDING_ODD, *m, false);
                        
                    
                    
                        leaf->anchor = leaf->verts.at(0).getCentroid2D();
                     
                        leaf->geometry.append(*m);
                       
                       // Add normals for the top surface
                     for (int i = 0; i < leaf->geometry.getNumVertices(); i++) {
                         leaf->geometry.addNormal(ofVec3f(0, 0, 1));
                       }
                     
                } else {
                    cout << "error" <<endl;
                }
                leaf->geometry.setColorForIndices(0, leaf->geometry.getNumIndices(), layerColor);
                leaf->setPosition(leaf->geometry.getCentroid());
            
                anchor += leaf->anchor / childrenSize;
            }
            nodo->anchor = anchor;
            nodo->setPosition(nodo->anchor);
            
            global_anchor += nodo->anchor / global_children;
        }
    }
    
    rootNode->anchor = global_anchor;
    rootNode->setGlobalPosition(rootNode->anchor);
    
    return rootNode;
}


FeatureCollectionNode* MVTLoader::parseFeatureCollectionNode(mapbox::vector_tile::layer  layer) {
    
    
    std::size_t feature_count = layer.featureCount();
    auto extent = layer.getExtent();
    auto version = layer.getVersion();
    
    projection->setExtent(extent);
    
    vector<FeatureNode*> featureNodes;
    
    for (int i = 0; i < feature_count; i++) {
        auto const feature = mapbox::vector_tile::feature(layer.getFeature(i), layer);
        featureNodes.push_back(parseNode(feature));
        
    }
    
    FeatureCollectionNode* newFeatureCollection = new FeatureCollectionNode(featureNodes);
    newFeatureCollection->nodeType = "collection";
    
    // aqui
    newFeatureCollection->layerColor =ofFloatColor::deepSkyBlue;
    
    
    return newFeatureCollection;
    
}
    

FeatureNode* MVTLoader::parseNode(mapbox::vector_tile::feature feature) {
    
   auto const& feature_id = feature.getID();
    
    mapbox::vector_tile::GeomType nodeType = feature.getType();
      
    auto props = feature.getProperties();
   // string kind = props.
    for (auto const& prop : props) {
        print_value printvisitor;
        std::string value = mapbox::util::apply_visitor(printvisitor, prop.second);
        std::cout << "      " << prop.first  << ": " << value << "\n";
    }
    
    mapbox::vector_tile::points_arrays_type geom = feature.getGeometries<mapbox::vector_tile::points_arrays_type>(1.0);
    
    return parseFeatureNode(feature);
    
    
}

FeatureNode* MVTLoader::parseFeatureNode(mapbox::vector_tile::feature feature) {
    
   
    mapbox::vector_tile::points_arrays_type geom = feature.getGeometries<mapbox::vector_tile::points_arrays_type>(1.0);
    auto props = feature.getProperties();
    
    
    vector<ofPolyline> verts;
    ofMesh newMesh;
    
    glm::vec3 anchor;
    
   
    mapbox::vector_tile::GeomType type = feature.getType();
    
    string typeString = "";
    
    if(type == mapbox::vector_tile::POINT) {
        
        typeString = "Point";
        ofPolyline p;
        p.addVertex(geom[0][0].x,geom[0][0].y,0);
        verts.push_back(p);
        
    } else if (type == mapbox::vector_tile::LINESTRING){
        
        typeString = "Line";
        
        parseLineGeometry(geom, props, &verts, &newMesh, &anchor);
       //anchor = newMesh.getCentroid();
        
        
    } else if (type == mapbox::vector_tile::POLYGON){
            typeString = "Polygon";
          
            parsePolygonGeometry(geom, props, &verts, &newMesh, &anchor, false);
        
    } else {
        
    }
    
   
    
    FeatureLeafNode* newNode = new FeatureLeafNode(verts,  typeString);
    newNode->nodeType = typeString;
    //AQUI
    //newNode->anchor = anchor;
    
   
    
    return newNode;
    
}

void MVTLoader::parsePolygonGeometry(mapbox::vector_tile::points_arrays_type geom, mapbox::vector_tile::feature::properties_type props,vector<ofPolyline>* _polyLines, ofMesh *meshToFill, glm::vec3 *anchor, bool multi) {
    
    float height = 0;
    float minHeight = 0;
    float heightFactor = 0.00001;
    string kind = "";
    
    
     int i=0;
     for (auto const& point_array : geom) {
          
    
             vector<glm::vec3> verts = parsePointArrayInProjectedCoords(point_array);
           //  if (i == 0) {
            //     *anchor = getCentroidFromPoints(verts);
           //  }
         
        
         _polyLines->push_back(verts);
         
         i++;
     }
    
     // Manual essellate the shape
      
     /*
      for (const auto& polyline : polyLines) {
         const vector<glm::vec3>& vertices = polyline.getVertices();
         const int numVertices = vertices.size();

         // Create triangles from the vertices
         for (int i = 1; i < numVertices - 1; ++i) {
             meshToFill->addVertex(vertices[0]);
             meshToFill->addVertex(vertices[i]);
             meshToFill->addVertex(vertices[i + 1]);
             
             meshToFill->addColor(ofColor(layerColor));
             meshToFill->addColor(ofColor(layerColor));
             meshToFill->addColor(ofColor(layerColor));
         }
     }
      */
     
   
   

}

ofPath MVTLoader::polysToPath(vector<ofPolyline> polylines) {
  ofPath path;
  for(int outline = 0; outline < polylines.size(); ++outline) {
        for (int i = 0; i < polylines[outline].getVertices().size(); i++){
            if ( i == 0 )
                path.moveTo(polylines[outline].getVertices()[i].x,polylines[outline].getVertices()[i].y);
            else
                path.lineTo(polylines[outline].getVertices()[i].x,polylines[outline].getVertices()[i].y);
        }
        path.close();
    }
    return path;
}

void MVTLoader::parseLineGeometry(mapbox::vector_tile::points_arrays_type geom, mapbox::vector_tile::feature::properties_type props, vector<ofPolyline>* vertsToFill, ofMesh* meshToFill, glm::vec3* anchor) {
    

     int i=0;
     for (auto const& point_array : geom) {
             vector<glm::vec3> verts = parsePointArrayInProjectedCoords(point_array);
          //   if (i == 0) {
          //       *anchor = getCentroidFromPoints(verts);
          //   }
            
             
         for(auto const& vert : verts){
             ofPolyline p;
             p.addVertex(vert);
             vertsToFill->push_back(p);
         }
         
         
            
         i++;
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
    
    
    ofPoint p  = projection->getProjection(coord, position);
     
    
    
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
    tile = new mapbox::vector_tile::buffer();
    buffer = ofLoadURL(filename).data.getText();
    tile->setBuffer(buffer);
    tile->parse();
      
    return true;
}

bool MVTLoader::openLocal(const std::string& filename)
{
    tile = new mapbox::vector_tile::buffer();
    buffer = open_tile(filename);
    tile->setBuffer(buffer);
    tile->parse();

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



