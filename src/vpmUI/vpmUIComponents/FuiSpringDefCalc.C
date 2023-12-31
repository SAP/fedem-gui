// SPDX-FileCopyrightText: 2023 SAP SE
//
// SPDX-License-Identifier: Apache-2.0
//
// This file is part of FEDEM - https://openfedem.org
////////////////////////////////////////////////////////////////////////////////

#include "vpmUI/vpmUIComponents/FuiSpringDefCalc.H"
#include "vpmUI/vpmUIComponents/FuiQueryInputField.H"
#include "FFuLib/FFuIOField.H"
#include "FFuLib/FFuRadioButton.H"
#include "FFuLib/FFuLabelFrame.H"
#include "FFuLib/FFuLabel.H"


FuiSpringDefCalc::FuiSpringDefCalc()
{
  IAmSensitive = true;
  IAmBlockingVariable = false;
  IAmDeflection = true;
  IAmAngular = false;
  IAmSpring = false;
}


void FuiSpringDefCalc::initWidgets()
{
  myVariableField->setInputCheckMode(FFuIOField::DOUBLECHECK);
  myVariableField->setDoubleDisplayMode(FFuIOField::AUTO,8,1);
  myVariableField->setAcceptedCB(FFaDynCB1M(FuiSpringDefCalc,this,
					    onDoubleChanged,double));

  myILengthField->setInputCheckMode(FFuIOField::DOUBLECHECK);
  myILengthField->setDoubleDisplayMode(FFuIOField::AUTO,8,1);
  myILengthField->setAcceptedCB(FFaDynCB1M(FuiSpringDefCalc,this,
					   onDoubleChanged,double));

  myIDeflField->setInputCheckMode(FFuIOField::DOUBLECHECK);
  myIDeflField->setDoubleDisplayMode(FFuIOField::AUTO,8,1);
  myIDeflField->setAcceptedCB(FFaDynCB1M(FuiSpringDefCalc,this,
					 onDoubleChanged,double));

  myILDToggleGroup.insert(myILToggle);
  myILDToggleGroup.insert(myIDToggle);
  myILDToggleGroup.setExclusive(true);
  myILDToggleGroup.setGroupToggleCB(FFaDynCB2M(FuiSpringDefCalc,this,
					       onIntBoolChanged,int,bool));

  myLengthEngineField->setBehaviour(FuiQueryInputField::REF_NONE);
  myLengthEngineField->setRefSelectedCB(FFaDynCB1M(FuiSpringDefCalc,this,
						   onEngineChanged,int));

  this->setLabels(false,true);
}


void FuiSpringDefCalc::placeWidgets(int width, int height)
{
  int fontHeight = myVariableLabel->getFontHeigth();
  int labelWidth = IAmSpring || (IAmAngular > 0 && !IAmDeflection) ? myILToggle->getWidthHint() : myIDToggle->getWidthHint();

  int border      = 6;
  int fieldHeight = 20;
  int fieldSpace  = 10;

  while (11*(fieldHeight + fieldSpace)/2 > height-border)
    if (11*fieldHeight/2 > height-border) {
      fieldHeight = 2*(height-border)/11;
      fieldSpace = 0;
      break;
    }
    else
      fieldSpace--;

  if (fieldHeight < fontHeight) fontHeight = fieldHeight;

  int v1 = border;
  int v2 = v1 + labelWidth + border;
  int v3 = width - border;

  myFrame->setEdgeGeometry(0,width,0,height);

  int line = border/2 + fieldSpace + fieldHeight;
  myVariableLabel->setCenterYGeometry(v1,line,v2-v1,fieldHeight);
  myVariableField->setCenterYGeometry(v2,line,v3-v2,fieldHeight);

  line += fieldSpace + fieldHeight;
  myILToggle->setCenterYGeometry(v1,line,v2-v1,fieldHeight);
  myILengthField->setCenterYGeometry(v2,line,v3-v2,fieldHeight);

  line += fieldSpace + fieldHeight;
  myIDToggle->setCenterYGeometry(v1,line,v2-v1,fieldHeight);
  myIDeflField->setCenterYGeometry(v2,line,v3-v2,fieldHeight);

  line += fieldSpace + fieldHeight;
  myLengthChangeLabel->setCenterYGeometry(v1,line,v3-v1,fontHeight);
  line += fieldSpace + fontHeight;
  myLengthEngineField->setCenterYGeometry(v1,line,v3-v1,fieldHeight);
}


void FuiSpringDefCalc::setChangedCB(const FFaDynCB0& aDynCB)
{
  myChangedCB = aDynCB;
}

void FuiSpringDefCalc::setValuesChangedCB(const FFaDynCB1<FuiSpringDefCalcValues&>& aDynCB)
{
  myValuesChangedCB = aDynCB;
}


void FuiSpringDefCalc::setCBs(const FuiSpringDefCalcValues& values)
{
  myLengthEngineField->setButtonCB(values.myQIFieldButtonCB);
}


void FuiSpringDefCalc::buildDynamicWidgets(const FuiSpringDefCalcValues& values)
{
  IAmBlockingVariable = !values.variableIsEditable;
  if (IAmSensitive)
    myVariableField->setSensitivity(values.variableIsEditable);

  this->setLabels(values.useAngularLabels,values.useSpringLabels);

  if (values.fixedOrFree) {
    myILengthField->popDown();
    myIDeflField->popDown();
    myILToggle->popDown();
    myIDToggle->popDown();
    myLengthChangeLabel->popDown();
    myLengthEngineField->popDown();
  }
  else {
    myILengthField->popUp();
    myIDeflField->popUp();
    myILToggle->popUp();
    myIDToggle->popUp();
    myLengthChangeLabel->popUp();
    myLengthEngineField->popUp();
    myLengthEngineField->setQuery(values.engineQuery);
  }
}


void FuiSpringDefCalc::setValues(const FuiSpringDefCalcValues& values)
{
  this->setLabels(values.useAngularLabels,values.useSpringLabels);
  this->placeWidgets(this->getWidth(),this->getHeight());

  myVariableField->setValue(values.variable);

  double initLength = 0.0;
  double initDeflection = 0.0;
  if (values.useAsDeflection) {
    myIDToggle->setValue(true);
    initLength = values.variable - values.initLengtOrDeflection;
    initDeflection = values.initLengtOrDeflection;
  }
  else {
    myILToggle->setValue(true);
    initLength = values.initLengtOrDeflection;
    if (IAmDeflection)
      initDeflection = values.variable - values.initLengtOrDeflection;
    else // prescribed velocity or acceleration
      initDeflection = 0.0;
  }

  myIDToggle->setSensitivity(IAmSensitive && IAmDeflection);
  myILengthField->setSensitivity(IAmSensitive && !values.useAsDeflection);
  myIDeflField->setSensitivity(IAmSensitive && values.useAsDeflection);

  myILengthField->setValue(initLength);
  myIDeflField->setValue(initDeflection);

  myLengthEngineField->setSelectedRef(values.selectedLengthEngine);
}


void FuiSpringDefCalc::getValues(FuiSpringDefCalcValues& values)
{
  values.variable = myVariableField->getDouble();

  values.useAsDeflection = myIDToggle->getValue();
  if (values.useAsDeflection)
    values.initLengtOrDeflection = myIDeflField->getDouble();
  else
    values.initLengtOrDeflection = myILengthField->getDouble();

  values.selectedLengthEngine = myLengthEngineField->getSelectedRef();
}


void FuiSpringDefCalc::onDoubleChanged(double)
{
  this->onValuesChanged();
}

void FuiSpringDefCalc::onIntBoolChanged(int, bool)
{
  this->onValuesChanged();
}

void FuiSpringDefCalc::onEngineChanged(int)
{
  this->onValuesChanged();
}


void FuiSpringDefCalc::onValuesChanged()
{
  FuiSpringDefCalcValues values;
  this->getValues(values);
  myValuesChangedCB.invoke(values);
  myChangedCB.invoke();
}


void FuiSpringDefCalc::setSensitivity(bool isSensitive)
{
  IAmSensitive = isSensitive;

  myIDToggle->setSensitivity(isSensitive && IAmDeflection);
  myILToggle->setSensitivity(isSensitive);
  myILengthField->setSensitivity(isSensitive && !myIDToggle->getValue());
  myIDeflField->setSensitivity(isSensitive && myIDToggle->getValue());

  myLengthEngineField->setSensitivity(isSensitive);

  if (!IAmBlockingVariable)
    myVariableField->setSensitivity(isSensitive);
}


void FuiSpringDefCalc::setLabels(char isAngle, bool isSpring)
{
  if (isAngle == IAmAngular && isSpring == IAmSpring) return;

  IAmDeflection = isAngle == 0 || isAngle == 1;
  IAmAngular = isAngle;
  IAmSpring = isSpring;

  if (IAmSpring) // Spring controlled motion
  {
    if (IAmAngular > 0) {
      myFrame->setLabel("Stress free angle control");
      myVariableLabel->setLabel("Angle in model");
      myILToggle->setLabel("Constant stress free angle");
      myIDToggle->setLabel("Constant deflection");
      myLengthChangeLabel->setLabel("Stress free angle change");
    }
    else {
      myFrame->setLabel("Stress free length control");
      myVariableLabel->setLabel("Length in model");
      myILToggle->setLabel("Constant stress free length");
      myIDToggle->setLabel("Constant deflection");
      myLengthChangeLabel->setLabel("Stress free length change");
    }
  }
  else         // Prescribed motion
  {
    if (IAmAngular > 0) {
      myFrame->setLabel("Rotation control");
      myVariableLabel->setLabel("Angle in model");
      myIDToggle->setLabel("Constant rotation");
    }
    else {
      myFrame->setLabel("Translation control");
      myVariableLabel->setLabel("Length in model");
      myIDToggle->setLabel("Constant displacement");
    }

    switch (IAmAngular) {
    case 0:
      myILToggle->setLabel("Constant length");
      myLengthChangeLabel->setLabel("Length change");
      break;
    case 1:
      myILToggle->setLabel("Constant angle");
      myLengthChangeLabel->setLabel("Angle change");
      break;
    case  2:
    case -1:
      myILToggle->setLabel("Constant velocity");
      myLengthChangeLabel->setLabel("Velocity change");
      break;
    case  3:
    case -2:
      myILToggle->setLabel("Constant acceleration");
      myLengthChangeLabel->setLabel("Acceleration change");
      break;
    }
  }
}
