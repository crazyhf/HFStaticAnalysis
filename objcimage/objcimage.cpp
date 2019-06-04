//
//  objcimage.cpp
//  HFStaticAnalysis
//
//  Created by linhuafeng on 2019/5/21.
//  Copyright © 2019 Haven·Lin. All rights reserved.
//

#include "objcimage.hpp"

using namespace HFAnalysisUtils;

using namespace clang;
using namespace clang::ast_matchers;


void HFObjCImageHandler::run(const MatchFinder::MatchResult &Result)
{
    const ObjCMessageExpr *messageExpr = Result.Nodes.getNodeAs<ObjCMessageExpr>(handlerID());
    
    if (nullptr == messageExpr
        || unneedAnalysis(_analysisConfig,
                          messageExpr->getSourceRange().getBegin())) {
        return;
    }
    
    __unused DiagnosticsEngine &D = _analysisConfig->Diagnostics();
}
