//
//  declanalysishandler.cpp
//  HFStaticAnalysis
//
//  Created by linhuafeng on 2019/5/28.
//  Copyright © 2019 Haven·Lin. All rights reserved.
//

#include "declanalysishandler.hpp"

using namespace HFAnalysisUtils;

using namespace clang;
using namespace clang::ast_matchers;


bool HFDeclAnalysisHandler::analyseDeclType(const TypeSourceInfo *typeInfo,
                                            HFAnalysisUtils::HFAnalysisUnit &analysisUnit)
{
    if (nullptr == typeInfo) {
        std::cout << HF_ANALYSIS_TAG_ERROR
                  << " HFAnalysisUnit-" << analysisUnit.unitName().str() << " : "
                  << "typeInfo is null "
                  << analysisUnit.sourceFile().str()
                  << "(row:" << analysisUnit.lineNumber()
                  << ",col:" << analysisUnit.columnNumber() << ")" << std::endl;
        return false;
    }
    
    TypeLoc curTypeLoc = typeInfo->getTypeLoc();
    if (!curTypeLoc || curTypeLoc.getBeginLoc().isInvalid()) {
        std::cout << HF_ANALYSIS_TAG_ERROR
                  << " HFAnalysisUnit-" << analysisUnit.unitName().str() << " : "
                  << "typeLoc isInvalid "
                  << analysisUnit.sourceFile().str()
                  << "(row:" << analysisUnit.lineNumber()
                  << ",col:" << analysisUnit.columnNumber() << ")" << std::endl;
        return false;
    }
    
    const SourceRange &sourceRange = analysisUnit.sourceRange();
    
    if (sourceRange.getEnd().isInvalid()) {
        std::cout << HF_ANALYSIS_TAG_ERROR
                  << " HFAnalysisUnit-" << analysisUnit.unitName().str() << " : "
                  << "sourceRange.getEnd().isInvalid() "
                  << analysisUnit.sourceFile().str()
                  << "(row:" << analysisUnit.lineNumber()
                  << ",col:" << analysisUnit.columnNumber() << ")" << std::endl;
        return false;
    }
    
    const SourceManager &sourceMgr = _analysisConfig->CI().getSourceManager();
    DiagnosticsEngine &D = _analysisConfig->Diagnostics();
    
    unsigned endCursor   = sourceRange.getEnd().getRawEncoding();
    unsigned beginCursor = curTypeLoc.getBeginLoc().getRawEncoding();
    
    StringRef sourceCode(sourceMgr.getCharacterData(curTypeLoc.getBeginLoc()), endCursor - beginCursor);
    
    const SplitQualType &qualType = typeInfo->getType().split();
    if (qualType.Ty->isPointerType()
        || qualType.Ty->isObjCObjectPointerType()) {
        long endIdx = endCursor - beginCursor;
        long curIdx = sourceCode.find_last_of('*');
        if (std::string::npos == (unsigned long)curIdx || curIdx >= endIdx) return true;
        
        bool hasSpaceBefore = true, hasSpaceAfter = false;
        if (0 == curIdx - calContinuousSpace(sourceCode, curIdx, 0)) {
            std::cout << HF_ANALYSIS_TAG_WARN
                      << " must have space before asterisk, "
                      << analysisUnit.sourceFile().str()
                      << "(row:" << analysisUnit.lineNumber()
                      << ",col:" << sourceMgr.getSpellingColumnNumber(curTypeLoc.getBeginLoc())
                      << ")" << std::endl;
            hasSpaceBefore = false;
        }
        unsigned resultIdx = calContinuousSpace(sourceCode, curIdx, endIdx);
        if (resultIdx - curIdx > 1
            && StringRef(sourceCode.data() + resultIdx, analysisUnit.unitName().size()).equals(analysisUnit.unitName())) {
            std::cout << HF_ANALYSIS_TAG_WARN
                      << " asterisk must put together with property name, "
                      << analysisUnit.sourceFile().str()
                      << "(row:" << analysisUnit.lineNumber()
                      << ",col:" << sourceMgr.getSpellingColumnNumber(curTypeLoc.getBeginLoc())
                      << ")" << std::endl;
            hasSpaceAfter = true;
        }
        
        if (!hasSpaceBefore || hasSpaceAfter) {
            SourceLocation asteriskLoc = curTypeLoc.getBeginLoc().getLocWithOffset(curIdx);
            FixItHint hint;
            if (!hasSpaceBefore && hasSpaceAfter) {
                hint = FixItHint::CreateReplacement(SourceRange(asteriskLoc, asteriskLoc.getLocWithOffset(resultIdx - curIdx + analysisUnit.unitName().size() - 1)), std::string(" *").append(analysisUnit.unitName().str()));
            } else if (!hasSpaceBefore) {
                hint = FixItHint::CreateInsertion(asteriskLoc, " ");
            } else if (hasSpaceAfter) {
                hint = FixItHint::CreateReplacement(SourceRange(asteriskLoc, asteriskLoc.getLocWithOffset(resultIdx - curIdx + analysisUnit.unitName().size() - 1)), std::string("*").append(analysisUnit.unitName().str()));
            }
            D.Report(sourceRange.getEnd(),
                     D.getCustomDiagID(DiagnosticsEngine::Warning,
                                       HF_DIAGNOSTIC_MSG("asterisk have space before and put together with property name"))).AddFixItHint(hint);
        }
    }
    return true;
}
