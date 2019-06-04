//
//  property.hpp
//  HFStaticAnalysis
//
//  Created by linhuafeng on 2019/5/1.
//  Copyright © 2019 Haven·Lin. All rights reserved.
//

#ifndef HF_STATICANALYSIS_PROPERTY_HPP
#define HF_STATICANALYSIS_PROPERTY_HPP

#include "analysishandler/declanalysishandler.hpp"


class HFPropertyHandler : public HFDeclAnalysisHandler
{
private:
    bool analysePropertyAttr(const clang::ObjCPropertyDecl *propertyDecl,
                             HFAnalysisUtils::HFAnalysisUnit &analysisUnit);
    
    bool analysePropertyType(const clang::ObjCPropertyDecl *propertyDecl,
                             HFAnalysisUtils::HFAnalysisUnit &analysisUnit);
    
public:
    HFPropertyHandler(HFAnalysisConfigPtr &analysisConfig)
                     : HFDeclAnalysisHandler(analysisConfig) {}
    
    virtual std::string handlerID() {
        return "objcPropertyDecl";
    }
    
    virtual const clang::ast_matchers::DeclarationMatcher handlerMatcher() {
        return clang::ast_matchers::objcPropertyDecl().bind(handlerID());
    }
    
    virtual void run(const clang::ast_matchers::MatchFinder::MatchResult &Result);
};

#endif /* property_hpp */
