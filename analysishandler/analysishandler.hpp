//
//  analysishandler.hpp
//  HFStaticAnalysis
//
//  Created by linhuafeng on 2019/5/5.
//  Copyright © 2019 Haven·Lin. All rights reserved.
//

#ifndef HF_STATICANALYSIS_ANALYSISHANDLER_HPP
#define HF_STATICANALYSIS_ANALYSISHANDLER_HPP

#include "analysistoken.hpp"


template <typename T>
class HFAnalysisHandler : public clang::ast_matchers::MatchFinder::MatchCallback
{
protected:
    HFAnalysisConfigPtr _analysisConfig;
    
public:
    HFAnalysisHandler(HFAnalysisConfigPtr &analysisConfig) : _analysisConfig(analysisConfig) {}
    
    virtual std::string handlerID() = 0;
    
    virtual const clang::ast_matchers::internal::Matcher<T> handlerMatcher() = 0;
};

#define HF_PRINT_SOURCECODE(_ast_node_) \
        HFAnalysisUtils::printSourceCode(_analysisConfig->CI(), \
                                         _ast_node_->getSourceRange());

#endif /* analysishandler_hpp */
