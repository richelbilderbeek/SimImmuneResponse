//---------------------------------------------------------------------------
/*
  SimImmuneResponse. Active simulation about the human immune response
  Copyright (C) 2008  Richel Bilderbeek

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
//---------------------------------------------------------------------------
// From http://www.richelbilderbeek.nl
//---------------------------------------------------------------------------
#ifndef UnitFormSimImmuneResponseAboutH
#define UnitFormSimImmuneResponseAboutH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TFormSimImmuneResponseAbout : public TForm
{
__published:	// IDE-managed Components
        TRichEdit *RichEdit1;
        TPanel *PanelTop;
        TImage *Image1;
        TPanel *PanelTopRight;
        TPanel *Panel1;
        TPanel *Panel2;
        TPanel *Panel3;
        TPanel *Panel4;
        TPanel *Panel5;
private:	// User declarations
public:		// User declarations
        __fastcall TFormSimImmuneResponseAbout(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSimImmuneResponseAbout *FormSimImmuneResponseAbout;
//---------------------------------------------------------------------------
#endif
