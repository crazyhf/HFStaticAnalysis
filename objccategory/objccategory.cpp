//
//  objccategory.cpp
//  HFStaticAnalysis
//
//  Created by linhuafeng on 2019/5/16.
//  Copyright © 2019 Haven·Lin. All rights reserved.
//

#include "objccategory.hpp"

using namespace HFAnalysisUtils;

using namespace clang;
using namespace clang::ast_matchers;


void HFObjCCategoryHandler::run(const MatchFinder::MatchResult &Result)
{
    const ObjCCategoryDecl *categoryDecl = Result.Nodes.getNodeAs<ObjCCategoryDecl>(handlerID());
    
    if (nullptr == categoryDecl
        || unneedAnalysis(_analysisConfig,
                          categoryDecl->getSourceRange().getBegin())) {
        return;
    }
    
    if (categoryDecl->IsClassExtension()) {
        return;
    }
    
    // StringRef className = categoryDecl->getClassInterface()->getName();
    // StringRef categoryName = categoryDecl->getName();
    
    DiagnosticsEngine &D = _analysisConfig->Diagnostics();
    
    ConstStrVecRef methodPrefixs = _analysisConfig->rule_category_method_prefix();
    
    std::string categoryPrefix = categoryDecl->getName().str();
    if ('_' != categoryPrefix.at(categoryPrefix.size() - 1)) {
        categoryPrefix.append("_");
    }
    
    for (ObjCCategoryDecl::method_iterator ite = categoryDecl->meth_begin(); ite != categoryDecl->meth_end(); ite++) {
        StringRef selName0 = ite->getSelector().getNameForSlot(0);
        if (ite->isPropertyAccessor()) {
            continue;
        }
        bool hasWarning = true;
        for (StrVec::const_iterator ite = methodPrefixs.begin();
             ite != methodPrefixs.end(); ite++) {
            if (selName0.startswith_lower(*ite)) {
                hasWarning = false;
                break;
            }
        }
        if (hasWarning) {
            if (selName0.startswith_lower(categoryPrefix)) {
                return;
            }
            D.Report(ite->getLocation(),
                     D.getCustomDiagID(DiagnosticsEngine::Warning,
                                       HF_DIAGNOSTIC_MSG("category method must has fixed prefix")));
        }
    }
}
