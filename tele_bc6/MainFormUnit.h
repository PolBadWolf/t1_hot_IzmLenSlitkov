//---------------------------------------------------------------------------

#ifndef MainFormUnitH
#define MainFormUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Dialogs.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TEdit *comPortEdit;
        TButton *Button1;
        TShape *Shape1;
        TShape *Shape2;
        TShape *Shape3;
        TShape *Shape4;
        TShape *Shape5;
        TShape *Shape6;
        TShape *Shape7;
        TShape *Shape8;
        TButton *Button2;
        TShape *ShapeConnect;
        TTimer *TimerRender;
        TPanel *PanelTube;
        TPanel *PanelSpeed;
        TEdit *Edit1;
        TEdit *Edit2;
        TEdit *Edit3;
        TEdit *Edit4;
        TEdit *Edit5;
        TEdit *Edit6;
        TEdit *Edit7;
        TEdit *Edit8;
        TEdit *Edit9;
        TEdit *Edit10;
        TEdit *Edit11;
        TEdit *Edit12;
        TEdit *Edit13;
        TEdit *Edit14;
        TLabel *Label1;
        TStringGrid *StringGrid1;
        TGroupBox *GroupBox1;
        TLabel *Label2;
        TGroupBox *GroupBox2;
        TLabel *L_fist;
        TGroupBox *GroupBox3;
        TLabel *L_next;
        TGroupBox *GroupBox4;
        TLabel *L_sFist;
        TGroupBox *GroupBox5;
        TLabel *L_sNext;
        TGroupBox *GroupBox6;
        TLabel *L_OffSet;
        TButton *Button3;
        TEdit *Edit15;
        TSaveDialog *SaveDialog1;
        TSpeedButton *SpeedButton1;
        TShape *Shape9;
        TEdit *Edit_kspeed;
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall TimerRenderTimer(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall SpeedButton1Click(TObject *Sender);
private:	// User declarations
        void __fastcall EventNewDateComPort(int RdByte);
        TShape *SensInd[8];
        TEdit  *SpeedB[7];
        TEdit  *SpeedE[7];
        void __fastcall BbSaveData();
        int     eventNewDateInd;
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
