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
#include "sim_state.h"

#include <stdexcept>

namespace {

/// In debug builds, prefix the feedback with its index, to ease debugging
std::string Tag(const int index)
{
  #ifndef NDEBUG
  return "[" + std::to_string(index) + "] ";
  #else
  (void)index;
  return "";
  #endif
}

} // namespace

std::string SimState::GetFeedback() const
{
  using C = EnumCellType;
  using L = EnumLocation;

  const bool anything_present =
       !mInBloodStream.empty()
    || !mInBoneMarrow.empty()
    || !mInCellTissue.empty()
    || !mInLymphNode.empty();

  if (!IsPresent(C::cellHealthy) && anything_present)
  {
    return Tag(-1) + "The organism has died. GAME OVER.";
  }

  if (!IsPresent(C::virus)
    && !IsPresent(C::antibody)
    && IsPresent(C::cellHealthy))
  {
    return Tag(0) + "The organism is healthy yet...";
  }

  if (!IsPresent(C::macrophageWithoutVirusParticles)
    && !IsPresent(C::macrophageWithVirusParticles))
  {
    return Tag(1) +
      "Viruses in bloodstream and in cell tissue!"
      " Create macrophages to ingest viruses.";
  }
  if (IsPresent(C::macrophageWithoutVirusParticles)
    && !IsPresent(L::bloodStream, C::macrophageWithoutVirusParticles)
    && !IsPresent(L::bloodStream, C::macrophageWithVirusParticles))
  {
    return Tag(2) +
      "Viruses in bloodstream and in cell tissue!"
      " Send your macrophages to the blood stream"
      " to ingest viruses.";
  }
  if (IsPresent(C::macrophageWithoutVirusParticles)
    && IsPresent(L::bloodStream, C::macrophageWithoutVirusParticles)
    && !IsPresent(L::bloodStream, C::macrophageWithVirusParticles))
  {
    return Tag(3) +
      "Viruses in bloodstream and in cell tissue!"
      " Send your macrophages in the blood stream near"
      " viruses to ingest them.";
  }
  if (IsPresent(C::macrophageWithVirusParticles)
    && !IsPresent(C::antibody)
    && IsPresent(L::bloodStream, C::macrophageWithVirusParticles)
    && !IsPresent(L::lymphNode, C::macrophageWithVirusParticles))
  {
    return Tag(4) +
      "Macrophage has ingested virus. Send it to the lymph node to"
      " alert the T helper cells.";
  }
  if (IsPresent(L::lymphNode, C::macrophageWithVirusParticles)
    && !IsPresent(C::tHelperCell)
    && !IsPresent(C::activatedThelperCell))
  {
    return Tag(5) +
      "Macrophage with ingested virus must alert T helper cells in lymph"
      " node. Produce T helper cell to respond.";
  }
  if (IsPresent(L::lymphNode, C::macrophageWithVirusParticles)
    && IsPresent(C::tHelperCell)
    && !IsPresent(L::lymphNode, C::tHelperCell)
    && !IsPresent(L::lymphNode, C::activatedThelperCell))
  {
    return Tag(6) +
      "Macrophage with ingested virus must alert T helper cells in lymph"
      " node. Send T helper cells to lymph node.";
  }
  if (IsPresent(L::lymphNode, C::macrophageWithVirusParticles)
    && IsPresent(C::tHelperCell)
    && IsPresent(L::lymphNode, C::tHelperCell)
    && !IsPresent(L::lymphNode, C::activatedThelperCell))
  {
    return Tag(7) +
      "Macrophage with ingested virus must alert T helper cells in lymph"
      " node. Send T helper cell near macrophage in lymph node.";
  }
  if (IsPresent(L::lymphNode, C::cytokine)
    && !IsPresent(C::bCell)
    && !IsPresent(C::activatedBcell))
  {
    return Tag(8) +
      "T helper cells are active and producing cytokines. Produce"
      " a B cell to start the humoral response.";
  }
  if (IsPresent(L::lymphNode, C::cytokine)
    && IsPresent(C::bCell)
    && !IsPresent(L::lymphNode, C::bCell)
    && !IsPresent(L::lymphNode, C::activatedBcell))
  {
    return Tag(9) +
      "T helper cells are active and producing cytokines. Send"
      " B cell to lymph node to start the humoral response.";
  }
  if (IsPresent(L::lymphNode, C::cytokine)
    && IsPresent(C::bCell)
    && IsPresent(L::lymphNode, C::bCell)
    && !IsPresent(L::lymphNode, C::activatedBcell))
  {
    return Tag(10) +
      "Let B cell in lymph node start the humoral response"
      " by activating them with cytokine.";
  }
  if (IsPresent(L::lymphNode, C::activatedBcell)
    && IsPresent(L::lymphNode, C::antibody)
    && !IsPresent(L::bloodStream, C::antibody))
  {
    return Tag(11) +
      "B cells in lymph node produce antibodies."
      " Wait until they reach the blood stream before"
      " viruses in blood stream can be destroyed.";
  }
  if (IsPresent(L::bloodStream, C::antibody)
    && !IsPresent(L::bloodStream, C::macrophageWithVirusParticles)
    && !IsPresent(L::bloodStream, C::macrophageWithoutVirusParticles))
  {
    return Tag(12) +
      "Antibodies present in blood stream."
      " Send macrophages to blood stream to ingest viruses."
      " Due to the antibodies, they can effectively remove all viruses now.";
  }
  if (IsPresent(L::cellTissue, C::cytokine)
    && !IsPresent(C::cytotoxicTcell)
    && !IsPresent(C::activatedCytotoxicTcell))
  {
    return Tag(13) +
      "Macrophages in blood stream effectively remove"
      " viruses in blood stream, due to antibodies."
      " Produce cytotoxic T cells to disinfect cell tissue.";
  }
  if (IsPresent(L::cellTissue, C::cytokine)
    && IsPresent(C::cytotoxicTcell)
    && !IsPresent(C::activatedCytotoxicTcell))
  {
    return Tag(14) +
      "Activate cytotoxic T cells with cytokine"
      " to disinfect cell tissue.";
  }
  if (IsPresent(L::cellTissue, C::cytokine)
    && IsPresent(C::activatedCytotoxicTcell)
    && !IsPresent(L::cellTissue, C::activatedCytotoxicTcell))
  {
    return Tag(15) +
      "Macrophages in blood stream effectively remove"
      " viruses in blood stream, due to antibodies."
      " Send cytotoxic T cells to cell tissue to disinfect cell tissue.";
  }
  if (IsPresent(L::cellTissue, C::cytokine)
    && IsPresent(C::activatedCytotoxicTcell)
    && IsPresent(L::cellTissue, C::activatedCytotoxicTcell)
    && IsPresent(C::virus))
  {
    return Tag(16) +
      "Macrophages in blood stream effectively remove"
      " viruses in blood stream, due to antibodies."
      " Cytotoxic T cells actively destroy infected cells,"
      " due to cytokines.";
  }
  return Tag(99) + "The organism has acquired immunity.";
}

bool SimState::IsPresent(const EnumLocation location, const EnumCellType cellType) const
{
  return GetLocation(location).count(cellType) == 1;
}

bool SimState::IsPresent(const EnumCellType cellType) const
{
  for (const auto location: GetAllLocations())
  {
    if (IsPresent(location, cellType)) return true;
  }
  return false;
}

std::set<EnumCellType>& SimState::GetLocation(const EnumLocation location)
{
  return const_cast<std::set<EnumCellType>&>(
    static_cast<const SimState&>(*this).GetLocation(location));
}

const std::set<EnumCellType>& SimState::GetLocation(const EnumLocation location) const
{
  switch (location)
  {
    case EnumLocation::bloodStream: return mInBloodStream;
    case EnumLocation::boneMarrow: return mInBoneMarrow;
    case EnumLocation::cellTissue: return mInCellTissue;
    case EnumLocation::lymphNode: return mInLymphNode;
  }
  throw std::logic_error("Unknown location");
}
