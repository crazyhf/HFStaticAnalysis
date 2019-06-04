//
//  analysisparser.hpp
//  HFStaticAnalysis
//
//  Created by linhuafeng on 2019/5/29.
//  Copyright © 2019 Haven·Lin. All rights reserved.
//

#ifndef HF_STATICANALYSIS_ANALYSISPARSER_HPP
#define HF_STATICANALYSIS_ANALYSISPARSER_HPP

#include "analysistoken.hpp"


namespace HFAnalysisParser
{
    std::vector<HFAnalysisToken> parsePropertyAttrRule(llvm::StringRef rule);
}

#endif /* analysisparser_hpp */
