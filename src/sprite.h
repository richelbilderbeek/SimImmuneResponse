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
#ifndef SIMIMMUNERESPONSE_SPRITE_H
#define SIMIMMUNERESPONSE_SPRITE_H

#include <memory>
#include <optional>

#include "enums.h"

/// A cell or molecule moving around in a location.
/// Sprites are pure simulation objects: they know nothing about
/// how they are drawn (see SpriteImages for that).
class Sprite
{
  public:
  Sprite(const EnumCellType cellType, const double x, const double y);
  virtual ~Sprite() = default;
  Sprite(const Sprite&) = delete;
  Sprite& operator=(const Sprite&) = delete;

  EnumCellType GetCellType() const noexcept { return mCellType; }

  /// Can the player grab this sprite at the coordinate?
  virtual bool CanGrab(const int x, const int y) const;

  /// Move the sprite for one timestep
  virtual void Move();
  void Move(const double dx, const double dy);

  /// The location the sprite wants to move to, if any
  virtual std::optional<EnumLocation> WantsToMigrateTo() const;

  double GetX() const noexcept { return mX; }
  double GetY() const noexcept { return mY; }
  void SetX(const double x) noexcept { mX = x; }
  void SetY(const double y) noexcept { mY = y; }

  /// Every sprite has the same size, in pixels
  static constexpr int width = 32;
  static constexpr int height = 32;

  /// The size of a location, in pixels
  static constexpr double maxx = 320.0;
  static constexpr double maxy = 224.0;
  static constexpr double speed = 1.0;

  private:
  const EnumCellType mCellType;
  double mX;
  double mY;
};

/// A cell that moves around actively, left and right
class SpriteActiveCell : public Sprite
{
  public:
  SpriteActiveCell(const EnumCellType cellType, const double x, const double y);
  void Move() override;

  private:
  bool mGoLeft;
};

/// A tissue cell: cannot be grabbed and moves down slowly
class SpriteTissueCell : public Sprite
{
  public:
  using Sprite::Sprite;
  bool CanGrab(const int, const int) const override { return false; }
  void Move() override;
};

/// A molecule (antibody or cytokine): cannot be grabbed
/// and flows to all locations except the bone marrow
class SpriteMolecule : public Sprite
{
  public:
  using Sprite::Sprite;
  bool CanGrab(const int, const int) const override { return false; }
  std::optional<EnumLocation> WantsToMigrateTo() const override;
};

/// A virus: cannot be grabbed and flows between
/// the blood stream and cell tissue
class SpriteVirus : public Sprite
{
  public:
  SpriteVirus(const double x, const double y);
  bool CanGrab(const int, const int) const override { return false; }
  std::optional<EnumLocation> WantsToMigrateTo() const override;
};

/// Create a sprite of the right type
std::shared_ptr<Sprite> CreateSprite(
  const EnumCellType cellType, const double x, const double y);

/// Random number in range [0.0, 1.0]
double GetRandomUniform();

/// Random non-negative integer, as used throughout the simulation
int GetRandomInt();

#endif // SIMIMMUNERESPONSE_SPRITE_H
