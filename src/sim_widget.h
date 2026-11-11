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
#ifndef SIMIMMUNERESPONSE_SIM_WIDGET_H
#define SIMIMMUNERESPONSE_SIM_WIDGET_H

#include <QPixmap>
#include <QPoint>
#include <QTimer>
#include <QWidget>

#include <array>

#include "sim_immune_response.h"

/// Shows the four locations and lets the player drag cells between them.
///
/// Layout:
///   bone marrow  | cell tissue
///   blood stream | lymph node
class SimWidget : public QWidget
{
  Q_OBJECT

  public:
  explicit SimWidget(QWidget* parent = nullptr);

  /// Produce a new cell in the bone marrow
  void AddCell(const EnumCellType cellType);

  void Restart();

  const SimImmuneResponse& GetSim() const noexcept { return mSim; }

  /// Which location is at this widget coordinate?
  static EnumLocation GetLocation(const QPoint& pos);

  /// The top-left corner of a location, in widget coordinates
  static QPoint GetOrigin(const EnumLocation location);

  static constexpr int nCols = 2;
  static constexpr int nRows = 2;

  QSize sizeHint() const override;

  signals:
  /// Emitted when the player picks up a cell
  void cellGrabbed(EnumCellType cellType);

  protected:
  void paintEvent(QPaintEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;

  private:
  SimImmuneResponse mSim;
  QTimer mTimer;

  /// The sprite being dragged by the player, if any
  SpritePtr mDragged;
  QPoint mDragPos;

  std::array<QPixmap, 4> mBackgrounds;

  void CreateBackgrounds();
  QPoint Clamp(const QPoint& pos) const;
};

Q_DECLARE_METATYPE(EnumCellType)

#endif // SIMIMMUNERESPONSE_SIM_WIDGET_H
