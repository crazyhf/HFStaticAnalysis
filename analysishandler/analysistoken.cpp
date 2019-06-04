//
//  analysistoken.cpp
//  HFStaticAnalysis
//
//  Created by linhuafeng on 2019/5/29.
//  Copyright © 2019 Haven·Lin. All rights reserved.
//

#include "analysistoken.hpp"

using namespace HFAnalysisUtils;

using namespace llvm;
using namespace clang;


namespace HFAnalysisParser
{
    unsigned HFAnalysisToken::digestToken(StringRef sourceCode, unsigned uStart)
    {
        if (uStart < sourceCode.size()) {
            StringRef codePiece = sourceCode.substr(uStart);
            for (unsigned idx = 0; idx < _tokenWords.wordsCount; idx++) {
                if (codePiece.startswith(_tokenWords.wordsList[idx])) {
                    return uStart + strlen(_tokenWords.wordsList[idx]);
                }
            }
        }
        return uStart;
    }
    
    unsigned HFAnalysisSerialTok::digestToken(StringRef sourceCode, unsigned uStart)
    {
        unsigned tokCount = _serialCount;
        if (0 == _serialCount) {
            tokCount = sourceCode.size() - uStart;
        }
        
        for (unsigned idx = 0; idx < tokCount; idx++) {
            if (auto step = HFAnalysisToken::digestToken(sourceCode, uStart) - uStart) {
                uStart += step;
            } else {
                break;
            }
        }
        return uStart;
    }
}
