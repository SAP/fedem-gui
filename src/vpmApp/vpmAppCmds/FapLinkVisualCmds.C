// SPDX-FileCopyrightText: 2023 SAP SE
//
// SPDX-License-Identifier: Apache-2.0
//
// This file is part of FEDEM - https://openfedem.org
////////////////////////////////////////////////////////////////////////////////

#include "FapLinkVisualCmds.H"
#include "FFuLib/FFuAuxClasses/FFuaCmdItem.H"
#include "vpmUI/Fui.H"
#include "vpmUI/Icons/FuiIconPixmaps.H"
#include "vpmDB/FmDB.H"
#include "vpmDB/FmPart.H"
#include "vpmDB/FmSubAssembly.H"
#include "vpmDB/FmElementGroupProxy.H"
#include "vpmPM/FpPM.H"
#include "FFlLib/FFlFEParts/FFlVDetail.H"


void FapLinkVisualCmds::init()
{
  FFuaCmdItem* cmdItem ;

  cmdItem = new FFuaCmdItem("cmdId_LinkSelection_hide");
  cmdItem->setSmallIcon(lineViewObj_xpm);
  cmdItem->setText("Hide Part Faces");
  cmdItem->setToolTip("Hide the element/geometry faces in the selected part/group");
  cmdItem->setActivatedCB(FFaDynCB0S(FapLinkVisualCmds::hideLinkSelection));
  cmdItem->setGetSensitivityCB(FFaDynCB1S(FapLinkVisualCmds::getElmFaceHideShowSensitivity,bool&));

  cmdItem = new FFuaCmdItem("cmdId_LinkSelection_show");
  cmdItem->setSmallIcon(solidViewObj_xpm);
  cmdItem->setText("Show Part Faces");
  cmdItem->setToolTip("Show the element/geometry faces in the selected part/group");
  cmdItem->setActivatedCB(FFaDynCB0S(FapLinkVisualCmds::showLinkSelection));
  cmdItem->setGetSensitivityCB(FFaDynCB1S(FapLinkVisualCmds::getElmFaceHideShowSensitivity,bool&));

  cmdItem = new FFuaCmdItem("cmdId_SubassemblySelection_show");
  cmdItem->setText("Show Subassembly Parts");
  cmdItem->setToolTip("Show the parts in the selected subassembly");
  cmdItem->setActivatedCB(FFaDynCB0S(FapLinkVisualCmds::showSubassemblySelection));
  cmdItem->setGetSensitivityCB(FFaDynCB1S(FapCmdsBase::alwaysSensitive,bool&));

  cmdItem = new FFuaCmdItem("cmdId_SubassemblySelection_hide");
  cmdItem->setText("Hide Subassembly Parts");
  cmdItem->setToolTip("Hides the parts in the selected subassembly");
  cmdItem->setActivatedCB(FFaDynCB0S(FapLinkVisualCmds::hideSubassemblySelection));
  cmdItem->setGetSensitivityCB(FFaDynCB1S(FapCmdsBase::alwaysSensitive,bool&));
}


void FapLinkVisualCmds::hideLinkSelection()
{
  FapLinkVisualCmds::setSelectionDetail(FFlVDetail::OFF);
}


void FapLinkVisualCmds::showLinkSelection()
{
  FapLinkVisualCmds::setSelectionDetail(FFlVDetail::ON);
}


void FapLinkVisualCmds::setSelectionDetail(int detailType)
{
  Fui::noUserInputPlease();

  FapPartGroupsMap data;
  FapCmdsBase::getSelectedPartsWithGroups(data);

  for (const FapPartGroupsMap::value_type& group : data)
    if (group.first->setVisDetail(group.second,detailType))
      group.first->onChanged();

  Fui::okToGetUserInput();
}


void FapLinkVisualCmds::showSubassemblySelection()
{
  std::vector<FmSubAssembly*> selection;
  FapCmdsBase::getSelected(selection);

  for (FmSubAssembly* subass : selection)
  {
    std::vector<FmLink*> parts;
    FmDB::getAllLinks(parts,subass);
    for (FmLink* link : parts)
      if (link->setModelType(FmLink::SURFACE) ||
          link->setMeshType(FmLink::REDUCED))
        FpPM::touchModel();
  }
}

void FapLinkVisualCmds::hideSubassemblySelection()
{
  std::vector<FmSubAssembly*> selection;
  FapCmdsBase::getSelected(selection);

  for (FmSubAssembly* subass : selection)
  {
    std::vector<FmLink*> parts;
    FmDB::getAllLinks(parts, subass);
    for (FmLink* link : parts)
      if (link->setModelType(FmLink::OFF) ||
          link->setMeshType(FmLink::OFF))
        FpPM::touchModel();
  }
}


void FapLinkVisualCmds::getElmFaceHideShowSensitivity(bool& isSensitive)
{
  isSensitive = false;
  std::vector<FmModelMemberBase*> selection;
  FapCmdsBase::getCurrentSelection(selection);

  for (size_t i = 0; i < selection.size() && !isSensitive; i++)
  {
    int dLevel = FmLink::OFF;
    if (selection[i]->isOfType(FmLink::getClassTypeID()))
      dLevel = ((FmLink*)selection[i])->getModelType();
    else if (selection[i]->isOfType(FmElementGroupProxy::getClassTypeID()))
      if (((FmElementGroupProxy*)selection[i])->getOwner())
	dLevel = ((FmElementGroupProxy*)selection[i])->getOwner()->getModelType();

    isSensitive = (dLevel == FmLink::FULL || dLevel == FmLink::SURFACE || dLevel == FmLink::OFF);
  }
}
