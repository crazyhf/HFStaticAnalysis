//
//  analysisconfig.cpp
//  HFStaticAnalysis
//
//  Created by linhuafeng on 2019/5/22.
//  Copyright © 2019 Haven·Lin. All rights reserved.
//

#include "llvm/Support/JSON.h"

#include "analysisconfig.hpp"

using namespace llvm;
using namespace clang;


HFAnalysisConfig::HFAnalysisConfig(const CompilerInstance &CI,
                                   StringRef workpath,
                                   StringRef configFile,
                                   bool enableDebugTrace)
                                  : _CI(CI),
                                    _workpath(workpath.str()),
                                    _enableDebugTrace(enableDebugTrace)
{
    std::string pathPrefix = _workpath;
    if (workpath.endswith("/")) {
        _workpath.erase(_workpath.length() - 1, 1);
    } else {
        pathPrefix.append("/");
    }
    
    FILE *pFile = fopen(configFile.data(), "r");
    
    if (nullptr == pFile) {
        std::cout << HF_ANALYSIS_TAG_ERROR
                  << " cannot open file "
                  << configFile.str() << std::endl;
        return;
    }
    
    fseek(pFile, 0, SEEK_END);
    long lSize = ftell(pFile);
    rewind(pFile);
    
    char *buffer = new char[lSize];
    if (buffer) {
        long result = fread(buffer, sizeof(char), lSize, pFile);
        if (result != lSize) {
            std::cout << HF_ANALYSIS_TAG_WARN
                      << " " << result
                      << " datas are read, but total size is "
                      << lSize << std::endl;
        }
        Expected<json::Value> jsonVal = json::parse(StringRef(buffer, result));
        if (jsonVal && json::Value::Object == jsonVal->kind()) {
            if (const auto *jsonObj = jsonVal->getAsObject()) {
                if (auto enableDebugTrace = jsonObj->getBoolean("enableDebugTrace")) {
                    _enableDebugTrace = enableDebugTrace.getValue();
                }
                if (const auto *ignoreFileTypes = jsonObj->getArray("ignoreFileTypes")) {
                    for (json::Array::const_iterator ite = ignoreFileTypes->begin();
                         ite != ignoreFileTypes->end(); ite++) {
                        if (const auto fileType = ite->getAsString()) {
                            _ignoreFileTypes.push_back((*fileType).str().insert(0, "."));
                        }
                    }
                }
                if (const auto *ignoreDirectories = jsonObj->getArray("ignoreDirectories")) {
                    for (json::Array::const_iterator ite = ignoreDirectories->begin();
                         ite != ignoreDirectories->end(); ite++) {
                        if (const auto directory = ite->getAsString()) {
                            _ignoreDirectories.push_back((*directory).str().insert(0, pathPrefix));
                        }
                    }
                }
                if (const auto *category_method_prefix = jsonObj->getArray("@category_method_prefix")) {
                    for (json::Array::const_iterator ite = category_method_prefix->begin();
                         ite != category_method_prefix->end(); ite++) {
                        if (const auto methodPrefix = ite->getAsString()) {
                            _rule_category_method_prefix.push_back((*methodPrefix).str().append("_"));
                        }
                    }
                }
            }
        } else {
            std::cout << HF_ANALYSIS_TAG_ERROR
                      << " parse configFile failed "
                      << configFile.str() << std::endl;
        }
        delete [] buffer;
    }
    
    fclose(pFile);
}

