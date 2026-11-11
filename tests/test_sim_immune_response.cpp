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
#include <QtTest>

#include "sim_immune_response.h"
#include "sim_widget.h"
#include "sprite_images.h"

using C = EnumCellType;
using L = EnumLocation;

class TestSimImmuneResponse : public QObject
{
  Q_OBJECT

  private slots:

  void initTestCase()
  {
    std::srand(42);
  }

  void cell_types_have_names_and_images()
  {
    for (const auto c: GetAllCellTypes())
    {
      QVERIFY(!CellTypeToString(c).empty());
      QVERIFY(!GetLegendText(c).isEmpty());
      QCOMPARE(GetPixmap(c).width(), Sprite::width);
      QCOMPARE(GetPixmap(c).height(), Sprite::height);
    }
  }

  void initial_state()
  {
    const SimImmuneResponse sim;
    QCOMPARE(sim.GetLocation(L::bloodStream).Count(C::virus), 10);
    QCOMPARE(sim.GetLocation(L::cellTissue).Count(C::cellHealthy), 80);
    QVERIFY(sim.GetSprites(L::boneMarrow).empty());
    QVERIFY(sim.GetSprites(L::lymphNode).empty());
    QVERIFY(QString::fromStdString(sim.GetFeedback()).contains("Create macrophages"));
  }

  void produced_cell_appears_in_bone_marrow_and_can_be_grabbed()
  {
    SimImmuneResponse sim;
    sim.AddSprite(C::bCell, L::boneMarrow);
    QCOMPARE(sim.GetLocation(L::boneMarrow).Count(C::bCell), 1);
    QVERIFY(sim.CanGrabSprite(L::boneMarrow, 165, 5));
    const auto s = sim.GrabSprite(L::boneMarrow, 165, 5);
    QVERIFY(s);
    QCOMPARE(s->GetCellType(), C::bCell);
    QVERIFY(sim.GetSprites(L::boneMarrow).empty());
    QVERIFY(!sim.GrabSprite(L::boneMarrow, 165, 5));
  }

  void viruses_and_tissue_cells_cannot_be_grabbed()
  {
    SimImmuneResponse sim;
    sim.AddSprite(CreateSprite(C::virus, 0, 0), L::boneMarrow);
    sim.AddSprite(CreateSprite(C::cellHealthy, 100, 0), L::boneMarrow);
    QVERIFY(!sim.CanGrabSprite(L::boneMarrow, 5, 5));
    QVERIFY(!sim.CanGrabSprite(L::boneMarrow, 105, 5));
  }

  void sprites_leaving_the_top_migrate()
  {
    Location bone(L::boneMarrow);
    bone.AddSprite(CreateSprite(C::virus, 10, -40));
    bone.AddSprite(CreateSprite(C::bCell, 10, -40)); // cells do not migrate
    Migrants migrants;
    bone.RemoveEmigrants(migrants);
    QCOMPARE(static_cast<int>(migrants.size()), 1);
    QCOMPARE(bone.Count(C::bCell), 1);
    const L destination = migrants.front().first;
    QVERIFY(destination == L::bloodStream || destination == L::cellTissue);
  }

  void macrophage_ingests_virus_in_blood_stream()
  {
    Location blood(L::bloodStream);
    // Let the initial viruses leave, so only the test virus remains
    for (const auto& s: blood.GetSprites()) s->SetY(-100.0);
    Migrants leavers;
    blood.RemoveEmigrants(leavers);
    QVERIFY(blood.GetSprites().empty());

    blood.AddSprite(CreateSprite(C::macrophageWithoutVirusParticles, 100, 100));
    blood.AddSprite(CreateSprite(C::virus, 101, 101));
    blood.MoveAllSprites();
    QCOMPARE(blood.Count(C::virus), 0);
    QCOMPARE(blood.Count(C::macrophageWithVirusParticles), 1);
  }

  void macrophage_with_virus_activates_t_helper_cell()
  {
    Location lymph(L::lymphNode);
    lymph.AddSprite(CreateSprite(C::tHelperCell, 100, 100));
    lymph.AddSprite(CreateSprite(C::macrophageWithVirusParticles, 100, 100));
    lymph.MoveAllSprites();
    QCOMPARE(lymph.Count(C::activatedThelperCell), 1);
    // Activated T helper cells produce cytokines in the lymph node
    for (int i = 0; i != 1000 && !lymph.IsPresent(C::cytokine); ++i)
    {
      lymph.MoveAllSprites();
    }
    QVERIFY(lymph.IsPresent(C::cytokine));
  }

  void cytokine_activates_b_cell_which_produces_antibodies()
  {
    Location lymph(L::lymphNode);
    lymph.AddSprite(CreateSprite(C::bCell, 100, 100));
    lymph.AddSprite(CreateSprite(C::cytokine, 100, 100));
    lymph.MoveAllSprites();
    QCOMPARE(lymph.Count(C::activatedBcell), 1);
    for (int i = 0; i != 10000 && !lymph.IsPresent(C::antibody); ++i)
    {
      lymph.MoveAllSprites();
    }
    QVERIFY(lymph.IsPresent(C::antibody));
  }

  void activated_cytotoxic_t_cell_kills_infected_cell()
  {
    Location tissue(L::boneMarrow);
    tissue.AddSprite(CreateSprite(C::cytotoxicTcell, 100, 100));
    tissue.AddSprite(CreateSprite(C::cytokine, 100, 100));
    tissue.AddSprite(CreateSprite(C::cellInfected, 100, 100));
    tissue.MoveAllSprites();
    QCOMPARE(tissue.Count(C::activatedCytotoxicTcell), 1);
    QCOMPARE(tissue.Count(C::cellInfected), 0);
  }

  void feedback_progresses()
  {
    SimState s;
    s.GetLocation(L::cellTissue).insert(C::cellHealthy);
    QVERIFY(QString::fromStdString(s.GetFeedback()).contains("healthy"));
    s.GetLocation(L::bloodStream).insert(C::virus);
    QVERIFY(QString::fromStdString(s.GetFeedback()).contains("Create macrophages"));
    s.GetLocation(L::boneMarrow).insert(C::macrophageWithoutVirusParticles);
    QVERIFY(QString::fromStdString(s.GetFeedback()).contains("Send your macrophages to the blood stream"));

    SimState dead;
    dead.GetLocation(L::bloodStream).insert(C::virus);
    QVERIFY(QString::fromStdString(dead.GetFeedback()).contains("GAME OVER"));
  }

  void long_run_is_stable()
  {
    SimImmuneResponse sim;
    for (const auto c: { C::macrophageWithoutVirusParticles, C::tHelperCell,
      C::bCell, C::cytotoxicTcell })
    {
      sim.AddSprite(c, L::boneMarrow);
    }
    for (int i = 0; i != 20000; ++i)
    {
      sim.Tick();
      for (const auto location: GetAllLocations())
      {
        for (const auto& s: sim.GetSprites(location))
        {
          // New viruses may enter up to one sprite width outside
          // the location, after which they are pushed back in
          QVERIFY(s->GetX() >= -Sprite::width - Sprite::speed * 2);
          QVERIFY(s->GetX() <= Sprite::maxx + Sprite::speed * 2);
          QVERIFY(s->GetY() <= Sprite::maxy + Sprite::speed * 2);
        }
      }
    }
    QVERIFY(!sim.GetFeedback().empty());
  }

  void drag_cell_from_bone_marrow_to_blood_stream()
  {
    SimWidget w;
    QCOMPARE(w.size(), QSize(640, 448));
    QSignalSpy spy(&w, &SimWidget::cellGrabbed);
    w.AddCell(C::macrophageWithoutVirusParticles);
    // New cells appear at the top-center of the bone marrow
    QTest::mousePress(&w, Qt::LeftButton, Qt::NoModifier, QPoint(165, 5));
    QCOMPARE(spy.count(), 1);
    QTest::mouseMove(&w, QPoint(50, 300));
    QTest::mouseRelease(&w, Qt::LeftButton, Qt::NoModifier, QPoint(50, 300));
    QVERIFY(!w.GetSim().GetLocation(L::boneMarrow).IsPresent(C::macrophageWithoutVirusParticles));
    QCOMPARE(w.GetSim().GetLocation(L::bloodStream).Count(C::macrophageWithoutVirusParticles), 1);
    const auto& last = w.GetSim().GetSprites(L::bloodStream).back();
    QCOMPARE(static_cast<int>(last->GetX()), 50);
    QCOMPARE(static_cast<int>(last->GetY()), 300 - 224);
  }

  void restart_resets_simulation()
  {
    SimWidget w;
    w.AddCell(C::bCell);
    w.Restart();
    QVERIFY(w.GetSim().GetSprites(L::boneMarrow).empty());
  }

  void location_of_widget_coordinates()
  {
    QCOMPARE(SimWidget::GetLocation(QPoint(10, 10)), L::boneMarrow);
    QCOMPARE(SimWidget::GetLocation(QPoint(400, 10)), L::cellTissue);
    QCOMPARE(SimWidget::GetLocation(QPoint(10, 300)), L::bloodStream);
    QCOMPARE(SimWidget::GetLocation(QPoint(400, 300)), L::lymphNode);
  }
};

QTEST_MAIN(TestSimImmuneResponse)
#include "test_sim_immune_response.moc"
