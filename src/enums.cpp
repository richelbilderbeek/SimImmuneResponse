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
#include "enums.h"

#include <stdexcept>

std::string CellTypeToString(const EnumCellType c)
{
  switch (c)
  {
    case EnumCellType::activatedBcell: return "activated B cell";
    case EnumCellType::activatedCytotoxicTcell: return "activated cytotoxic T cell";
    case EnumCellType::activatedThelperCell: return "activated T helper cell";
    case EnumCellType::antibody: return "antibody";
    case EnumCellType::bCell: return "B cell";
    case EnumCellType::cellHealthy: return "healthy cell";
    case EnumCellType::cellInfected: return "infected cell";
    case EnumCellType::cytokine: return "cytokine";
    case EnumCellType::cytotoxicTcell: return "cytotoxic T cell";
    case EnumCellType::macrophageWithVirusParticles: return "macrophage with virus particles";
    case EnumCellType::macrophageWithoutVirusParticles: return "macrophage without virus particles";
    case EnumCellType::tHelperCell: return "T helper cell";
    case EnumCellType::virus: return "virus";
  }
  throw std::domain_error("Unknown cell type of integer value "
    + std::to_string(static_cast<int>(c)));
}

std::string LocationToString(const EnumLocation location)
{
  switch (location)
  {
    case EnumLocation::bloodStream: return "Blood stream";
    case EnumLocation::boneMarrow: return "Bone marrow";
    case EnumLocation::cellTissue: return "Cell tissue";
    case EnumLocation::lymphNode: return "Lymph node";
  }
  throw std::domain_error("Unknown location of integer value "
    + std::to_string(static_cast<int>(location)));
}
