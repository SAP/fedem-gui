# SPDX-FileCopyrightText: 2023 SAP SE
#
# SPDX-License-Identifier: Apache-2.0
#
# This file is part of FEDEM - https://openfedem.org

set ( LIB_ID vpmUIQtTopLevels )

if ( USE_FORTRAN )
  string ( APPEND CMAKE_CXX_FLAGS " -DFT_HAS_PREVIEW" )
endif ( USE_FORTRAN )
if ( USE_EXT_CTRLSYS )
  string ( APPEND CMAKE_CXX_FLAGS " -DFT_HAS_EXTCTRL" )
endif ( USE_EXT_CTRLSYS )


## Files with header and source with same name
set ( COMPONENT_FILE_LIST  FuiQtAdvAnalysisOptions FuiQtAnimationControl FuiQtAnimExportSetup
                           FuiQtAppearance FuiQtCtrlModeller FuiQtCtrlGridAttributes
                           FuiQtCSSelector
                           FuiQtEigenOptions FuiQtFppOptions FuiQtGageOptions
                           FuiQtLinkRamSettings
                           FuiQtMainWindow FuiQtMiniFileBrowser FuiQtModeller
                           FuiQtModelManager FuiQtModelPreferences FuiQtOutputList
                           FuiQtPlugins FuiQtPreferences FuiQtProperties
                           FuiQtRDBSelector
                           FuiQtRepeatCurve FuiQtStressOptions FuiQtViewSettings
                           FuiQtSeaEnvironment FuiQtEventDefinition
                           FuiQtSplitBeam FuiQtCreateBeamstringPair FuiQtObjectBrowser
                           FuiQtMooringLine FuiQtModelExport )
if ( USE_WINDPOW )
  list ( APPEND COMPONENT_FILE_LIST FuiQtAirEnvironment FuiQtAirfoilDefinition FuiQtBladeDefinition
                                    FuiQtCreateTurbineAssembly FuiQtCreateTurbineTower FuiQtTurbWind )
endif ( USE_WINDPOW )

if ( Qwt_LIBRARY )
  list ( APPEND COMPONENT_FILE_LIST FuiQtGraphView FuiQtRDBMEFatigue )
endif ( Qwt_LIBRARY )

if ( USE_NCODE_INTERFACE )
  list ( APPEND COMPONENT_FILE_LIST FuiQtDutyCycleOptions )
endif ( USE_NCODE_INTERFACE )


## Pure header files, i.e., header files without a corresponding source file
set ( HEADER_FILE_LIST )
## Pure implementation files, i.e., source files without corresponding header
set ( SOURCE_FILE_LIST FuiQt FuiQtMsg )


foreach ( FILE ${COMPONENT_FILE_LIST} )
  set ( CPP_SOURCE_FILES ${CPP_SOURCE_FILES} ${FILE}.C )
  set ( HPP_HEADER_FILES ${HPP_HEADER_FILES} ${FILE}.H )
endforeach ( FILE ${COMPONENT_FILE_LIST} )


foreach ( FILE ${HEADER_FILE_LIST} )
  set ( HPP_HEADER_FILES ${HPP_HEADER_FILES} ${FILE}.H )
endforeach ( FILE ${HEADER_FILE_LIST} )


foreach ( FILE ${SOURCE_FILE_LIST} )
  set ( CPP_SOURCE_FILES ${CPP_SOURCE_FILES} ${FILE}.C )
endforeach ( FILE ${SOURCE_FILE_LIST} )


## Header files to be moc'ed
set ( QT_MOC_HEADER_FILE_LIST FuiQtEventDefinition FuiQtModelExport )
if ( USE_WINDPOW )
  list ( APPEND QT_MOC_HEADER_FILE_LIST FuiQtAirfoilDefinition FuiQtBladeDefinition FuiQtTurbWind )
endif ( USE_WINDPOW )

foreach ( FILE ${QT_MOC_HEADER_FILE_LIST} )
  set ( QT_MOC_HEADER_FILES ${QT_MOC_HEADER_FILES} ${FILE}.H )
endforeach ( FILE ${QT_MOC_HEADER_FILE_LIST} )


include_directories ( ${CMAKE_CURRENT_BINARY_DIR} )
# Named moc_<filename>.cxx where input is filname.H
QT4_WRAP_CPP ( QT_MOC_SOURCE_FILES ${QT_MOC_HEADER_FILES} )

set ( HEADER_FILES ${HPP_HEADER_FILES} )
set ( SOURCE_FILES ${CPP_SOURCE_FILES} ${QT_MOC_SOURCE_FILES} )


# Dependencies and system libraries

find_package ( Qt4 REQUIRED )
include ( ${QT_USE_FILE} )

set ( DEPENDENCY_LIST vpmUITopLevels vpmUIQtComponents FFuQtComponents FFuQtBaseClasses FFuComponents )
if ( USE_WINDPOW )
  list ( APPEND DEPENDENCY_LIST FFuInputTables )
endif ( USE_WINDPOW )
list ( APPEND DEPENDENCY_LIST vpmDB )


# Build the library ${LIB_ID}

message ( STATUS "Building library ${LIB_ID}" )
add_library ( ${LIB_ID} ${SOURCE_FILES} ${HEADER_FILES} )
target_link_libraries ( ${LIB_ID} ${DEPENDENCY_LIST} )


# Install the dependent shared object libraries

set ( QT_LIBS Qt3Support QtCore QtGui QtNetwork QtOpenGL QtSql QtXml )
foreach ( LIB ${QT_LIBS} )
  get_target_property ( LIB_PATH Qt4::${LIB} LOCATION )
  if ( WIN )
    string ( REPLACE ".lib" ".dll" DLL_PATH ${LIB_PATH} )
    if ( FTENV_VERBOSE )
      message ( STATUS "Installing ${DLL_PATH} to ${CMAKE_INSTALL_PREFIX}" )
    endif ( FTENV_VERBOSE )
    install ( FILES ${DLL_PATH} DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
              CONFIGURATIONS Release RelWithDebInfo )
  endif ( WIN )
endforeach ( LIB ${QT_LIBS} )
