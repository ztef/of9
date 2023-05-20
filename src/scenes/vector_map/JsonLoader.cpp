//
//  JsonLoader.cpp
//  vectorTileExperiment
//
//  Created by Matt Blair on 6/26/14.
//
//

#include "JsonLoader.h"
#include <math.h>



JsonLoader::JsonLoader() {
    tessellator = ofTessellator();
    cout << "jsonLoader Creado";
}

FeatureNode* JsonLoader::loadTile(std::string fileName, Projection* _p) {
    
    projection = _p;
    
    if (jsonRoot.open(fileName)) {
        ofLog(OF_LOG_VERBOSE, "Json parsed successfully");
    } else {
        ofLog(OF_LOG_VERBOSE, "Json could not be parsed!");
    }
    return loadNodeGraph();
}

JsonLoader::JsonLoader(std::string fileName) {
    
    if (jsonRoot.open(fileName)) {
        ofLog(OF_LOG_VERBOSE, "Json parsed successfully");
    } else {
        ofLog(OF_LOG_VERBOSE, "Json could not be parsed!");
    }
    
    tessellator = ofTessellator();
    
}

FeatureNode* JsonLoader::loadNodeGraph() {
    
    vector<FeatureNode*> layers;
    
    ofLog(OF_LOG_VERBOSE, "Loading Layers:");
    
    std::string layerNames[5] = {
         
        "earth",
        "water",
        "roads",
        "buildings",
        "pois"
    };
    
    ofColor layerColors[5] = {
        
        
        ofFloatColor::lawnGreen,
        ofFloatColor::deepSkyBlue,
        ofFloatColor::maroon,
        ofFloatColor::darkGrey,
        ofFloatColor::white,
    };
    
    float layerHeights[5] = {
        
        0.0f,
        0.00001f,
        0.0f,
        0.0f,
        0.000f
    };
    
    for (int i = 0; i < 5; i++) {
        if(!jsonRoot[layerNames[i]].empty()){
            layerColor = layerColors[i];
            FeatureCollectionNode* newLayer = parseFeatureCollectionNode(jsonRoot[layerNames[i]]);

            if(newLayer->children.size()>0){

                newLayer->move(0, 0, layerHeights[i]);
                newLayer->layerColor = layerColor;
                newLayer->layerName = layerNames[i];
                layers.push_back(newLayer);
                cout << layerNames[i] + " layer parsed with " + ofToString(newLayer->children.size()) + " children";
            
            }
        }
    }
    
    rootNode = new FeatureCollectionNode(layers);
    
    return rootNode;
}

FeatureCollectionNode* JsonLoader::getCollection(){
    return rootNode;
}

FeatureNode* JsonLoader::parseNode(ofxJSONElement nodeJson) {
    
    std::string nodeType = nodeJson["type"].asString();
    
    if (nodeType.compare("Feature") == 0) {
        return parseFeatureNode(nodeJson);
    } else if (nodeType.compare("FeatureCollection") == 0) {
        return parseFeatureCollectionNode(nodeJson);
    } else {
        ofLog(OF_LOG_WARNING, "Couldn't match Feature type; regresando nodo vacio");
        return new FeatureNode();
    }
    
}

FeatureCollectionNode* JsonLoader::parseFeatureCollectionNode(ofxJSONElement collectionJson) {
    
    ofxJSONElement features = collectionJson["features"];
    
    vector<FeatureNode*> featureNodes;
    
    for (int i = 0; i < features.size(); i++) {
        featureNodes.push_back(parseNode(features[i]));
    }
    
    FeatureCollectionNode* newFeatureCollection = new FeatureCollectionNode(featureNodes);
    
    // aqui
    newFeatureCollection->layerColor =ofFloatColor::deepSkyBlue;
    
    ofLog(OF_LOG_VERBOSE, "Feature collection of " + ofToString(features.size()) + " features parsed with position " +
          ofToString(newFeatureCollection->getX()) + ", " +
          ofToString(newFeatureCollection->getY()));
    
    return newFeatureCollection;
    
}

FeatureNode* JsonLoader::parseFeatureNode(ofxJSONElement featureJson) {
    
    ofxJSONElement coords = featureJson["geometry"]["coordinates"];
    ofxJSONElement props = featureJson["properties"];
    
    ofMesh newMesh = ofMesh();
    
    glm::vec3 anchor;
    
    std::string type = featureJson["geometry"]["type"].asString();
    
    if (type.compare("Point") == 0) {
        
        anchor = parsePointInProjectedCoords(coords);
       
        newMesh.addVertex(glm::vec3(0, 0, 0));
        //newMesh.addVertex(anchor);
        
        //ESTE ES EL BUENO :
        //newMesh = ofMesh::box(0.0001, 0.0001, 0.01);
        
        /*
         for(auto & v : newMesh.getVertices()) {
            //v = glm::vec4(v, 1).xyz();
            v = glm::vec4(v,1);
        }
         */
        
        newMesh.setMode(OF_PRIMITIVE_POINTS);
        newMesh.setupIndicesAuto();
        
    } else if (type.compare("MultiPoint") == 0) {
        
        vector<glm::vec3> verts = parsePointArrayInProjectedCoords(coords);
       

        anchor = getCentroidFromPoints(verts);
        
        //movePoints(&verts, -(anchor));
        


        newMesh.setMode(OF_PRIMITIVE_POINTS);
        newMesh.addVertices(verts);
        newMesh.setupIndicesAuto();
        
    } else if (type.compare("LineString") == 0) {
        
        parseLineGeometry(coords, props, &newMesh, &anchor);
        anchor = newMesh.getCentroid();



        //std::vector<glm::vec3>  vecs =  newMesh.getVertices();
        //movePoints(&vecs, ofVec3f(-(anchor.x), -(anchor.y), 0));
        
    } else if (type.compare("MultiLineString") == 0) {
        
        for (int i = 0; i < coords.size(); i++) {
            parseLineGeometry(coords[i], props, &newMesh, &anchor);
        }
        anchor = newMesh.getCentroid();
        
        
    } else if (type.compare("Polygon") == 0) {
        
        newMesh.setMode(OF_PRIMITIVE_TRIANGLES);
        parsePolygonGeometry(coords, props, &newMesh, &anchor, false);
        
        ofLog(OF_LOG_VERBOSE, "Polygon mesh with vertices: " + ofToString(newMesh.getVertices()));
        
    } else if (type.compare("MultiPolygon") == 0) {
        
        newMesh.setMode(OF_PRIMITIVE_TRIANGLES);
        int cs = coords.size();
        for (int i = 0; i < cs; i++) {
            ofMesh subMesh = ofMesh();
            subMesh.setMode(OF_PRIMITIVE_TRIANGLES);
            parsePolygonGeometry(coords[i], props, &subMesh, &anchor, true);
            newMesh.append(subMesh);
        }
        anchor = newMesh.getCentroid();

        //ofVec3f * vecs = (ofVec3f*) newMesh.getVerticesPointer();
        //std::vector<glm::vec3>  vecs =  newMesh.getVertices();
        //movePoints(&vecs, ofVec3f(-(anchor.x), -(anchor.y), 0));
        
    } else if (type.compare("GeometryCollection") == 0) {
        
        // Ignoring this for now
        
    } else {
        
        ofLog(OF_LOG_WARNING, "Couldn't match geometry type; returning empty mesh");
        
    }
     
    

    newMesh.setColorForIndices(0, newMesh.getNumIndices(), layerColor);
    
    FeatureLeafNode* newNode = new FeatureLeafNode(newMesh, type);
    newNode->idString = featureJson["id"].asString();
   // newNode->setPosition(anchor);
      newNode->anchor = anchor;
      newNode->setPosition(0,0,0);
    

    ofLog(OF_LOG_VERBOSE, "Parsed geometry type: " + type + ", with centroid: " + ofToString(anchor));
    
    return newNode;
    
}


glm::vec3 JsonLoader::parsePointInProjectedCoords(ofxJSONElement pointJson) {
    
    //double x = lon2x(pointJson[0].asFloat());
    //double y = lat2y(pointJson[1].asFloat());
    
    //double x = getXX(pointJson[0].asFloat(),1024);
    //double y = getYY(pointJson[1].asFloat(),768, 1024);
    
    //mercatortile::XY rxy = mercatortile::xy(pointJson[0].asDouble(), pointJson[1].asDouble());
    
    //double x = rxy.x * 0.0001;
    //double y = rxy.y * 0.0001;
    
    //double a = -115.0;
    //double b = 61.97;
    //mercatortile::XY m = mercatortile::xy(b,a);
    
    Coordinate coord;
    
    coord.longitude = pointJson[0].asDouble();
    coord.latitude = pointJson[1].asDouble();
    
    
    ofPoint p  = projection->getProjection(coord);
    //Coordinate d = projection->getCoordinate(p);
    
    
    
   // cout << "X " << m.x << ", Y " << m.y << " " << endl;
    
    return glm::vec3(p);
    
}








vector<glm::vec3> JsonLoader::parsePointArrayInProjectedCoords(ofxJSONElement pointArrayJson) {
    
    vector<glm::vec3> pts;
    
    for (int i = 0; i < pointArrayJson.size(); i++) {
        pts.push_back(parsePointInProjectedCoords(pointArrayJson[i]));
    }
    
    return pts;
    
}

void JsonLoader::parseLineGeometry(ofxJSONElement lineJson, ofxJSONElement propsJson, ofMesh* meshToFill, glm::vec3* anchor) {
    
    vector<glm::vec3> verts = parsePointArrayInProjectedCoords(lineJson);
    
    //*anchor = getCentroidFromPoints(verts);
    
    //movePoints(&verts, -(*anchor));
    
    meshToFill->setMode(OF_PRIMITIVE_LINE_STRIP);
    meshToFill->addVertices(verts);
    meshToFill->setupIndicesAuto();
    
    for (int i = 0; i < meshToFill->getNumVertices(); i++) {
        meshToFill->addNormal(glm::vec3(0, 0, 1));
    }
    
}

void JsonLoader::parsePolygonGeometry(ofxJSONElement polygonJson, ofxJSONElement propsJson, ofMesh *meshToFill, glm::vec3 *anchor, bool multi) {
    
    float height = 0;
    float minHeight = 0;
    float heightFactor = 0.00001;
    string kind = "";
    
    
    if (propsJson["height"] != Json::nullValue) {
        height = propsJson["height"].asFloat();
        height = height * heightFactor;
    }
    
    if (propsJson["min_height"] != Json::nullValue) {
        minHeight = propsJson["min_height"].asFloat();
        minHeight =  0.0;
    }
    
    if (propsJson["kind"] != Json::nullValue) {
        kind = propsJson["kind"].asString();
    }
    
    vector<ofPolyline> polyLines;
    
    int ps = polygonJson.size();
   

    for (int i = 0; i < ps; i++) {
        
        vector<glm::vec3> verts = parsePointArrayInProjectedCoords(polygonJson[i]);
        
        if (i == 0) {
            *anchor = getCentroidFromPoints(verts);
        }
        
        // kind != "earth"
        if(!multi){
           // movePoints(&verts, ofVec3f(-(anchor->x), -(anchor->y), height));
        } else {
             
         // movePoints(&verts, ofVec3f(-(anchor->x), -(anchor->y), height));
            
        }
    
        
        polyLines.push_back(verts);
        
    }
    
     
        tessellator.tessellateToMesh(polyLines, OF_POLY_WINDING_ODD, *meshToFill, false);
      
    
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

void JsonLoader::movePoints(vector<glm::vec3> *pts, glm::vec3 offset) {
    
    for (int i = 0; i < pts->size(); i++) {
      (*pts)[i] += offset;
    }
}

ofVec3f JsonLoader::getCentroidFromPoints(vector<glm::vec3> pts) {
    glm::vec3 centroid;
    
    for (int i = 0; i < pts.size(); i++) {
        centroid += pts[i] / pts.size();
    }
    
    return centroid;
}


