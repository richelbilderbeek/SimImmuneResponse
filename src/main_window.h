/*
  SimImmuneResponse. Active simulation about the human immune response
  Copyright (C) 2008-2026  Richel Bilderbeek

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef SIMIMMUNERESPONSE_MAIN_WINDOW_H
#define SIMIMMUNERESPONSE_MAIN_WINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QTextEdit>
#include <QTimer>

#include "enums.h"

class SimWidget;

/// Shows a cell type enlarged, with its name
class InfoWidget : public QWidget
{
  Q_OBJECT

  public:
  explicit InfoWidget(QWidget* parent = nullptr);
  void SetCellType(const EnumCellType cellType);

  protected:
  void paintEvent(QPaintEvent* event) override;

  private:
  bool mHasCellType{false};
  EnumCellType mCellType{EnumCellType::virus};
};

/// The main window: the simulation, a legend, buttons,
/// the most recently grabbed cell and feedback
class MainWindow : public QMainWindow
{
  Q_OBJECT

  public:
  explicit MainWindow(QWidget* parent = nullptr);

  SimWidget* GetSimWidget() const noexcept { return mSimWidget; }

  private:
  SimWidget* mSimWidget;
  InfoWidget* mInfo;
  QTextEdit* mFeedback;
  QTimer mFeedbackTimer;

  QWidget* CreateLegend();
  QWidget* CreateOptions();
  void UpdateFeedback();
  void ShowAbout();
};

#endif // SIMIMMUNERESPONSE_MAIN_WINDOW_H
