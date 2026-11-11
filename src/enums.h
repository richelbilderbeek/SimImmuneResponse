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
#ifndef SIMIMMUNERESPONSE_ENUMS_H
#define SIMIMMUNERESPONSE_ENUMS_H

#include <array>
#include <string>

enum class EnumLocation
{
  bloodStream,
  boneMarrow,
  cellTissue,
  lymphNode
};

enum class EnumCellType
{
  activatedBcell,
  activatedCytotoxicTcell,
  activatedThelperCell,
  antibody,
  bCell,
  cellHealthy,
  cellInfected,
  cytokine,
  cytotoxicTcell,
  macrophageWithoutVirusParticles,
  macrophageWithVirusParticles,
  tHelperCell,
  virus
};

/// All locations, in a fixed order
constexpr std::array<EnumLocation, 4> GetAllLocations()
{
  return {
    EnumLocation::bloodStream,
    EnumLocation::boneMarrow,
    EnumLocation::cellTissue,
    EnumLocation::lymphNode
  };
}

/// All cell types, in a fixed order
constexpr std::array<EnumCellType, 13> GetAllCellTypes()
{
  return {
    EnumCellType::activatedBcell,
    EnumCellType::activatedCytotoxicTcell,
    EnumCellType::activatedThelperCell,
    EnumCellType::antibody,
    EnumCellType::bCell,
    EnumCellType::cellHealthy,
    EnumCellType::cellInfected,
    EnumCellType::cytokine,
    EnumCellType::cytotoxicTcell,
    EnumCellType::macrophageWithoutVirusParticles,
    EnumCellType::macrophageWithVirusParticles,
    EnumCellType::tHelperCell,
    EnumCellType::virus
  };
}

std::string CellTypeToString(const EnumCellType c);
std::string LocationToString(const EnumLocation location);

#endif // SIMIMMUNERESPONSE_ENUMS_H
