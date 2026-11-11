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
#include "sprite_images.h"

#include <map>
#include <stdexcept>

namespace {

QString GetResourceName(const EnumCellType cellType)
{
  using C = EnumCellType;
  switch (cellType)
  {
    case C::activatedBcell: return "activated_bcell";
    case C::activatedCytotoxicTcell: return "activated_cytotoxic_tcell";
    case C::activatedThelperCell: return "activated_thelper_cell";
    case C::antibody: return "antibody";
    case C::bCell: return "bcell";
    case C::cellHealthy: return "cell_healthy";
    case C::cellInfected: return "cell_infected";
    case C::cytokine: return "cytokine";
    case C::cytotoxicTcell: return "cytotoxic_tcell";
    case C::macrophageWithoutVirusParticles: return "macrophage_without_virus_particles";
    case C::macrophageWithVirusParticles: return "macrophage_with_virus_particles";
    case C::tHelperCell: return "thelper_cell";
    case C::virus: return "virus";
  }
  throw std::domain_error("Unknown cell type");
}

} // namespace

const QPixmap& GetPixmap(const EnumCellType cellType)
{
  static std::map<EnumCellType, QPixmap> pixmaps;
  auto i = pixmaps.find(cellType);
  if (i == pixmaps.end())
  {
    const QString filename = ":/images/" + GetResourceName(cellType) + ".png";
    QPixmap pixmap(filename);
    if (pixmap.isNull())
    {
      throw std::runtime_error("Cannot load " + filename.toStdString());
    }
    i = pixmaps.emplace(cellType, pixmap).first;
  }
  return i->second;
}

QString GetLegendText(const EnumCellType cellType)
{
  using C = EnumCellType;
  switch (cellType)
  {
    case C::activatedBcell: return "Activated\nB cell";
    case C::activatedCytotoxicTcell: return "Activated\ncytotoxic T cell";
    case C::activatedThelperCell: return "Activated\nT helper cell";
    case C::antibody: return "Anti\nbody";
    case C::bCell: return "Latent\nB cell";
    case C::cellHealthy: return "Healthy\ntissue cell";
    case C::cellInfected: return "Infected\ntissue cell";
    case C::cytokine: return "Cyto\nkine";
    case C::cytotoxicTcell: return "Cytotoxic\nT cell";
    case C::macrophageWithoutVirusParticles: return "Macrophage without\nvirus particles";
    case C::macrophageWithVirusParticles: return "Macrophage with\nvirus particles";
    case C::tHelperCell: return "Latent\nT helper cell";
    case C::virus: return "Virus\non the loose";
  }
  throw std::domain_error("Unknown cell type");
}
