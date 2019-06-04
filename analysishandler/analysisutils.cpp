//
//  analysisutils.cpp
//  HFStaticAnalysis
//
//  Created by linhuafeng on 2019/5/6.
//  Copyright © 2019 Haven·Lin. All rights reserved.
//

#include "analysisutils.hpp"

using namespace llvm;
using namespace clang;
using namespace clang::ast_matchers;


bool HFAnalysisUtils::unneedAnalysis(const HFAnalysisConfigPtr &analysisConfig,
                                     const SourceLocation &location)
{
    StringRef filename = analysisConfig->CI().getSourceManager().getFilename(location);
    if (filename.empty() || 0 == filename.find("/Applications/Xcode.app/")) {
        return true;
    }
    
    ConstStrVecRef ignoreFileTypes = analysisConfig->ignoreFileTypes();
    for (StrVec::const_iterator ite = ignoreFileTypes.begin();
         ite != ignoreFileTypes.end(); ite++) {
        if (filename.endswith_lower(*ite)) {
            return true;
        }
    }
    
    ConstStrVecRef ignoreDirectories = analysisConfig->ignoreDirectories();
    for (StrVec::const_iterator ite = ignoreDirectories.begin();
         ite != ignoreDirectories.end(); ite++) {
        if (filename.startswith_lower(*ite)) {
            return true;
        }
    }
    
    // std::cout << HF_ANALYSIS_TAG_TRACE
    //           << " analysis "
    //           << filename.str()
    //           << std::endl;
    return false;
}

StringRef HFAnalysisUtils::extractSourceCode(const CompilerInstance &CI,
                                             const SourceRange &range)
{
    long length = 0;
    if (range.getBegin().isInvalid() || range.getEnd().isInvalid()
        || (length = range.getEnd().getRawEncoding() - range.getBegin().getRawEncoding()) <= 0) {
        return StringRef();
    }
    
    const char * pSourceCode = CI.getSourceManager().getCharacterData(range.getBegin());
    
    return StringRef(pSourceCode, length);
}

void HFAnalysisUtils::printSourceCode(const CompilerInstance &CI,
                                      const SourceRange &range)
{
    std::cout << HF_ANALYSIS_TAG_TRACE
              << " source code piece" << std::endl
              << "================================" << std::endl
              << "|||" << extractSourceCode(CI, range).str() << "|||" << std::endl
              << "================================" << std::endl;
}

unsigned HFAnalysisUtils::calContinuousSpace(StringRef sourceCode, long cursor, size_t endIndex)
{
    if (endIndex > sourceCode.size()) endIndex = sourceCode.size();
    
    if (cursor < 0 || cursor >= (long)sourceCode.size() || cursor == (long)endIndex) return cursor;
    
    long endCursor = endIndex;
    unsigned resultCursor = cursor;
    if (cursor > endCursor) {
        while (--cursor >= endCursor) {
            if (' ' == sourceCode[cursor]) {
                resultCursor = cursor;
            } else {
                break;
            }
        }
    } else {
        while (++cursor < endCursor) {
            resultCursor = cursor;
            if (' ' != sourceCode[cursor]) break;
        }
        if (cursor == endCursor) {
            resultCursor = cursor;
        }
    }
    
    return resultCursor;
}

bool HFAnalysisUtils::hasPropertyNullAttr(ObjCPropertyDecl::PropertyAttributeKind attrKind)
{
    return (attrKind & (ObjCPropertyDecl::OBJC_PR_nullability
                        | ObjCPropertyDecl::OBJC_PR_null_resettable));
}

bool HFAnalysisUtils::hasPropertyAtomicAttr(ObjCPropertyDecl::PropertyAttributeKind attrKind)
{
    return (attrKind & (ObjCPropertyDecl::OBJC_PR_atomic
                        | ObjCPropertyDecl::OBJC_PR_nonatomic));
}

bool HFAnalysisUtils::hasPropertyMemAttr(ObjCPropertyDecl::PropertyAttributeKind attrKind)
{
    return (attrKind & (ObjCPropertyDecl::OBJC_PR_assign
                        | ObjCPropertyDecl::OBJC_PR_retain
                        | ObjCPropertyDecl::OBJC_PR_copy
                        | ObjCPropertyDecl::OBJC_PR_weak
                        | ObjCPropertyDecl::OBJC_PR_strong
                        | ObjCPropertyDecl::OBJC_PR_unsafe_unretained));
}

std::string HFAnalysisUtils::propertyNullAttrStr(const ObjCPropertyDecl *propertyDecl)
{
    ObjCPropertyDecl::PropertyAttributeKind attrKind = propertyDecl->getPropertyAttributes();
    
    if (ObjCPropertyDecl::OBJC_PR_nullability & attrKind) {
        if (ObjCPropertyDecl::OBJC_PR_null_resettable & attrKind) {
            return "null_resettable";
        }
        if (propertyDecl->isOptional()) {
            return "nullable";
        } else {
            return "nonnull";
        }
    }
    return "";
}

std::string HFAnalysisUtils::propertyAtomicAttrStr(ObjCPropertyDecl::PropertyAttributeKind attrKind)
{
    if (ObjCPropertyDecl::OBJC_PR_atomic & attrKind) {
        return "atomic";
    } else if (ObjCPropertyDecl::OBJC_PR_nonatomic & attrKind) {
        return "nonatomic";
    }
    return "";
}

std::string HFAnalysisUtils::propertyMemAttrStr(clang::ObjCPropertyDecl::PropertyAttributeKind attrKind)
{
    if (ObjCPropertyDecl::OBJC_PR_assign & attrKind) {
        return "assign";
    } else if (ObjCPropertyDecl::OBJC_PR_retain & attrKind) {
        return "retain";
    } else if (ObjCPropertyDecl::OBJC_PR_copy & attrKind) {
        return "copy";
    } else if (ObjCPropertyDecl::OBJC_PR_weak & attrKind) {
        return "weak";
    } else if (ObjCPropertyDecl::OBJC_PR_strong & attrKind) {
        return "strong";
    } else if (ObjCPropertyDecl::OBJC_PR_unsafe_unretained & attrKind) {
        return "unsafe_unretained";
    }
    return "";
}

std::string HFAnalysisUtils::propertyReadwriteAttrStr(ObjCPropertyDecl::PropertyAttributeKind attrKind)
{
    if (ObjCPropertyDecl::OBJC_PR_readwrite & attrKind) {
        return "readwrite";
    } else if (ObjCPropertyDecl::OBJC_PR_readonly & attrKind) {
        return "readonly";
    }
    return "";
}

std::string HFAnalysisUtils::propertyAccessorAttrStr(const ObjCPropertyDecl *propertyDecl)
{
    ObjCPropertyDecl::PropertyAttributeKind attrKind = propertyDecl->getPropertyAttributes();
    
    std::string attrString;
    
    if (ObjCPropertyDecl::OBJC_PR_setter & attrKind) {
        attrString.append("setter=").append(propertyDecl->getSetterName().getAsString());
        
    }
    
    if (ObjCPropertyDecl::OBJC_PR_getter & attrKind) {
        if (attrString.length()) {
            attrString.append(", getter=");
        } else {
            attrString.append("getter=");
        }
        attrString.append(propertyDecl->getGetterName().getAsString());
    }
    
    return attrString;
}


void HFAnalysisUtils::HFAnalysisUnit::printAnalysisUnit()
{
    if (!_analysisConfig->enableDebugTrace()) return;
    
    std::cout << HF_ANALYSIS_TAG_TRACE
              << " HFAnalysisUnit-" << _unitName.str() << " : "
              << _sourceFile.str()
              << "(row:" << _lineNumber << ",col:" << _columnNumber << ")" << std::endl
              << "=====================================" << std::endl
              << "|||||" << _sourceCode.str() << "|||||" << std::endl
              << "=====================================" << std::endl;
}

void HFAnalysisUtils::HFAnalysisUnit::printPropertyAttr(const ObjCPropertyDecl *propertyDecl)
{
    if (!_analysisConfig->enableDebugTrace()) return;
    
    ObjCPropertyDecl::PropertyAttributeKind attrKind = propertyDecl->getPropertyAttributes();
    
    std::string attrString(propertyNullAttrStr(propertyDecl));
    
    std::string atomicAttrStr(propertyAtomicAttrStr(attrKind));
    if (atomicAttrStr.length()) {
        if (attrString.length()) {
            attrString.append(", ");
        }
        attrString.append(atomicAttrStr);
    }
    
    std::string memAttrStr(propertyMemAttrStr(attrKind));
    if (memAttrStr.length()) {
        if (attrString.length()) {
            attrString.append(", ");
        }
        attrString.append(memAttrStr);
    }
    
    std::string readwriteAttrStr(propertyReadwriteAttrStr(attrKind));
    if (readwriteAttrStr.length()) {
        if (attrString.length()) {
            attrString.append(", ");
        }
        attrString.append(readwriteAttrStr);
    }
    
    std::string accessorAttrStr(propertyAccessorAttrStr(propertyDecl));
    if (accessorAttrStr.length()) {
        if (attrString.length()) {
            attrString.append(", ");
        }
        attrString.append(accessorAttrStr);
    }
    
    std::cout << HF_ANALYSIS_TAG_TRACE
              << " HFAnalysisUnit-" << unitName().str() << " : "
              << sourceFile().str()
              << "(row:" << lineNumber()
              << ",col:" << columnNumber() << ")" << std::endl
              << "=====================================" << std::endl
              << "|||||(" << attrString << ")|||||" << std::endl
              << "=====================================" << std::endl;
}


