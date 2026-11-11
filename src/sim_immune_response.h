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
#ifndef SIMIMMUNERESPONSE_SIM_IMMUNE_RESPONSE_H
#define SIMIMMUNERESPONSE_SIM_IMMUNE_RESPONSE_H

#include <array>
#include <string>

#include "enums.h"
#include "location.h"
#include "sim_state.h"

/// The simulation: four locations with sprites, independent of any GUI
class SimImmuneResponse
{
  public:
  SimImmuneResponse();

  bool CanGrabSprite(const EnumLocation location, const int x, const int y) const;

  /// Removes the topmost sprite at the coordinate and returns it.
  /// Returns nullptr if there is nothing to grab
  SpritePtr GrabSprite(const EnumLocation location, const int x, const int y);

  const Sprites& GetSprites(const EnumLocation location) const;

  void AddSprite(const SpritePtr& s, const EnumLocation location);

  /// Add a new cell at the top-center of a location
  void AddSprite(const EnumCellType cellType, const EnumLocation location);

  /// Simulate one timestep
  void Tick();

  /// Feedback to the player on what to do next
  std::string GetFeedback() const;

  Location& GetLocation(const EnumLocation location);
  const Location& GetLocation(const EnumLocation location) const;

  private:
  std::array<Location, 4> mLocations;
  SimState mSimState;
};

#endif // SIMIMMUNERESPONSE_SIM_IMMUNE_RESPONSE_H
