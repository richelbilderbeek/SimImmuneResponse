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
#include "location.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>

Location::Location(const EnumLocation location)
  : mLocation{location}
{
  switch (mLocation)
  {
    case EnumLocation::bloodStream:
    {
      const int nViruses = 10;
      for (int i = 0; i != nViruses; ++i)
      {
        AddSprite(CreateSprite(EnumCellType::virus,
          std::rand() % static_cast<int>(Sprite::maxx),
          std::rand() % static_cast<int>(Sprite::maxy)));
      }
      break;
    }
    case EnumLocation::cellTissue:
    {
      // 7 rows, plus one row above the screen, of 10 columns
      for (int y = -1; y != 7; ++y)
      {
        for (int x = 0; x != 10; ++x)
        {
          AddSprite(CreateSprite(EnumCellType::cellHealthy,
            x * Sprite::width, y * Sprite::height));
        }
      }
      break;
    }
    case EnumLocation::boneMarrow:
    case EnumLocation::lymphNode:
      break;
  }
}

void Location::AddSprite(const SpritePtr& s)
{
  mSprites.push_back(s);
}

bool Location::CanGrabSprite(const int x, const int y) const
{
  return std::any_of(mSprites.begin(), mSprites.end(),
    [x, y](const SpritePtr& s) { return s->CanGrab(x, y); });
}

SpritePtr Location::GrabSprite(const int x, const int y)
{
  // Search from the back, as the last sprite is drawn on top
  const auto i = std::find_if(mSprites.rbegin(), mSprites.rend(),
    [x, y](const SpritePtr& s) { return s->CanGrab(x, y); });
  if (i == mSprites.rend()) return nullptr;
  SpritePtr sprite = *i;
  mSprites.erase(std::next(i).base());
  return sprite;
}

bool Location::IsPresent(const EnumCellType cellType) const
{
  return Count(cellType) > 0;
}

int Location::Count(const EnumCellType cellType) const
{
  return static_cast<int>(std::count_if(mSprites.begin(), mSprites.end(),
    [cellType](const SpritePtr& s) { return s->GetCellType() == cellType; }));
}

void Location::RemoveEmigrants(Migrants& emigrants)
{
  for (auto i = mSprites.begin(); i != mSprites.end(); )
  {
    if (const auto destination = (*i)->WantsToMigrateTo())
    {
      emigrants.emplace_back(*destination, *i);
      i = mSprites.erase(i);
    }
    else
    {
      ++i;
    }
  }
}

void Location::AddMigrants(const Migrants& immigrants)
{
  for (const auto& p: immigrants)
  {
    if (p.first == mLocation) mSprites.push_back(p.second);
  }
}

void Location::RespondToSimState()
{
  if (mLocation != EnumLocation::bloodStream) return;

  // Add viruses, except when there are antibodies and macrophages
  if (Count(EnumCellType::virus) < 5
    && GetRandomInt() % 100 == 0
    && !IsPresent(EnumCellType::antibody)
    && !IsPresent(EnumCellType::macrophageWithVirusParticles)
    && !IsPresent(EnumCellType::macrophageWithoutVirusParticles))
  {
    AddSprite(CreateSprite(EnumCellType::virus,
      static_cast<double>(std::rand() % static_cast<int>(Sprite::maxx) + 64) - 32.0,
      -32.0));
  }
}

void Location::MoveAllSprites()
{
  for (const auto& s: mSprites) s->Move();
  LetSpritesAct();
  LetSpritesInteract();
}

void Location::LetSpritesAct()
{
  using C = EnumCellType;
  for (auto i = mSprites.begin(); i != mSprites.end(); )
  {
    const SpritePtr s = *i;

    // Infected cell bursts, spawning viruses
    if (s->GetCellType() == C::cellInfected && GetRandomInt() % 2000 == 0)
    {
      for (int j = 0; j != 3; ++j)
      {
        AddSprite(CreateSprite(C::virus, s->GetX(), s->GetY()));
      }
      i = mSprites.erase(i);
      continue;
    }

    // Activated T helper cells produce cytokines, in lymph node only
    if (s->GetCellType() == C::activatedThelperCell
      && mLocation == EnumLocation::lymphNode
      && GetRandomInt() % 10 == 0
      && Count(C::cytokine) < 5)
    {
      AddSprite(CreateSprite(C::cytokine, s->GetX(), s->GetY()));
    }

    // Activated B cells produce antibodies, in lymph node only
    if (s->GetCellType() == C::activatedBcell
      && mLocation == EnumLocation::lymphNode
      && GetRandomInt() % 100 == 0
      && Count(C::antibody) < 5)
    {
      AddSprite(CreateSprite(C::antibody, s->GetX(), s->GetY()));
    }
    ++i;
  }
}

void Location::LetSpritesInteract()
{
  using C = EnumCellType;
  const bool inBloodStream = mLocation == EnumLocation::bloodStream;

  for (auto i = mSprites.begin(); i != mSprites.end(); ++i)
  {
    for (auto j = mSprites.begin(); j != mSprites.end(); )
    {
      if (i == j || !AreNear(**i, **j)) { ++j; continue; }

      const C a = (*i)->GetCellType();
      const C b = (*j)->GetCellType();
      const auto replace_i = [&i](const C with)
      {
        *i = CreateSprite(with, (*i)->GetX(), (*i)->GetY());
      };

      // Healthy cell gets infected by a virus
      if (a == C::cellHealthy && b == C::virus && GetRandomInt() % 500 == 0)
      {
        replace_i(C::cellInfected);
        j = mSprites.erase(j);
        continue;
      }

      // Macrophages without virus particles ingest viruses in blood stream
      if (a == C::macrophageWithoutVirusParticles && inBloodStream && b == C::virus)
      {
        replace_i(C::macrophageWithVirusParticles);
        j = mSprites.erase(j);
        continue;
      }

      // Macrophages destroy viruses, if helped by antibodies
      if (a == C::macrophageWithVirusParticles
        && b == C::virus
        && inBloodStream
        && IsPresent(C::antibody))
      {
        j = mSprites.erase(j);
        continue;
      }

      // Cytokines activate B cells
      if ((*i)->GetCellType() == C::bCell && b == C::cytokine)
      {
        replace_i(C::activatedBcell);
      }

      // Cytokines activate cytotoxic T cells
      if ((*i)->GetCellType() == C::cytotoxicTcell && b == C::cytokine)
      {
        replace_i(C::activatedCytotoxicTcell);
      }

      // Activated cytotoxic T cells destroy infected cells
      if ((*i)->GetCellType() == C::activatedCytotoxicTcell && b == C::cellInfected)
      {
        j = mSprites.erase(j);
        continue;
      }

      // Macrophages with virus particles activate T helper cells
      if ((*i)->GetCellType() == C::tHelperCell && b == C::macrophageWithVirusParticles)
      {
        replace_i(C::activatedThelperCell);
      }
      ++j;
    }
  }
}

void Location::UpdateSimState(SimState& state) const
{
  for (const auto& s: mSprites)
  {
    state.GetLocation(mLocation).insert(s->GetCellType());
  }
}

bool AreNear(const Sprite& a, const Sprite& b)
{
  const int dx = static_cast<int>(a.GetX()) - static_cast<int>(b.GetX());
  const int dy = static_cast<int>(a.GetY()) - static_cast<int>(b.GetY());
  return std::abs(dx) < Sprite::width && std::abs(dy) < Sprite::height;
}
