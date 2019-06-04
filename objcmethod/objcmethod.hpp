//
//  objcmethod.hpp
//  HFStaticAnalysis
//
//  Created by linhuafeng on 2019/5/2.
//  Copyright © 2019 Haven·Lin. All rights reserved.
//

#ifndef HF_STATICANALYSIS_OBJCMETHOD_HPP
#define HF_STATICANALYSIS_OBJCMETHOD_HPP

#include "analysishandler/analysishandler.hpp"


class HFObjCMethodHandler : public HFAnalysisHandler<clang::Decl>
{
    
public:
    HFObjCMethodHandler(HFAnalysisConfigPtr &analysisConfig)
                       : HFAnalysisHandler(analysisConfig) {}
};

#endif /* objcmethod_hpp */
