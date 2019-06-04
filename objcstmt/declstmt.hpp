//
//  declstmt.hpp
//  HFStaticAnalysis
//
//  Created by linhuafeng on 2019/5/25.
//  Copyright © 2019 Haven·Lin. All rights reserved.
//

#ifndef HF_STATICANALYSIS_DECLSTMT_HPP
#define HF_STATICANALYSIS_DECLSTMT_HPP

#include "analysishandler/analysishandler.hpp"


class HFDeclStmtHandler : public HFAnalysisHandler<clang::Stmt>
{
private:
    
public:
    HFDeclStmtHandler(HFAnalysisConfigPtr &analysisConfig)
                     : HFAnalysisHandler(analysisConfig) {}
    
    virtual std::string handlerID() {
        return "declStmt";
    }
    
    virtual const clang::ast_matchers::StatementMatcher handlerMatcher() {
        return clang::ast_matchers::declStmt().bind(handlerID());
    }
};

#endif /* declstmt_hpp */
