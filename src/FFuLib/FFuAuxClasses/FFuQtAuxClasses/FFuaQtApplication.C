// SPDX-FileCopyrightText: 2023 SAP SE
//
// SPDX-License-Identifier: Apache-2.0
//
// This file is part of FEDEM - https://openfedem.org
////////////////////////////////////////////////////////////////////////////////

#include <QApplication>
#include <QDesktopWidget>
#include <QSplashScreen>
#include <QStyleFactory>
#include <QScreen>
#include <QTimer>
#ifdef FFU_DEBUG
#include <iostream>
#endif

#include "FFuLib/FFuBase/FFuComponentBase.H"
#include "FFuLib/FFuQtBaseClasses/FFuQtTopLevelShell.H"
#include "FFuLib/FFuAuxClasses/FFuQtAuxClasses/FFuaQtEventBlocker.H"
#include "FFuLib/FFuAuxClasses/FFuaApplication.H"
#include "FFuLib/Icons/splashBase.xpm"
#include "FFuLib/Icons/splashWind.xpm"
#include "FFuLib/Icons/splashOffshore.xpm"

QSplashScreen* FFuaApplication::splashScreen = NULL;
QTimer* FFuaApplication::splashTimer = NULL;
bool FFuaApplication::isBlocked = false;

static FFuaQtEventBlocker applicationblocker;
// Workaround Qt bug (see TT#2679)
// Qt4 extern Q_EXPORT int qt_ntfs_permission_lookup;

////////////////////////////////////////////////////////////////////////
//
//  Initialisation routine.
//

void FFuaApplication::init(int argc, char** argv, bool useGUI)
{
  // Workaround Qt bug (see TT#2679)
  // Qt4 qt_ntfs_permission_lookup = 0;
  new QApplication(argc,argv,useGUI);
  FFuQtTopLevelShell::initQtBlocking();

  if (useGUI)
  {
    // Setting a MS Windows look on all platforms
#if defined(win32) || defined(win64)
    QStyle* style = QStyleFactory::create("WindowsVista");
#else
    QStyle* style = QStyleFactory::create("Plastique");
#endif
    QApplication::setStyle(style);

    splashScreen = new QSplashScreen(QPixmap(splashBase_xpm));
    splashScreen->show();
  }

  qApp->installEventFilter(&applicationblocker);
}


bool FFuaApplication::splashMessage(const char* msg)
{
  if (!splashScreen) return false;

  splashScreen->showMessage(msg, Qt::AlignTop | Qt::AlignHCenter);
  return true;
}

void FFuaApplication::splashScreenWindpower()
{
  if (!splashScreen) return;

  splashScreen->setPixmap(QPixmap(splashWind_xpm));
}

void FFuaApplication::splashScreenOffshore()
{
  if (!splashScreen) return;

  splashScreen->setPixmap(QPixmap(splashOffshore_xpm));
}


int FFuaApplication::getScreenHeight()
{
  return qApp->desktop()->screenGeometry(qApp->desktop()->primaryScreen()).height();
}

int FFuaApplication::getScreenWidth()
{
  return qApp->desktop()->screenGeometry(qApp->desktop()->primaryScreen()).width();
}


////////////////////////////////////////////////////////////////////////
//
// Event handling :
//

void FFuaApplication::handlePendingEvents()
{
  qApp->processEvents();
}

int FFuaApplication::mainLoop()
{
  if (splashScreen)
  {
    splashTimer = new QTimer();
    splashTimer->setSingleShot(true);
    splashTimer->setInterval(1500);
    QObject::connect(splashTimer, SIGNAL(timeout()), splashScreen, SLOT(close()));
    splashTimer->start();
  }
  return qApp->exec();
}


void FFuaApplication::blockUserEvents(bool doBlock)
{
  if (doBlock == isBlocked) return;

#ifdef FFU_DEBUG
  std::cout << (doBlock ? "B" : "Unb") <<"locking the GUI"<< std::endl;
#endif
  applicationblocker.block(doBlock);
  isBlocked = doBlock;
}


void FFuaApplication::breakUserEventBlock(bool doBreak)
{
  if (!isBlocked) return;

#ifdef FFU_DEBUG
  std::cout << (doBreak ? "B" : "Unb") <<"reaking the GUI block"<< std::endl;
#endif
  applicationblocker.breakBlock(doBreak);
}
