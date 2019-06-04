//
//  property.cpp
//  HFStaticAnalysis
//
//  Created by linhuafeng on 2019/5/1.
//  Copyright © 2019 Haven·Lin. All rights reserved.
//

#include "analysishandler/analysisparser.hpp"
#include "property.hpp"

using namespace HFAnalysisParser;
using namespace HFAnalysisUtils;

using namespace clang;
using namespace clang::ast_matchers;


void HFPropertyHandler::run(const MatchFinder::MatchResult &Result)
{
    const ObjCPropertyDecl *propertyDecl = Result.Nodes.getNodeAs<ObjCPropertyDecl>(handlerID());
    
    if (nullptr == propertyDecl) return;
    
    // if (unneedAnalysis(_analysisConfig,
    //                    propertyDecl->getSourceRange().getBegin())) return;
    //
    // HF_PRINT_SOURCECODE(propertyDecl);
    
    HFAnalysisUnit analysisUnit(_analysisConfig,
                                propertyDecl->getName(),
                                propertyDecl->getSourceRange());
    if (analysisUnit.unneedAnalysis()) return;
    
    analysisUnit.printAnalysisUnit();
    
    if (!analysePropertyType(propertyDecl, analysisUnit)) return;
    
    if (!analysePropertyAttr(propertyDecl, analysisUnit)) return;
}

bool HFPropertyHandler::analysePropertyAttr(const ObjCPropertyDecl *propertyDecl,
                                            HFAnalysisUnit &analysisUnit)
{
    /**
     *  oc obj default attr (OBJC_PR_atomic, OBJC_PR_strong, OBJC_PR_readwrite)
     *  other type default attr (OBJC_PR_atomic, OBJC_PR_assign, OBJC_PR_readwrite)
     */
    ObjCPropertyDecl::PropertyAttributeKind attrKind = propertyDecl->getPropertyAttributes();
    
    DiagnosticsEngine &D = _analysisConfig->Diagnostics();
    
    const SourceLocation &lParenLoc = propertyDecl->getLParenLoc();
    if (!lParenLoc.isValid()) {
        const SourceLocation &atLoc = propertyDecl->getAtLoc();
        std::string attrCode = std::string(" (nonatomic, ").append(propertyMemAttrStr(attrKind)).append(")");
        std::cout << HF_ANALYSIS_TAG_WARN
                  << " must set property attributes"
                  << attrCode << std::endl;
        D.Report(propertyDecl->getLocation(),
                 D.getCustomDiagID(DiagnosticsEngine::Warning,
                                   HF_DIAGNOSTIC_MSG("must set property attributes")))
        .AddFixItHint(FixItHint::CreateInsertion(atLoc.getLocWithOffset(9), attrCode));
        return false;
    }
    
    analysisUnit.printPropertyAttr(propertyDecl);
    
    std::vector<HFAnalysisToken> tokenVec = parsePropertyAttrRule("");
    
    return true;
}

bool HFPropertyHandler::analysePropertyType(const ObjCPropertyDecl *propertyDecl,
                                            HFAnalysisUnit &analysisUnit)
{
    return analyseDeclType(propertyDecl->getTypeSourceInfo(), analysisUnit);
}
