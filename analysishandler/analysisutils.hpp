//
//  analysisutils.hpp
//  HFStaticAnalysis
//
//  Created by linhuafeng on 2019/5/6.
//  Copyright © 2019 Haven·Lin. All rights reserved.
//

#ifndef HF_STATICANALYSIS_ANALYSISUTILS_HPP
#define HF_STATICANALYSIS_ANALYSISUTILS_HPP

#include "analysisconfig.hpp"


namespace HFAnalysisUtils
{
    bool unneedAnalysis(const HFAnalysisConfigPtr &analysisConfig,
                        const clang::SourceLocation &location);
    
    llvm::StringRef extractSourceCode(const clang::CompilerInstance &CI,
                                      const clang::SourceRange &range);
    
    void printSourceCode(const clang::CompilerInstance &CI,
                         const clang::SourceRange &range);
    
    unsigned calContinuousSpace(llvm::StringRef sourceCode, long cursor, size_t endIndex);
    
    bool hasPropertyNullAttr(clang::ObjCPropertyDecl::PropertyAttributeKind attrKind);
    
    bool hasPropertyAtomicAttr(clang::ObjCPropertyDecl::PropertyAttributeKind attrKind);
    
    bool hasPropertyMemAttr(clang::ObjCPropertyDecl::PropertyAttributeKind attrKind);
    
    std::string propertyNullAttrStr(const clang::ObjCPropertyDecl *propertyDecl);
    
    std::string propertyAtomicAttrStr(clang::ObjCPropertyDecl::PropertyAttributeKind attrKind);
    
    std::string propertyMemAttrStr(clang::ObjCPropertyDecl::PropertyAttributeKind attrKind);
    
    std::string propertyReadwriteAttrStr(clang::ObjCPropertyDecl::PropertyAttributeKind attrKind);
    
    std::string propertyAccessorAttrStr(const clang::ObjCPropertyDecl *propertyDecl);
    
    
    class HFAnalysisUnit {
    private:
        llvm::StringRef _sourceCode;
        
        llvm::StringRef _unitName;
        clang::SourceRange _range;
        const HFAnalysisConfigPtr &_analysisConfig;
        
        llvm::StringRef _sourceFile;
        unsigned        _lineNumber;
        unsigned        _columnNumber;
        
    public:
        HFAnalysisUnit(const HFAnalysisConfigPtr &analysisConfig,
                       llvm::StringRef unitName,
                       const clang::SourceRange &range)
                      : _sourceCode(extractSourceCode(analysisConfig->CI(), range)),
                        _unitName(unitName), _range(range), _analysisConfig(analysisConfig)
        {
            clang::SourceManager &sourceMgr = analysisConfig->CI().getSourceManager();
            
            _sourceFile     = sourceMgr.getFilename(range.getBegin());
            _lineNumber     = sourceMgr.getSpellingLineNumber(range.getBegin());
            _columnNumber   = sourceMgr.getSpellingColumnNumber(range.getBegin());
        }
        
        bool unneedAnalysis()
        {
            if (!_sourceCode.size()) return true;
            
            return HFAnalysisUtils::unneedAnalysis(_analysisConfig, _range.getBegin());
        }
        
        
        void printAnalysisUnit();
        
        void printPropertyAttr(const clang::ObjCPropertyDecl *propertyDecl);
        
        
        llvm::StringRef sourceCode() const { return _sourceCode; }
        
        llvm::StringRef unitName() const { return _unitName; }
        
        clang::SourceRange sourceRange() const { return _range; }
        
        unsigned lineNumber() const { return _lineNumber; }
        
        unsigned columnNumber() const { return _columnNumber; }
        
        llvm::StringRef sourceFile() const { return _sourceFile; }
    };
    
    typedef std::vector<std::string> StrVec;
    typedef std::vector<std::string> &StrVecRef;
    typedef const std::vector<std::string> ConstStrVec;
    typedef const std::vector<std::string> &ConstStrVecRef;
}

#endif /* analysisutils_hpp */
