//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormUnit1.h"
#include "crc8.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
int OldX = -1000;
bool mh_up = true;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
        Datch[0] = Edit1;
        Datch[1] = Edit2;
        Datch[2] = Edit3;
        Datch[3] = Edit4;
        Datch[4] = Edit5;
        Datch[5] = Edit6;
        Datch[6] = Edit7;
        Datch[7] = Edit8;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::GroupBox10MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
        OldX = X;
        mh_up = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::GroupBox10MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
        int dl_cur, dl_sm;
        if (OldX>0)
        {
                if (!mh_up)
                {
                        dl_cur = GroupBox10->Width;
                        dl_sm = X - OldX;
                        if (dl_sm>0)
                        {
                                if ( (dl_cur+dl_sm)>800 )
                                {
                                        dl_sm = 800-dl_cur;
                                }
                        }
                        if (dl_sm<0)
                        {
                                if ( (dl_cur+dl_sm)<70 )
                                {
                                        dl_sm = 70-dl_cur;
                                }
                        }
                        GroupBox10->Width = dl_cur+dl_sm;
                        Label1->Caption = int((dl_cur+dl_sm)*20/1.5);
                }
        }
        OldX = X;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::GroupBox10MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
        OldX = -1000;
        mh_up = true;
}
//---------------------------------------------------------------------------
        unsigned char buff[8];

void __fastcall TForm1::Button1Click(TObject *Sender)
{
//
        int stat;
        if (Port!=NULL)
        {
                delete Port;
                Port = NULL;
        }
        Port = new TComPort;
        if (Port==NULL)
        {
                ShowMessage("Ошибка создания объекта порта");
                return;
        }
        stat = Port->Open(Edit10->Text,BaudRate9600,NO);
        if (stat>0)
        {
                ShowMessage("Ошибка открытия порта");
        }
        else
        {
                int temp;
                for (int i=0;i<8;i++) //8;i++)
                {
                        temp = Datch[i]->Text.ToInt();
                        buff[0] = 0xe6;
                        buff[1] = 0x19;
                        buff[2] = 0x00;
                        buff[3] = i;
                        buff[4] = ((unsigned char *)& temp)[0];
                        buff[5] = ((unsigned char *)& temp)[1];
                        buff[6] = crc8_buf(buff,6);
                        buff[7] = 0;
                        Port->Write(buff, 8);
                }
                //длина
                temp = Label1->Caption.ToInt();
                buff[0] = 0xe6;
                buff[1] = 0x19;
                buff[2] = 0x00;
                buff[3] = 8;
                buff[4] = ((unsigned char *)& temp)[0];
                buff[5] = ((unsigned char *)& temp)[1];
                buff[6] = crc8_buf(buff,6);
                buff[7] = 0;
                Port->Write(buff, 8);
                // скорость
                temp = Edit9->Text.ToInt();
                buff[0] = 0xe6;
                buff[1] = 0x19;
                buff[2] = 0x00;
                buff[3] = 9;
                buff[4] = ((unsigned char *)& temp)[0];
                buff[5] = ((unsigned char *)& temp)[1];
                buff[6] = crc8_buf(buff,6);
                buff[7] = 0;
                Port->Write(buff, 8);
                while ( Port->Deep() ) Sleep(20);
        }
        if (Port!=NULL)
        {
                delete Port;
                Port = NULL;
        }
}
//---------------------------------------------------------------------------



