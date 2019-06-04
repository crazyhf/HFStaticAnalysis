#
#    CMakeLists.txt
#    HFStaticAnalysis
#
#    Created by linhuafeng on 2019/5/1.
#    Copyright © 2019 Haven·Lin. All rights reserved.
#

# If we don't need RTTI or EH, there's no reason to export anything
# from the plugin.
if( NOT MSVC ) # MSVC mangles symbols differently, and
               # main.export contains C++ symbols.
  if( NOT LLVM_REQUIRES_RTTI )
    if( NOT LLVM_REQUIRES_EH )
      set(LLVM_EXPORTED_SYMBOL_FILE ${CMAKE_CURRENT_SOURCE_DIR}/main.exports)
    endif()
  endif()
endif()

set(OBJCDECL_SOURCES
    objcdecl/vardecl.hpp
    objcdecl/vardecl.cpp)

SOURCE_GROUP(objcdecl FILES ${OBJCDECL_SOURCES})

set(OBJCSTMT_SOURCES
    objcstmt/declstmt.hpp
    objcstmt/declstmt.cpp
    objcstmt/compoundstmt.hpp
    objcstmt/compoundstmt.cpp)

SOURCE_GROUP(objcstmt FILES  ${OBJCSTMT_SOURCES})

set(PROPERTY_SOURCES
    property/property.hpp
    property/property.cpp)

SOURCE_GROUP(property FILES  ${PROPERTY_SOURCES})

set(OBJCIMAGE_SOURCES
    objcimage/objcimage.hpp
    objcimage/objcimage.cpp)

SOURCE_GROUP(objcimage FILES ${OBJCIMAGE_SOURCES})

set(OBJCMETHOD_SOURCES
    objcmethod/objcmethod.hpp
    objcmethod/objcmethod.cpp)

SOURCE_GROUP(objcmethod FILES  ${OBJCMETHOD_SOURCES})

set(OBJCCATEGORY_SOURCES
    objccategory/objccategory.hpp
    objccategory/objccategory.cpp)

SOURCE_GROUP(objccategory FILES ${OBJCCATEGORY_SOURCES})

set(ANALYSISHANDLER_SOURCES
    analysishandler/analysistoken.hpp
    analysishandler/analysistoken.cpp
    analysishandler/analysisutils.hpp
    analysishandler/analysisutils.cpp
    analysishandler/analysisconfig.hpp
    analysishandler/analysisconfig.cpp
    analysishandler/analysisparser.hpp
    analysishandler/analysisparser.cpp
    analysishandler/analysishandler.hpp
    analysishandler/declanalysishandler.hpp
    analysishandler/declanalysishandler.cpp)

SOURCE_GROUP(analysishandler FILES ${ANALYSISHANDLER_SOURCES})

set(SOURCES
    main.cpp
    ${OBJCDECL_SOURCES}
    ${OBJCSTMT_SOURCES}
    ${PROPERTY_SOURCES}
    ${OBJCIMAGE_SOURCES}
    ${OBJCMETHOD_SOURCES}
    ${OBJCCATEGORY_SOURCES}
    ${ANALYSISHANDLER_SOURCES})

add_llvm_library(HFStaticAnalysis MODULE ${SOURCES} PLUGIN_TOOL clang)

if(LLVM_ENABLE_PLUGINS AND (WIN32 OR CYGWIN))
  target_link_libraries(HFStaticAnalysis PRIVATE
    clangAST
    clangBasic
    clangFrontend
    LLVMSupport
    )
endif()
