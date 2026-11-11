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
#include "sim_widget.h"

#include <QFont>
#include <QMouseEvent>
#include <QPainter>

#include <algorithm>

#include "sprite_images.h"

namespace {

constexpr int paneWidth = static_cast<int>(Sprite::maxx);
constexpr int paneHeight = static_cast<int>(Sprite::maxy);

QColor GetBackgroundColor(const EnumLocation location)
{
  switch (location)
  {
    case EnumLocation::bloodStream: return QColor(255, 196, 196);
    case EnumLocation::boneMarrow: return QColor(255, 196, 255);
    case EnumLocation::cellTissue: return QColor(196, 196, 196);
    case EnumLocation::lymphNode: return QColor(255, 255, 196);
  }
  return Qt::white;
}

} // namespace

SimWidget::SimWidget(QWidget* parent)
  : QWidget(parent)
{
  qRegisterMetaType<EnumCellType>("EnumCellType");
  setFixedSize(sizeHint());
  setMouseTracking(true);
  setAttribute(Qt::WA_OpaquePaintEvent);
  CreateBackgrounds();

  connect(&mTimer, &QTimer::timeout, this, [this]()
  {
    mSim.Tick();
    update();
  });
  mTimer.start(10);
}

QSize SimWidget::sizeHint() const
{
  return QSize(nCols * paneWidth, nRows * paneHeight);
}

void SimWidget::CreateBackgrounds()
{
  QFont font("Courier New");
  font.setStyleHint(QFont::TypeWriter);
  font.setPixelSize(32);
  for (const auto location: GetAllLocations())
  {
    QPixmap& p = mBackgrounds[static_cast<std::size_t>(location)];
    p = QPixmap(paneWidth, paneHeight);
    p.fill(GetBackgroundColor(location));
    QPainter painter(&p);
    painter.setFont(font);
    painter.setPen(Qt::black);
    painter.drawText(p.rect(), Qt::AlignCenter,
      QString::fromStdString(LocationToString(location)));
  }
}

EnumLocation SimWidget::GetLocation(const QPoint& pos)
{
  const bool left = pos.x() < paneWidth;
  const bool top = pos.y() < paneHeight;
  if (top) return left ? EnumLocation::boneMarrow : EnumLocation::cellTissue;
  return left ? EnumLocation::bloodStream : EnumLocation::lymphNode;
}

QPoint SimWidget::GetOrigin(const EnumLocation location)
{
  switch (location)
  {
    case EnumLocation::boneMarrow: return QPoint(0, 0);
    case EnumLocation::cellTissue: return QPoint(paneWidth, 0);
    case EnumLocation::bloodStream: return QPoint(0, paneHeight);
    case EnumLocation::lymphNode: return QPoint(paneWidth, paneHeight);
  }
  return QPoint(0, 0);
}

QPoint SimWidget::Clamp(const QPoint& pos) const
{
  return QPoint(
    std::clamp(pos.x(), 0, width() - 1),
    std::clamp(pos.y(), 0, height() - 1));
}

void SimWidget::AddCell(const EnumCellType cellType)
{
  mSim.AddSprite(cellType, EnumLocation::boneMarrow);
}

void SimWidget::Restart()
{
  mDragged.reset();
  mSim = SimImmuneResponse();
  update();
}

void SimWidget::paintEvent(QPaintEvent*)
{
  QPainter painter(this);
  for (const auto location: GetAllLocations())
  {
    const QPoint origin = GetOrigin(location);
    painter.save();
    painter.setClipRect(QRect(origin, QSize(paneWidth, paneHeight)));
    painter.translate(origin);
    painter.drawPixmap(0, 0, mBackgrounds[static_cast<std::size_t>(location)]);
    for (const auto& s: mSim.GetSprites(location))
    {
      painter.drawPixmap(
        static_cast<int>(s->GetX()),
        static_cast<int>(s->GetY()),
        GetPixmap(s->GetCellType()));
    }
    painter.restore();
  }
  if (mDragged)
  {
    painter.drawPixmap(mDragPos, GetPixmap(mDragged->GetCellType()));
  }
}

void SimWidget::mousePressEvent(QMouseEvent* event)
{
  if (event->button() != Qt::LeftButton || mDragged) return;
  const QPoint pos = Clamp(event->pos());
  const EnumLocation location = GetLocation(pos);
  const QPoint local = pos - GetOrigin(location);
  mDragged = mSim.GrabSprite(location, local.x(), local.y());
  if (!mDragged) return;
  mDragPos = pos;
  emit cellGrabbed(mDragged->GetCellType());
  update();
}

void SimWidget::mouseMoveEvent(QMouseEvent* event)
{
  if (!mDragged) return;
  mDragPos = Clamp(event->pos());
  update();
}

void SimWidget::mouseReleaseEvent(QMouseEvent* event)
{
  if (event->button() != Qt::LeftButton || !mDragged) return;
  const QPoint pos = Clamp(event->pos());
  const EnumLocation location = GetLocation(pos);
  const QPoint local = pos - GetOrigin(location);
  mDragged->SetX(local.x());
  mDragged->SetY(local.y());
  mSim.AddSprite(mDragged, location);
  mDragged.reset();
  update();
}
