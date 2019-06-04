//
//  vardecl.cpp
//  HFStaticAnalysis
//
//  Created by linhuafeng on 2019/5/27.
//  Copyright © 2019 Haven·Lin. All rights reserved.
//

#include "vardecl.hpp"

using namespace HFAnalysisUtils;

using namespace clang;
using namespace clang::ast_matchers;


void HFVarDeclHandler::run(const MatchFinder::MatchResult &Result)
{
    const VarDecl *varDecl = Result.Nodes.getNodeAs<VarDecl>(handlerID());
    
    if (nullptr == varDecl) return;
    
    // if (unneedAnalysis(_analysisConfig,
    //                    varDecl->getSourceRange().getBegin())) return;
    //
    // HF_PRINT_SOURCECODE(varDecl);
    
    if (varDecl->hasInit()) {
        const Expr *expr = varDecl->getInit();
        if (Stmt::ImplicitCastExprClass == expr->getStmtClass()) {
            const ImplicitCastExpr *castExpr = (const ImplicitCastExpr *)expr;
            if (clang::CK_NullToPointer == castExpr->getCastKind()) {
                ;
            }
        }
    }
    
    if (Decl::ParmVar == varDecl->getKind()) {
        __unused const ParmVarDecl *parmVarDecl = (const ParmVarDecl *)varDecl;
    }
    
    HFAnalysisUnit analysisUnit(_analysisConfig,
                                varDecl->getName(),
                                varDecl->DeclaratorDecl::getSourceRange());
    if (analysisUnit.unneedAnalysis()) return;
    
    analysisUnit.printAnalysisUnit();
    
    analyseVarDeclType(varDecl, analysisUnit);
}

bool HFVarDeclHandler::analyseVarDeclType(const VarDecl *varDecl,
                                          HFAnalysisUnit &analysisUnit)
{
    return analyseDeclType(varDecl->getTypeSourceInfo(), analysisUnit);
}
