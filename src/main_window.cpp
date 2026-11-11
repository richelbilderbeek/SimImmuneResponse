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
#include "main_window.h"

#include <QApplication>
#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

#include <algorithm>

#include "sim_widget.h"
#include "sprite_images.h"

namespace {

QFont GetFont(const int pixelSize)
{
  QFont font("Courier New");
  font.setStyleHint(QFont::TypeWriter);
  font.setPixelSize(pixelSize);
  return font;
}

} // namespace

InfoWidget::InfoWidget(QWidget* parent)
  : QWidget(parent)
{
  setMinimumSize(200, 200);
  setToolTip("The cell you grabbed most recently");
}

void InfoWidget::SetCellType(const EnumCellType cellType)
{
  mHasCellType = true;
  mCellType = cellType;
  update();
}

void InfoWidget::paintEvent(QPaintEvent*)
{
  QPainter painter(this);
  painter.fillRect(rect(), Qt::black);
  if (!mHasCellType) return;
  const int size = std::min(width(), height());
  const QRect target((width() - size) / 2, (height() - size) / 2, size, size);
  painter.drawPixmap(target, GetPixmap(mCellType));
  painter.setFont(GetFont(14));
  const QString text = QString::fromStdString(CellTypeToString(mCellType));
  // Draw with a dark outline, so it is readable on any sprite
  painter.setPen(Qt::black);
  for (const QPoint d: { QPoint(-1, 0), QPoint(1, 0), QPoint(0, -1), QPoint(0, 1) })
  {
    painter.drawText(rect().translated(d), Qt::AlignCenter, text);
  }
  painter.setPen(Qt::white);
  painter.drawText(rect(), Qt::AlignCenter, text);
}

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent),
    mSimWidget{new SimWidget},
    mInfo{new InfoWidget},
    mFeedback{new QTextEdit}
{
  setWindowTitle("SimImmuneResponse");

  QWidget* const central = new QWidget;
  central->setAutoFillBackground(true);
  QPalette palette = central->palette();
  palette.setColor(QPalette::Window, Qt::black);
  central->setPalette(palette);

  mFeedback->setReadOnly(true);
  mFeedback->setFont(GetFont(19));
  mFeedback->setStyleSheet("QTextEdit { background: black; color: white; }");
  mFeedback->setMinimumWidth(260);

  QGridLayout* const layout = new QGridLayout(central);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(mSimWidget, 0, 0);
  layout->addWidget(CreateLegend(), 0, 1);

  QHBoxLayout* const bottom = new QHBoxLayout;
  bottom->setSpacing(0);
  bottom->addWidget(CreateOptions());
  bottom->addWidget(mInfo);
  bottom->addWidget(mFeedback, 1);
  layout->addLayout(bottom, 1, 0, 1, 2);
  setCentralWidget(central);

  connect(mSimWidget, &SimWidget::cellGrabbed, mInfo, &InfoWidget::SetCellType);
  connect(&mFeedbackTimer, &QTimer::timeout, this, &MainWindow::UpdateFeedback);
  mFeedbackTimer.start(5000);
  UpdateFeedback();
}

QWidget* MainWindow::CreateLegend()
{
  using C = EnumCellType;
  QWidget* const legend = new QWidget;
  QVBoxLayout* const layout = new QVBoxLayout(legend);
  layout->setContentsMargins(2, 2, 2, 2);
  layout->setSpacing(2);
  for (const C cellType: {
    C::virus, C::cellHealthy, C::cellInfected,
    C::macrophageWithoutVirusParticles, C::macrophageWithVirusParticles,
    C::tHelperCell, C::activatedThelperCell, C::cytokine,
    C::bCell, C::activatedBcell, C::antibody,
    C::cytotoxicTcell, C::activatedCytotoxicTcell })
  {
    QHBoxLayout* const row = new QHBoxLayout;
    QLabel* const image = new QLabel;
    image->setPixmap(GetPixmap(cellType));
    image->setFixedSize(Sprite::width, Sprite::height);
    QLabel* const text = new QLabel(GetLegendText(cellType));
    text->setFont(GetFont(11));
    text->setStyleSheet("color: white;");
    row->addWidget(image);
    row->addWidget(text, 1);
    layout->addLayout(row);
  }
  layout->addStretch();

  QScrollArea* const scroll = new QScrollArea;
  scroll->setWidget(legend);
  scroll->setWidgetResizable(true);
  scroll->setFixedWidth(190);
  scroll->setFixedHeight(mSimWidget->height());
  scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  scroll->setStyleSheet("QScrollArea, QScrollArea > QWidget > QWidget { background: black; }");
  return scroll;
}

QWidget* MainWindow::CreateOptions()
{
  using C = EnumCellType;
  QWidget* const options = new QWidget;
  options->setFixedWidth(260);
  QVBoxLayout* const layout = new QVBoxLayout(options);
  layout->setContentsMargins(8, 8, 8, 8);

  const auto add_button = [this, layout](const QString& text, const C cellType)
  {
    QPushButton* const button = new QPushButton(text);
    button->setFont(GetFont(12));
    connect(button, &QPushButton::clicked, this,
      [this, cellType]() { mSimWidget->AddCell(cellType); });
    layout->addWidget(button);
  };
  add_button("Produce macrophage", C::macrophageWithoutVirusParticles);
  add_button("Produce T helper cell", C::tHelperCell);
  add_button("Produce B cell", C::bCell);
  add_button("Produce cytotoxic T lymphocyte", C::cytotoxicTcell);

  QHBoxLayout* const row = new QHBoxLayout;
  QPushButton* const about = new QPushButton("About");
  QPushButton* const restart = new QPushButton("Restart");
  about->setFont(GetFont(12));
  restart->setFont(GetFont(12));
  row->addWidget(about);
  row->addWidget(restart);
  layout->addLayout(row);

  // In a web browser, the player quits by leaving the page
  #ifndef Q_OS_WASM
  QPushButton* const quit = new QPushButton("Quit");
  quit->setFont(GetFont(12));
  layout->addWidget(quit);
  connect(quit, &QPushButton::clicked, qApp, &QApplication::quit);
  #endif
  layout->addStretch();

  connect(about, &QPushButton::clicked, this, &MainWindow::ShowAbout);
  connect(restart, &QPushButton::clicked, this, [this]()
  {
    mSimWidget->Restart();
    UpdateFeedback();
  });
  return options;
}

void MainWindow::UpdateFeedback()
{
  mFeedback->setPlainText(
    QString::fromStdString(mSimWidget->GetSim().GetFeedback()));
}

void MainWindow::ShowAbout()
{
  // Not using QDialog::exec, as a blocking event loop
  // is not supported when running in a web browser
  QDialog* const dialog = new QDialog(this);
  dialog->setAttribute(Qt::WA_DeleteOnClose);
  dialog->setWindowTitle("About");
  QVBoxLayout* const layout = new QVBoxLayout(dialog);

  QHBoxLayout* const top = new QHBoxLayout;
  QLabel* const image = new QLabel;
  image->setPixmap(QPixmap(":/images/about.png"));
  top->addWidget(image);

  QVBoxLayout* const texts = new QVBoxLayout;
  QLabel* const title = new QLabel("SimImmuneResponse");
  title->setFont(GetFont(35));
  texts->addWidget(title);
  for (const char* const line: {
    "Licensed under GPL 3.0",
    "(C) 2008 Richel Bilderbeek",
    "Programmed in April and May 2008",
    "Ported to Qt in 2026",
    "http://www.richelbilderbeek.nl" })
  {
    QLabel* const label = new QLabel(line);
    label->setFont(GetFont(16));
    texts->addWidget(label);
  }
  texts->addStretch();
  top->addLayout(texts);
  layout->addLayout(top);

  QTextEdit* const license = new QTextEdit;
  license->setReadOnly(true);
  license->setFont(GetFont(11));
  license->setPlainText(
    "  SimImmuneResponse. Active simulation about the human immune response\n"
    "  Copyright (C) 2008  Richel Bilderbeek\n"
    "\n"
    "  This program is free software: you can redistribute it and/or modify\n"
    "  it under the terms of the GNU General Public License as published by\n"
    "  the Free Software Foundation, either version 3 of the License, or\n"
    "  (at your option) any later version.\n"
    "\n"
    "  This program is distributed in the hope that it will be useful,\n"
    "  but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
    "  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
    "  GNU General Public License for more details.\n"
    "  You should have received a copy of the GNU General Public License\n"
    "  along with this program.  If not, see <http://www.gnu.org/licenses/>.");
  license->setFixedHeight(120);
  layout->addWidget(license);

  QPushButton* const ok = new QPushButton("OK");
  connect(ok, &QPushButton::clicked, dialog, &QDialog::accept);
  layout->addWidget(ok, 0, Qt::AlignRight);
  dialog->open();
}
