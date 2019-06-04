//
//  objccategory.hpp
//  HFStaticAnalysis
//
//  Created by linhuafeng on 2019/5/16.
//  Copyright © 2019 Haven·Lin. All rights reserved.
//

#ifndef HF_STATICANALYSIS_OBJCCATEGORY_HPP
#define HF_STATICANALYSIS_OBJCCATEGORY_HPP

#include "analysishandler/analysishandler.hpp"


class HFObjCCategoryHandler : public HFAnalysisHandler<clang::Decl>
{
public:
    HFObjCCategoryHandler(HFAnalysisConfigPtr &analysisConfig)
                         : HFAnalysisHandler(analysisConfig) {}
    
    virtual std::string handlerID() {
        return "objcCategoryDecl";
    }
    
    virtual const clang::ast_matchers::DeclarationMatcher handlerMatcher() {
        return clang::ast_matchers::objcCategoryDecl().bind(handlerID());
    }
    
    virtual void run(const clang::ast_matchers::MatchFinder::MatchResult &Result);
};

#endif /* objccategory_hpp */

