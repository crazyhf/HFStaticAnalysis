//
//  analysisconfig.hpp
//  HFStaticAnalysis
//
//  Created by linhuafeng on 2019/5/22.
//  Copyright © 2019 Haven·Lin. All rights reserved.
//

#ifndef HF_STATICANALYSIS_ANALYSISCONFIG_HPP
#define HF_STATICANALYSIS_ANALYSISCONFIG_HPP

#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Frontend/CompilerInstance.h"

#include "llvm/Support/raw_ostream.h"

#include <iostream>

#define HF_ANALYSIS_TAG             "[HFStaticAnalysis]"

#define HF_ANALYSIS_TAG_WARN        "[HFStaticAnalysis-WARN]"

#define HF_ANALYSIS_TAG_ERROR       "[HFStaticAnalysis-ERROR]"

#define HF_ANALYSIS_TAG_TRACE       "[HFStaticAnalysis-TRACE]"

#define HF_ANALYSIS_TAGEX(_module_) "[HFStaticAnalysis-" _module_ "]"

#define HF_DIAGNOSTIC_MSG(_msg_)    HF_ANALYSIS_TAG " " _msg_


class HFAnalysisConfig
{
protected:
    const clang::CompilerInstance &_CI;
    
    std::string _workpath;
    
    bool _enableDebugTrace;
    
    std::vector<std::string> _ignoreFileTypes;
    
    std::vector<std::string> _ignoreDirectories;
    
    
    std::vector<std::string> _rule_category_method_prefix;
    
public:
    HFAnalysisConfig(const clang::CompilerInstance &CI,
                     llvm::StringRef workpath,
                     llvm::StringRef configFile,
                     bool enableDebugTrace = false);
    
    const clang::CompilerInstance &CI() { return _CI; }
    
    clang::DiagnosticsEngine &Diagnostics() { return _CI.getDiagnostics(); }
    
    
    const std::string &workpath() { return _workpath; }
    
    bool enableDebugTrace() { return _enableDebugTrace; }
    
    const std::vector<std::string> &ignoreFileTypes() { return _ignoreFileTypes; }
    
    const std::vector<std::string> &ignoreDirectories() { return _ignoreDirectories; }
    
    
    const std::vector<std::string> &rule_category_method_prefix() { return _rule_category_method_prefix; }
};

typedef std::shared_ptr<HFAnalysisConfig> HFAnalysisConfigPtr;

#endif /* analysisconfig_hpp */

