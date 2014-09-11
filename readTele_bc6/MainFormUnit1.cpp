//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainFormUnit1.h"
#include <Windows.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
HFILE fileHandle;
LPOFSTRUCT lpReOpenBuff;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
        if (!OpenDialog1->Execute() )
                return;
        AnsiString pathName = OpenDialog1->FileName;
        AnsiString name = ExtractFilePath(pathName);
        AnsiString path = ExtractFilePath(pathName);
        //lpReOpenBuff->szPathName = path.c_str();
        fileHandle = OpenFile(  name.c_str(),
                                lpReOpenBuff,
                                OF_READ); 
}
//---------------------------------------------------------------------------
