//
//  TileDB.cpp
//  of9
//
//  Created by Esteban on 10/06/23.
//

#include "TileDataBase.hpp"



TileDataBase::TileDataBase(){
}

void TileDataBase::getInfo(){
    cout << "name:" << reader.GetMetadata("name") << endl;
    cout << "type:" << reader.GetMetadata("type") << endl;
    string version = reader.GetMetadata("version");
    cout << "version:" << version << endl;
    vector<string> versionSplit;
    //strsplit(version, '.', versionSplit);
    vector<int> versionInts;
    for (size_t i = 0; i < versionSplit.size(); i++)
        versionInts.push_back(atoi(versionSplit[i].c_str()));
    cout << "description:" << reader.GetMetadata("description") << endl;
    string format = reader.GetMetadata("format");
    cout << "format:" << format << endl;
    cout << "bounds:" << reader.GetMetadata("bounds") << endl;

    if (1) // Get metadata fields
    {
        std::vector<std::string> fieldNames;
        reader.GetMetadataFields(fieldNames);
        for (unsigned i = 0; i < fieldNames.size(); i++)
            cout << fieldNames[i] << endl;
    }
    
}

TileDataBase::~TileDataBase(){
}
