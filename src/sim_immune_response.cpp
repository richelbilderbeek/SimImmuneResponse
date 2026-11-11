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
#include "sim_immune_response.h"

#include <cassert>

SimImmuneResponse::SimImmuneResponse()
  : mLocations{{
      Location(EnumLocation::bloodStream),
      Location(EnumLocation::boneMarrow),
      Location(EnumLocation::cellTissue),
      Location(EnumLocation::lymphNode)
    }}
{
  for (const auto& location: mLocations) location.UpdateSimState(mSimState);
}

Location& SimImmuneResponse::GetLocation(const EnumLocation location)
{
  return mLocations[static_cast<std::size_t>(location)];
}

const Location& SimImmuneResponse::GetLocation(const EnumLocation location) const
{
  return mLocations[static_cast<std::size_t>(location)];
}

bool SimImmuneResponse::CanGrabSprite(
  const EnumLocation location, const int x, const int y) const
{
  return GetLocation(location).CanGrabSprite(x, y);
}

SpritePtr SimImmuneResponse::GrabSprite(
  const EnumLocation location, const int x, const int y)
{
  return GetLocation(location).GrabSprite(x, y);
}

void SimImmuneResponse::AddSprite(const EnumCellType cellType, const EnumLocation location)
{
  AddSprite(CreateSprite(cellType, Sprite::maxx * 0.5, 0.0), location);
}

void SimImmuneResponse::AddSprite(const SpritePtr& s, const EnumLocation location)
{
  assert(s);
  GetLocation(location).AddSprite(s);
}

const Sprites& SimImmuneResponse::GetSprites(const EnumLocation location) const
{
  return GetLocation(location).GetSprites();
}

void SimImmuneResponse::Tick()
{
  mSimState = SimState();

  // Collect all migrants and let them arrive
  Migrants migrants;
  for (auto& location: mLocations) location.RemoveEmigrants(migrants);
  for (auto& location: mLocations) location.AddMigrants(migrants);

  for (const auto& location: mLocations) location.UpdateSimState(mSimState);
  for (auto& location: mLocations) location.RespondToSimState();
  for (auto& location: mLocations) location.MoveAllSprites();
}

std::string SimImmuneResponse::GetFeedback() const
{
  return mSimState.GetFeedback();
}
