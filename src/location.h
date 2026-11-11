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
#ifndef SIMIMMUNERESPONSE_LOCATION_H
#define SIMIMMUNERESPONSE_LOCATION_H

#include <list>
#include <memory>
#include <utility>

#include "enums.h"
#include "sim_state.h"
#include "sprite.h"

using SpritePtr = std::shared_ptr<Sprite>;
using Sprites = std::list<SpritePtr>;
using Migrants = std::list<std::pair<EnumLocation, SpritePtr>>;

/// A location in the body, such as the blood stream, containing sprites
class Location
{
  public:
  /// Creates a location with its initial inhabitants
  explicit Location(const EnumLocation location);

  EnumLocation GetLocation() const noexcept { return mLocation; }
  const Sprites& GetSprites() const noexcept { return mSprites; }

  void AddSprite(const SpritePtr& s);
  bool CanGrabSprite(const int x, const int y) const;

  /// Removes the topmost sprite at the coordinate and returns it.
  /// Returns nullptr if there is nothing to grab
  SpritePtr GrabSprite(const int x, const int y);

  bool IsPresent(const EnumCellType cellType) const;
  int Count(const EnumCellType cellType) const;

  /// Move the sprites that want to leave to the emigrants
  void RemoveEmigrants(Migrants& emigrants);

  /// Add the immigrants that want to come to this location
  void AddMigrants(const Migrants& immigrants);

  /// Location-specific events, e.g. the blood stream adding new viruses
  void RespondToSimState();

  /// Move all sprites and let them interact
  void MoveAllSprites();

  void UpdateSimState(SimState& state) const;

  private:
  EnumLocation mLocation;
  Sprites mSprites;

  void LetSpritesAct();
  void LetSpritesInteract();
};

/// Are the two sprites near enough to interact?
bool AreNear(const Sprite& a, const Sprite& b);

#endif // SIMIMMUNERESPONSE_LOCATION_H
