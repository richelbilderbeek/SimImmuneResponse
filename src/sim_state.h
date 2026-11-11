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
#ifndef SIMIMMUNERESPONSE_SIM_STATE_H
#define SIMIMMUNERESPONSE_SIM_STATE_H

#include <set>
#include <string>

#include "enums.h"

/// Which cell types are present at which location.
/// Used to give the player feedback on what to do next.
struct SimState
{
  std::string GetFeedback() const;

  bool IsPresent(const EnumLocation location, const EnumCellType cellType) const;
  bool IsPresent(const EnumCellType cellType) const;

  std::set<EnumCellType>& GetLocation(const EnumLocation location);
  const std::set<EnumCellType>& GetLocation(const EnumLocation location) const;

  private:
  std::set<EnumCellType> mInBloodStream;
  std::set<EnumCellType> mInBoneMarrow;
  std::set<EnumCellType> mInCellTissue;
  std::set<EnumCellType> mInLymphNode;
};

#endif // SIMIMMUNERESPONSE_SIM_STATE_H
