// SPDX-FileCopyrightText: 2023 SAP SE
//
// SPDX-License-Identifier: Apache-2.0
//
// This file is part of FEDEM - https://openfedem.org
////////////////////////////////////////////////////////////////////////////////

#ifndef FD_SEA_STATE_H
#define FD_SEA_STATE_H

#include "vpmDisplay/FdBase.H"
#include "vpmDisplay/FdAnimatedBase.H"

class FmSeaState;
class FmMathFuncBase;
class FdAnimateModel;


class FdSeaState : public FdObject, public FdAnimatedBase
{
  Fmd_HEADER_INIT();

public:
  FdSeaState(FmSeaState* pt);

  virtual bool updateFdTopology(bool = true) { return this->updateFdCS(); }
  virtual bool updateFdDetails();
  virtual bool updateFdApperance();
  virtual bool updateFdCS();

  virtual void initAnimation() {}
  virtual void selectAnimationFrame(size_t frameNr);
  virtual void resetAnimation()	{ this->selectAnimationFrame(0); }
  virtual void deleteAnimationData() {}

  void showWaves(bool doShow) { bShowWaves = doShow; }

protected:
  virtual ~FdSeaState();

  virtual SoNodeKitListPart* getListSw() const;

  virtual void showHighlight();
  virtual void hideHighlight();

  bool evaluateWave(FmSeaState* seaState, FmMathFuncBase* waveFunc,
                    FdAnimateModel* animator = NULL) const;

private:
  void* highlightBoxId;
  bool  bShowWaves;
};

#endif
