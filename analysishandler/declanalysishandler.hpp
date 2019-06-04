//
//  declanalysishandler.hpp
//  HFStaticAnalysis
//
//  Created by linhuafeng on 2019/5/28.
//  Copyright © 2019 Haven·Lin. All rights reserved.
//

#ifndef HF_STATICANALYSIS_DECLANALYSISHANDLER_HPP
#define HF_STATICANALYSIS_DECLANALYSISHANDLER_HPP

#include "analysishandler/analysishandler.hpp"


class HFDeclAnalysisHandler : public HFAnalysisHandler<clang::Decl>
{
protected:
    bool analyseDeclType(const clang::TypeSourceInfo *typeInfo,
                         HFAnalysisUtils::HFAnalysisUnit &analysisUnit);
    
public:
    HFDeclAnalysisHandler(HFAnalysisConfigPtr &analysisConfig)
                         : HFAnalysisHandler(analysisConfig) {}
};

#endif /* declanalysishandler_hpp */
