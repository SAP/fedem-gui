/* SPDX-FileCopyrightText: 2023 SAP SE
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * This file is part of FEDEM - https://openfedem.org
 */
/*
 * AirfoilSelector.H
 *
 *  Created on: Jul 4, 2012
 *      Author: runarhr
 */

#ifndef _AIRFOIL_SELECTOR_H
#define _AIRFOIL_SELECTOR_H

#include <QWidget>

class QLabel;
class QComboBox;
class QRadioButton;
class QPushButton;
class QLineEdit;
class QVBoxLayout;
class QHBoxLayout;
class QAbstractItemView;
class AirfoilSelectionModel;


class AirfoilSelector : public QWidget
{
  Q_OBJECT

public:
  AirfoilSelector(const std::string& instPath, QWidget* parent);
  virtual ~AirfoilSelector() {}

  QAbstractItemView* getView();
  QComboBox* getComboBox() { return apAvailableAirfoilsPullDown; }
  AirfoilSelectionModel* getModel() { return apAirfoilSelectionModel; }

  QString getCurrentItem() const;
  QString getCurrentFolder() const;

  void selectItem(const QString& value);
  void addEntry(const QString& fullPath);
  void removeEntry(const QString& fullPath);
  void refresh();

private:
  QLabel* apBanner;
  QComboBox* apAvailableAirfoilsPullDown;
  QRadioButton* apCustomFolderRadioButton;
  QRadioButton* apDefaultFolderRadioButton;
  QPushButton* apBrowseButton;
  QLineEdit* apPathField;

  AirfoilSelectionModel* apAirfoilSelectionModel;

  QVBoxLayout* apMainLayout;
  QVBoxLayout* apButtonLayout;
  QHBoxLayout* apFolderLayout;

  QString customFolder;
  QString standardFolder;

public slots:
  void customFolderBrowse();
  void setLibrary(QString path, bool locked);
  void radioButtonsChanged();

signals:
  void selectionModelChanged();
};

#endif
