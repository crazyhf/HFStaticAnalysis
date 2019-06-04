//
//  compoundstmt.hpp
//  HFStaticAnalysis
//
//  Created by linhuafeng on 2019/5/2.
//  Copyright © 2019 Haven·Lin. All rights reserved.
//

#ifndef HF_STATICANALYSIS_COMPOUNDSTMT_HPP
#define HF_STATICANALYSIS_COMPOUNDSTMT_HPP

#include "analysishandler/analysishandler.hpp"


class HFCompoundStmtHandler : public HFAnalysisHandler<clang::Stmt>
{
    
public:
    HFCompoundStmtHandler(HFAnalysisConfigPtr &analysisConfig)
                         : HFAnalysisHandler(analysisConfig) {}
    
    virtual std::string handlerID() {
        return "compoundStmt";
    }
    
    virtual const clang::ast_matchers::StatementMatcher handlerMatcher() {
        return clang::ast_matchers::compoundStmt().bind(handlerID());
    }
};

#endif /* compoundstmt_hpp */
