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
#include "sprite.h"

#include <cstdlib>
#include <stdexcept>

Sprite::Sprite(const EnumCellType cellType, const double x, const double y)
  : mCellType{cellType}, mX{x}, mY{y}
{
}

bool Sprite::CanGrab(const int x, const int y) const
{
  return x >= mX
    && y >= mY
    && x < mX + width
    && y < mY + height;
}

void Sprite::Move()
{
  Move((GetRandomUniform() * 2.0 * speed) - speed, GetRandomUniform() * speed);
}

void Sprite::Move(const double dx, const double dy)
{
  mX += dx;
  mY += dy;
  if (mX + width > maxx) mX -= speed;
  else if (mX < 0) mX += speed;
  if (mY > maxy)
  {
    // -2.0 so it can be detected before actually appearing on screen
    mY = -height - 2.0;
  }
}

std::optional<EnumLocation> Sprite::WantsToMigrateTo() const
{
  return std::nullopt;
}

SpriteActiveCell::SpriteActiveCell(
  const EnumCellType cellType, const double x, const double y)
  : Sprite(cellType, x, y),
    mGoLeft{GetRandomInt() % 2 == 0}
{
}

void SpriteActiveCell::Move()
{
  const double dx = GetRandomUniform() * speed * 2.0 * (mGoLeft ? 1.0 : -1.0);
  const double dy = GetRandomUniform() * speed * 2.0;

  double x = GetX() + dx;
  double y = GetY() + dy;

  if      (x + width > maxx) { x -= speed; mGoLeft = false; }
  else if (x         < 0   ) { x += speed; mGoLeft = true;  }
  if (y > maxy)
  {
    // -2.0 so it can be detected before actually appearing on screen
    y = -height - 2.0;
  }
  SetX(x);
  SetY(y);
}

void SpriteTissueCell::Move()
{
  Sprite::Move(0.0, speed / 10.0);
}

std::optional<EnumLocation> SpriteMolecule::WantsToMigrateTo() const
{
  if (GetY() >= -height) return std::nullopt;
  switch (GetRandomInt() % 3)
  {
    case 0: return EnumLocation::bloodStream;
    case 1: return EnumLocation::cellTissue;
    default: return EnumLocation::lymphNode;
  }
}

SpriteVirus::SpriteVirus(const double x, const double y)
  : Sprite(EnumCellType::virus, x, y)
{
}

std::optional<EnumLocation> SpriteVirus::WantsToMigrateTo() const
{
  if (GetY() >= -height) return std::nullopt;
  return GetRandomInt() % 2 == 0
    ? EnumLocation::bloodStream
    : EnumLocation::cellTissue;
}

std::shared_ptr<Sprite> CreateSprite(
  const EnumCellType cellType, const double x, const double y)
{
  using C = EnumCellType;
  switch (cellType)
  {
    case C::activatedBcell:
    case C::activatedCytotoxicTcell:
    case C::activatedThelperCell:
    case C::macrophageWithoutVirusParticles:
    case C::macrophageWithVirusParticles:
      return std::make_shared<SpriteActiveCell>(cellType, x, y);
    case C::antibody:
    case C::cytokine:
      return std::make_shared<SpriteMolecule>(cellType, x, y);
    case C::bCell:
    case C::cytotoxicTcell:
    case C::tHelperCell:
      return std::make_shared<Sprite>(cellType, x, y);
    case C::cellHealthy:
    case C::cellInfected:
      return std::make_shared<SpriteTissueCell>(cellType, x, y);
    case C::virus:
      return std::make_shared<SpriteVirus>(x, y);
  }
  throw std::domain_error("Unknown cell type");
}

double GetRandomUniform()
{
  return static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX);
}

int GetRandomInt()
{
  return std::rand() >> 4;
}
