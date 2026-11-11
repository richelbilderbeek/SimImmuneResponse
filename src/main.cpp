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
#include <QApplication>

#include <cstdlib>
#include <ctime>

#include "main_window.h"

int main(int argc, char* argv[])
{
  std::srand(static_cast<unsigned>(std::time(nullptr)));
  QApplication app(argc, argv);
  QApplication::setApplicationName("SimImmuneResponse");
  MainWindow window;
  #ifdef Q_OS_WASM
  // The web page is the window: no title bar, top-left of the canvas
  window.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
  window.move(0, 0);
  #endif
  window.show();
  return app.exec();
}
