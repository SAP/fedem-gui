// SPDX-FileCopyrightText: 2023 SAP SE
//
// SPDX-License-Identifier: Apache-2.0
//
// This file is part of FEDEM - https://openfedem.org
////////////////////////////////////////////////////////////////////////////////

#ifndef FD_GROUP_PART_KIT_H
#define FD_GROUP_PART_KIT_H

#include "vpmDisplay/FdFEGroupPart.H"
#include <Inventor/nodekits/SoBaseKit.h> 
#include <vector>

#ifdef win32
#include <SoWinLeaveScope.h>
#endif

class SoPolygonOffset;
class SoMaterialBinding;
class SoLightModel;
class SoDrawStyle;
class SoVertexProperty;
class SoTransform;
class SoPackedColor;

class FdFEModel;
class FFaLegendMapper;


class FdFEGroupPartKit : public SoBaseKit, public FdFEGroupPart
{ 
public:
  FdFEGroupPartKit(); 
  static void init(); 
  void setSpecialGraphics(SoSeparator * scene, bool isLineShape);
  virtual bool hasSpecialGraphics();
  virtual void setVizMode(FdGpVizModeEnum mode);
  float getResultFromMaterialIndex(unsigned int matIdx);

protected: 
  virtual ~FdFEGroupPartKit();

private:
  //
  // Reimplementation of Visualization engine interface :
  //
  ////////////////////////////////////////
  
  virtual void setFdPointer(FdObject *backPt);

  virtual void setShapeIndexes(bool isFaceShape, const std::vector < std::vector<int> > & idxs);
  virtual void generateShapeIndexes();

  virtual void toggleOn       ( bool  turnOn );
  virtual void setLook        ( const FFdLook & aLook );
  virtual void setGouradOn    ( bool  setOn );
  virtual void set3DLLineWidth( int   lineWidth );
  virtual void set3DLLinePattern ( unsigned short pattern );
  virtual void setLineOffsetOn( bool  turnOn );
 
  // Results interface :

  virtual void setResultsOn( bool resultIsOn );
  virtual void selectResultFrame( int frameIdx );

  virtual void addResultFrame( int beforeFrame = -1 ); // frame = -1 => at end
  virtual void deleteResultFrame( int frameIdx = -1 ); // frameidx = -1 => ALL
  virtual void expandFrameArrayIfNeccesary( int frameIdx );

  virtual void setResultLookOn( bool turnOn );
  virtual void remapLookResults();
  virtual bool hasResultLook( unsigned int frameIdx );

  virtual void
  setResultLook( unsigned int frameIdx, lookPolicy lookBinding,
                 std::vector<double> & lookValues,
                 FFaLegendMapper const& mapping );
  virtual void
  deleteResultLook( int frameIdx = -1 );// frameIdx = -1 => all

  //
  // Inventor implementation specific :
  //
  /////////////////////////////////////////////////

  // Control vars
  
  bool IAmOn;
  bool IAmShowingResults;
  bool IAmShowingResultLooks;
  unsigned int    myCurrentFrame;
  FdGpVizModeEnum myVizMode; 
  
  unsigned short myLinePattern;
  int   myLineWidth;
  float myTransparency;
  
  // Convenience methods :
  
  void updateDrawStyle();
  void updateContents();
  void remapLookResults(unsigned int frameIdx, FFaLegendMapper const& mapping);

  // Result frame management :

  struct ResultsFrame
  {
    ResultsFrame() { resColors = 0; resLookPolicy = PR_FACE_VERTEX;}
    ~ResultsFrame(){ eraseAll();}

    void eraseAll();

    SoPackedColor * getResColors();
    
    std::vector<float>   resValues;
    unsigned char        resLookPolicy;
    SoPackedColor*       resColors;
  };

  std::vector<ResultsFrame*> myResultFrames;
    
  // Static node cathalog for shared nodes :
  
  static SoLightModel * ourBaseColorLightModel;

  static SoDrawStyle * ourLW2DrawStyle;
  static SoDrawStyle * ourLW3DrawStyle;

  bool   IAmUsingPrivateDrawStyle;

  static SoPolygonOffset * ourLinesOffset;

  static SoMaterialBinding * ourPrPartMaterialBinding; 
  static SoMaterialBinding * ourPrFaceMaterialBinding;
  static SoMaterialBinding * ourPrVertexMaterialBinding;
  static SoMaterialBinding * ourPrFaceVertexMaterialBinding;
  
  static SoPackedColor * ourNoResultsColor;

  // Node kit definitions :

  SO_KIT_HEADER(FdFEGroupPartKit);  

  SO_KIT_CATALOG_ENTRY_HEADER(toggle);
  SO_KIT_CATALOG_ENTRY_HEADER(backPt);
  SO_KIT_CATALOG_ENTRY_HEADER(separator);

  SO_KIT_CATALOG_ENTRY_HEADER(fullMaterial);
  SO_KIT_CATALOG_ENTRY_HEADER(lightModel);
  SO_KIT_CATALOG_ENTRY_HEADER(drawStyle);
  SO_KIT_CATALOG_ENTRY_HEADER(polygonOffset);

  SO_KIT_CATALOG_ENTRY_HEADER(binding);
  SO_KIT_CATALOG_ENTRY_HEADER(packedMaterial);

  SO_KIT_CATALOG_ENTRY_HEADER(shapeToggle);
   SO_KIT_CATALOG_ENTRY_HEADER(shapeSep);
    SO_KIT_CATALOG_ENTRY_HEADER(shape);
   SO_KIT_CATALOG_ENTRY_HEADER(specialGraphics);
 
};

#ifdef win32
#include <SoWinEnterScope.h>
#endif

#endif
