// SPDX-FileCopyrightText: 2023 SAP SE
//
// SPDX-License-Identifier: Apache-2.0
//
// This file is part of FEDEM - https://openfedem.org
////////////////////////////////////////////////////////////////////////////////

#ifndef FFU_SCROLLED_LIST_DIALOG_H
#define FFU_SCROLLED_LIST_DIALOG_H

#include "FFuLib/FFuBase/FFuTopLevelShell.H"
#include "FFaLib/FFaDynCalls/FFaDynCB.H"

class FFuScrolledList;
class FFuDialogButtons;
class FFuLabel;


class FFuScrolledListDialog : virtual public FFuTopLevelShell
{
public:
  FFuScrolledListDialog() {}
  virtual ~FFuScrolledListDialog() {}

  int getItem() const;

  void setOkButtonClickedCB(const FFaDynCB1<int>& aDynCB);
  void setCancelButtonClickedCB(const FFaDynCB1<int>& aDynCB);
  void setApplyButtonClickedCB(const FFaDynCB1<int>& aDynCB);

  void setOkButtonClickedCB(const FFaDynCB2<int,FFuComponentBase*>& aDynCB);
  void setCancelButtonClickedCB(const FFaDynCB2<int,FFuComponentBase*>& aDynCB);
  void setApplyButtonClickedCB(const FFaDynCB2<int,FFuComponentBase*>& aDynCB);

  FFuDialogButtons* myDialogButtons;
  FFuScrolledList* myItemSelector;
  FFuLabel* labNotesImage;
  FFuLabel* labNotesLabel;
  FFuLabel* labNotesText;
  bool showNotes;

protected:
  void onDlgButtonClicked(int);
  void callCancelButtonClickedCB();
  void callOkButtonClickedCB();
  void callApplyButtonClickedCB();

  // widget placement - from MuitiUIComponent
  virtual void placeWidgets(int width, int height);

  // Initialisation to be called by UILib dep. subclass
  void init();

protected:
  FFaDynCB1<int> myOkButtonClickedCB;
  FFaDynCB1<int> myCancelButtonClickedCB;
  FFaDynCB1<int> myApplyButtonClickedCB;
  FFaDynCB1<int> myItemChangedCB;

  FFaDynCB2<int,FFuComponentBase*> myOkButtonClickedWPtrCB;
  FFaDynCB2<int,FFuComponentBase*> myCancelButtonClickedWPtrCB;
  FFaDynCB2<int,FFuComponentBase*> myApplyButtonClickedWPtrCB;
  FFaDynCB2<int,FFuComponentBase*> myItemChangedWPtrCB;
};

#endif
