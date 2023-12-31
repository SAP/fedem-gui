// SPDX-FileCopyrightText: 2023 SAP SE
//
// SPDX-License-Identifier: Apache-2.0
//
// This file is part of FEDEM - https://openfedem.org
////////////////////////////////////////////////////////////////////////////////

#include <QResizeEvent>
#include <QCloseEvent>
#include <QEvent>

#include "FFuLib/FFuAuxClasses/FFuaApplication.H"
#include "FFuQtModalDialog.H"


FFuQtModalDialog::FFuQtModalDialog(int xpos, int ypos,
				   int width, int height,
				   const char* title,
				   const char* name,
				   Qt::WindowFlags f)
  : QDialog(NULL,f)
{
  this->setObjectName(name);
  this->setModal(true);
  this->setWidget(this);
  this->setWindowTitle(title);
  this->setGeometry(xpos,ypos,width,height);
}


bool FFuQtModalDialog::execute()
{
  FFuaApplication::breakUserEventBlock(true);
  int ret = this->exec();
  FFuaApplication::breakUserEventBlock(false);

  return (ret == QDialog::Accepted);
}


void FFuQtModalDialog::closeDialog(bool resultCode)
{
  this->done(resultCode ? QDialog::Accepted : QDialog::Rejected);
}


void FFuQtModalDialog::resizeEvent(QResizeEvent* rEvent)
{
  this->QWidget::resizeEvent(rEvent);
  this->placeWidgets(this->width(),this->height());
}


void FFuQtModalDialog::closeEvent(QCloseEvent* cEvent)
{
  if (this->onClose())
    cEvent->accept();
  else
    cEvent->ignore();
}


bool FFuQtModalDialog::event(QEvent* anyEvent)
{
  bool retVal = this->QWidget::event(anyEvent);
  switch (anyEvent->type())
    {
    case QEvent::Show:
      this->onPoppedUp();
      return true;
    case QEvent::Hide:
      this->onPoppedDown();
      return true;
    default:
      break;
    }

  return retVal;
}
