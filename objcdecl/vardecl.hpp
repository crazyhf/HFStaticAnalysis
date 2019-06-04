//
//  vardecl.hpp
//  HFStaticAnalysis
//
//  Created by linhuafeng on 2019/5/27.
//  Copyright © 2019 Haven·Lin. All rights reserved.
//

#ifndef HF_STATICANALYSIS_VARDECL_HPP
#define HF_STATICANALYSIS_VARDECL_HPP

#include "analysishandler/declanalysishandler.hpp"


class HFVarDeclHandler : public HFDeclAnalysisHandler
{
private:
    bool analyseVarDeclType(const clang::VarDecl *varDecl,
                            HFAnalysisUtils::HFAnalysisUnit &analysisUnit);
    
public:
    HFVarDeclHandler(HFAnalysisConfigPtr &analysisConfig)
                    : HFDeclAnalysisHandler(analysisConfig) {}
    
    virtual std::string handlerID() {
        return "varDecl";
    }
    
    virtual const clang::ast_matchers::DeclarationMatcher handlerMatcher() {
        return clang::ast_matchers::varDecl().bind(handlerID());
    }
    
    virtual void run(const clang::ast_matchers::MatchFinder::MatchResult &Result);
};

#endif /* vardecl_hpp */
