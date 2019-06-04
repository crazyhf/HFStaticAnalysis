//
//  main.cpp
//  HFStaticAnalysis
//
//  Created by linhuafeng on 2019/5/1.
//  Copyright © 2019 Haven·Lin. All rights reserved.
//

#include "objccategory/objccategory.hpp"
#include "objcimage/objcimage.hpp"
#include "property/property.hpp"
#include "objcdecl/vardecl.hpp"
#include "objcstmt/declstmt.hpp"
#include "objcstmt/compoundstmt.hpp"

#include "clang/Sema/Sema.h"

#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/FrontendPluginRegistry.h"

using namespace clang;
using namespace clang::ast_matchers;


namespace
{
    class HFStaticAnalysisConsumer : public ASTConsumer
    {
    private:
        HFObjCCategoryHandler _objcCateHandler;
        HFPropertyHandler     _propertyHandler;
        HFObjCImageHandler    _objcImgHandler;
        HFVarDeclHandler      _varDeclHandler;
        MatchFinder           _matcher;
        
    public:
        HFStaticAnalysisConsumer(HFAnalysisConfigPtr &analysisConfig)
                                : _objcCateHandler(analysisConfig),
                                  _propertyHandler(analysisConfig),
                                  _objcImgHandler(analysisConfig),
                                  _varDeclHandler(analysisConfig)
        {
            _matcher.addMatcher(_objcCateHandler.handlerMatcher(), &_objcCateHandler);
            _matcher.addMatcher(_propertyHandler.handlerMatcher(), &_propertyHandler);
            _matcher.addMatcher(_varDeclHandler.handlerMatcher(), &_varDeclHandler);
        }
        
        virtual void HandleTranslationUnit(ASTContext &ctx)
        {
            _matcher.matchAST(ctx);
        }
    };
    
    class HFStaticAnalysisAction : public PluginASTAction
    {
    private:
        HFAnalysisConfigPtr _analysisConfig;
        
    protected:
        std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                       StringRef InFile) override
        {
            return llvm::make_unique<HFStaticAnalysisConsumer>(_analysisConfig);
        }
        
        bool ParseArgs(const CompilerInstance &CI,
                       const std::vector<std::string> &args) override
        {
            StringRef workpath, configFile;
            bool enableDebugTrace = false;
            
            for (std::vector<std::string>::const_iterator ite = args.begin(); ite != args.end(); ite++) {
                if (ite->empty()) continue;
                
                StringRef argStr = StringRef(*ite);
                if (argStr.startswith_lower("workpath:") && argStr.size() > 9) {
                    workpath = argStr.substr(9);
                    // std::cout << HF_ANALYSIS_TAG_TRACE
                    //           << " ParseArgs workpath "
                    //           << workpath.str() << std::endl;
                } else if (argStr.startswith_lower("config:") && argStr.size() > 7) {
                    configFile = argStr.substr(7);
                    // std::cout << HF_ANALYSIS_TAG_TRACE
                    //           << " ParseArgs config "
                    //           << configFile.str() << std::endl;
                } else if (argStr.equals("enableDebugTrace")) {
                    enableDebugTrace = true;
                }
            }
            
            _analysisConfig = HFAnalysisConfigPtr(new HFAnalysisConfig(CI,
                                                                       workpath,
                                                                       configFile,
                                                                       enableDebugTrace));
            // DiagnosticsEngine &D = CI.getDiagnostics();
            // D.Report(D.getCustomDiagID(DiagnosticsEngine::Error, "start HFStaticAnalysis plugin..."));
            return true;
        }
    };
}

static FrontendPluginRegistry::Add<HFStaticAnalysisAction> X("HFStaticAnalysis", "static analysis");
