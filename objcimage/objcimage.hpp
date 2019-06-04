//
//  objcimage.hpp
//  HFStaticAnalysis
//
//  Created by linhuafeng on 2019/5/21.
//  Copyright © 2019 Haven·Lin. All rights reserved.
//

#ifndef HF_STATICANALYSIS_OBJCIMAGE_HPP
#define HF_STATICANALYSIS_OBJCIMAGE_HPP

#include "analysishandler/analysishandler.hpp"


class HFObjCImageHandler : public HFAnalysisHandler<clang::Stmt>
{
public:
    HFObjCImageHandler(HFAnalysisConfigPtr &analysisConfig)
                      : HFAnalysisHandler(analysisConfig) {}
    
    virtual std::string handlerID() {
        return "objcMessageExpr";
    }
    
    virtual const clang::ast_matchers::StatementMatcher handlerMatcher() {
        return clang::ast_matchers::objcMessageExpr().bind(handlerID());
    }
    
    virtual void run(const clang::ast_matchers::MatchFinder::MatchResult &Result);
};

#endif /* objcimage_hpp */
