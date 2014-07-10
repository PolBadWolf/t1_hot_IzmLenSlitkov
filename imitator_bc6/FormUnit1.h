//---------------------------------------------------------------------------

#ifndef FormUnit1H
#define FormUnit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ComPort.h"
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *GroupBox1;
        TEdit *Edit1;
        TGroupBox *GroupBox2;
        TEdit *Edit2;
        TGroupBox *GroupBox3;
        TEdit *Edit3;
        TGroupBox *GroupBox4;
        TEdit *Edit4;
        TGroupBox *GroupBox5;
        TEdit *Edit5;
        TGroupBox *GroupBox6;
        TEdit *Edit6;
        TGroupBox *GroupBox7;
        TEdit *Edit7;
        TGroupBox *GroupBox8;
        TEdit *Edit8;
        TGroupBox *GroupBox9;
        TGroupBox *GroupBox10;
        TLabel *Label1;
        TButton *Button1;
        TEdit *Edit9;
        TEdit *Edit10;
        void __fastcall GroupBox10MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall GroupBox10MouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall GroupBox10MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
        TEdit  *Datch[8];
        TComPort *Port;
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
