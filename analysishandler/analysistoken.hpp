//
//  analysistoken.hpp
//  HFStaticAnalysis
//
//  Created by linhuafeng on 2019/5/29.
//  Copyright © 2019 Haven·Lin. All rights reserved.
//

#ifndef HF_STATICANALYSIS_ANALYSISTOKEN_HPP
#define HF_STATICANALYSIS_ANALYSISTOKEN_HPP

#include "analysisutils.hpp"

#define HF_ANALYSISTOKEN_WORDS1(_tok_words_1_) \
        { _tok_words_1_ }
#define HF_ANALYSISTOKEN_WORDS2(_tok_words_1_, _tok_words_2_) \
        { _tok_words_1_, _tok_words_2_ }
#define HF_ANALYSISTOKEN_WORDS3(_tok_words_1_, _tok_words_2_, _tok_words_3_) \
        { _tok_words_1_, _tok_words_2_, _tok_words_3_ }
#define HF_ANALYSISTOKEN_WORDS4(_tok_words_1_, _tok_words_2_, _tok_words_3_, _tok_words_4_) \
        { _tok_words_1_, _tok_words_2_, _tok_words_3_, _tok_words_4_ }
#define HF_ANALYSISTOKEN_WORDS5(_tok_words_1_, _tok_words_2_, _tok_words_3_, _tok_words_4_, _tok_words_5_) \
        { _tok_words_1_, _tok_words_2_, _tok_words_3_, _tok_words_4_, _tok_words_5_ }
#define HF_ANALYSISTOKEN_WORDS6(_tok_words_1_, _tok_words_2_, _tok_words_3_, _tok_words_4_, _tok_words_5_, _tok_words_6_) \
        { _tok_words_1_, _tok_words_2_, _tok_words_3_, _tok_words_4_, _tok_words_5_, _tok_words_6_ }
#define HF_ANALYSISTOKEN_WORDS7(_tok_words_1_, _tok_words_2_, _tok_words_3_, _tok_words_4_, _tok_words_5_, _tok_words_6_, _tok_words_7_) \
        { _tok_words_1_, _tok_words_2_, _tok_words_3_, _tok_words_4_, _tok_words_5_, _tok_words_6_, _tok_words_7_ }
#define HF_ANALYSISTOKEN_WORDS8(_tok_words_1_, _tok_words_2_, _tok_words_3_, _tok_words_4_, _tok_words_5_, _tok_words_6_, _tok_words_7_, _tok_words_8_) \
        { _tok_words_1_, _tok_words_2_, _tok_words_3_, _tok_words_4_, _tok_words_5_, _tok_words_6_, _tok_words_7_, _tok_words_8_ }
#define HF_ANALYSISTOKEN_WORDS9(_tok_words_1_, _tok_words_2_, _tok_words_3_, _tok_words_4_, _tok_words_5_, _tok_words_6_, _tok_words_7_, _tok_words_8_, _tok_words_9_) \
        { _tok_words_1_, _tok_words_2_, _tok_words_3_, _tok_words_4_, _tok_words_5_, _tok_words_6_, _tok_words_7_, _tok_words_8_, _tok_words_9_ }
#define HF_ANALYSISTOKEN_WORDS10(_tok_words_1_, _tok_words_2_, _tok_words_3_, _tok_words_4_, _tok_words_5_, _tok_words_6_, _tok_words_7_, _tok_words_8_, _tok_words_9_, _tok_words_10_) \
        { _tok_words_1_, _tok_words_2_, _tok_words_3_, _tok_words_4_, _tok_words_5_, _tok_words_6_, _tok_words_7_, _tok_words_8_, _tok_words_9_, _tok_words_10_ }

#define HF_ANALYSISTOKEN_GEN(_tok_words_count, _tok_name_, _tok_words_list_) \
        static const char *_tok_name_##_WORDS[_tok_words_count] = _tok_words_list_; \
        static TokenWords _tok_name_ = { \
            sizeof(_tok_name_##_WORDS) / sizeof(char *), \
            #_tok_name_, \
            _tok_name_##_WORDS \
        };


namespace HFAnalysisParser
{
    struct TokenWords {
        unsigned wordsCount;
        const char *tokenName;
        const char **wordsList;
    };
    
    HF_ANALYSISTOKEN_GEN(6, OC_ANALYSIS_SPACE,
                         HF_ANALYSISTOKEN_WORDS6(" ", "\r", "\n",
                                                 "\t", "\v", "\f"));
    HF_ANALYSISTOKEN_GEN(2, OC_PROPERTY_ATOMIC,
                         HF_ANALYSISTOKEN_WORDS2("atomic", "nonatomic"));
    HF_ANALYSISTOKEN_GEN(2, OC_PROPERTY_ACCESSOR,
                         HF_ANALYSISTOKEN_WORDS2("setter", "getter"));
    HF_ANALYSISTOKEN_GEN(2, OC_PROPERTY_READWRITE,
                         HF_ANALYSISTOKEN_WORDS2("readwrite", "readonly"));
    HF_ANALYSISTOKEN_GEN(3, OC_PROPERTY_NULLABLE,
                         HF_ANALYSISTOKEN_WORDS3("null_resettable", "nullable", "nonnull"));
    HF_ANALYSISTOKEN_GEN(6, OC_PROPERTY_MEMORY,
                         HF_ANALYSISTOKEN_WORDS6("assign", "retain", "copy",
                                                 "weak", "strong", "unsafe_unretained"));
    
    class HFAnalysisToken
    {
    public:
        enum Necessity {
            Optional, Required
        };
        
    protected:
        Necessity _necessity;
        TokenWords &_tokenWords;
        
    public:
        HFAnalysisToken(Necessity necessity,
                        TokenWords &tokenWords)
                       : _necessity(necessity), _tokenWords(tokenWords)
        {
            ;
        }
        
        virtual ~HFAnalysisToken()
        {
            ;
        }
        
        Necessity necessity() { return _necessity; }
        
        virtual unsigned digestToken(llvm::StringRef sourceCode, unsigned uStart);
    };
    
    class HFAnalysisSerialTok : public HFAnalysisToken
    {
    protected:
        unsigned _serialCount; // 0 is infinitely continuous
        
    public:
        HFAnalysisSerialTok(Necessity necessity,
                            TokenWords &tokenWords,
                            unsigned serialCount = 1)
                           : HFAnalysisToken(necessity, tokenWords),
                             _serialCount(serialCount)
        {
            ;
        }
        
        virtual unsigned digestToken(llvm::StringRef sourceCode, unsigned uStart);
    };
    
    class HFAnalysisSerialSpace : HFAnalysisSerialTok
    {
    public:
        HFAnalysisSerialSpace(Necessity necessity,
                              unsigned serialCount = 0)
                             : HFAnalysisSerialTok(necessity, OC_ANALYSIS_SPACE, serialCount)
        {
            ;
        }
    };
}

#endif /* analysistoken_hpp */
