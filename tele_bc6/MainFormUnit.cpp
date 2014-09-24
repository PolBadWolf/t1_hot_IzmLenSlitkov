//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#pragma hdrstop
#include <fstream.h>

#include "MainFormUnit.h"
#include "ComPort.h"
#include "crc8.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
TComPort *comPort = NULL;
const int sensOffset = 20;
const int sensMetr = 100;
struct TbSensor
{
        int     position;
        int     timeBegin;
        int     timeEnd;
};
struct TbSens
{
        char     len[6];
        int      fist;
        int      next;
        int      napr;
        int      sFist;
        int      sNext;
        int      offset;
        TbSensor sensor[8];
} _Sens;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
        SensInd[0] = Shape1;
        SensInd[1] = Shape2;
        SensInd[2] = Shape3;
        SensInd[3] = Shape4;
        SensInd[4] = Shape5;
        SensInd[5] = Shape6;
        SensInd[6] = Shape7;
        SensInd[7] = Shape8;
        SpeedB[0]  = Edit1;
        SpeedB[1]  = Edit2;
        SpeedB[2]  = Edit3;
        SpeedB[3]  = Edit4;
        SpeedB[4]  = Edit5;
        SpeedB[5]  = Edit6;
        SpeedB[6]  = Edit7;
        SpeedE[0]  = Edit8;
        SpeedE[1]  = Edit9;
        SpeedE[2]  = Edit10;
        SpeedE[3]  = Edit11;
        SpeedE[4]  = Edit12;
        SpeedE[5]  = Edit13;
        SpeedE[6]  = Edit14;
        //BbSaveData();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
        if (comPort)
        {
                comPort->Close();
                delete comPort;
                comPort = NULL;
                ShapeConnect->Brush->Color = clWindow;
        }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
        if (comPort)
        {
                comPort->Close();
                delete comPort;
                comPort = NULL;
        }
        comPort = new TComPort;
        if (!comPort)
        {
                ShapeConnect->Brush->Color = clRed;
                ShowMessage("Ошибка создания объекта порта");
                return;
        }
        // ===================
        comPort->EventNewDate = EventNewDateComPort;
        if ( comPort->Open(comPortEdit->Text,BaudRate9600,NO) )
        {
                ShapeConnect->Brush->Color = clRed;
                comPort->Close();
                delete comPort;
                comPort = NULL;
                ShowMessage("Ошибка открытия порта");
                return;
        }
//        Timer1->Enabled = true;
        ShapeConnect->Brush->Color = clGreen;
}
//---------------------------------------------------------------------------
unsigned char bBuffMax = 0;
unsigned char bBuff[256];
void __fastcall TForm1::BbSaveData()
{
        // var
        char lenS[8];
        int bufPos[8];
        int sTimeBegin[8];
        int sTimeEnd[8];
        int nSolushin;
        int napr, sFist, sNext;
        int fist, next, offsetd;
        // read date
        // simbol len
        for (int i=0; i<sizeof(_Sens.len); i++)
        {
                lenS[i] = bBuff[i+6];
        }
        // fist - 12
        fist = bBuff[12];
        // next - 13
        next = bBuff[13];
        // napr - 14
        napr = bBuff[14];
        // sFist - 15
        sFist = bBuff[15];
        // sNext - 16
        sNext = bBuff[16];
        // dochet - 17
        offsetd = 0;
        for (int b=0; b<2; b++)
        {
                ((unsigned char *)&offsetd)[b] = bBuff[b+17];
        }
        // rs_dat - 19
        // position sensors
        for (unsigned char i=0; i<8; i++)
        {
                bufPos[i] = 0;
                for (int b=0; b<2; b++)
                {
                        ((unsigned char *)&bufPos[i])[b] = bBuff[(2*i)+b+19];
                }
        }
        // dat_time - 35
        // time begin
        for (unsigned char i=0; i<8; i++)
        {
                sTimeBegin[i] = 0;
                for (int b=0; b<4; b++)
                {
                        ((unsigned char*)&sTimeBegin[i])[b] = bBuff[(4*i)+b+35];
                }
        }
        // time end
        for (unsigned char i=0; i<8; i++)
        {
                sTimeEnd[i] = 0;
                for (int b=0; b<4; b++)
                {
                        ((unsigned char*)&sTimeEnd[i])[b] = bBuff[(4*i)+b+67];
                }
        }
        // crc - 99
        //-------------------------------------------------------------------
        /*
        sprintf(lenS, "07.365");
        bufPos[0] = 1000;
        bufPos[1] = 2000;
        bufPos[2] = 3000;
        bufPos[3] = 4000;
        bufPos[4] = 5000;
        bufPos[5] = 6000;
        bufPos[6] = 10350;
        bufPos[7] = 10860;
        sTimeBegin[0] = 1;
        sTimeBegin[1] = 1623;
        sTimeBegin[2] = 2339;
        sTimeBegin[3] = 3603;
        sTimeBegin[4] = 4680;
        sTimeBegin[5] = 5935;
        sTimeBegin[6] = 11332;
        sTimeBegin[7] = 0;//11774;
        sTimeEnd[0] = 8995;
        sTimeEnd[1] = 10116;
        sTimeEnd[2] = 11345;
        sTimeEnd[3] = 12642;
        sTimeEnd[4] = 15021;
        sTimeEnd[5] = 17731;
        sTimeEnd[6] = 24628;
        sTimeEnd[7] = 24965;
        nSolushin = 8;
        modeRender = 1;
        fist = 2;
        next = 6;
        offsetd = 13;
        */
        //-------------------------------------------------------------------
        //save
        // simbol len
        for (int i=0; i<sizeof(_Sens.len); i++)
        {
                _Sens.len[i] = lenS[i];
        }
        // lens sensors
        for (unsigned char i=0; i<8; i++)
        {
                _Sens.sensor[i].position = bufPos[i];
        }
        // time begin
        for (unsigned char i=0; i<8; i++)
        {
                _Sens.sensor[i].timeBegin = sTimeBegin[i];
        }
        // time end
        for (unsigned char i=0; i<8; i++)
        {
                _Sens.sensor[i].timeEnd = sTimeEnd[i];
        }
        // mode render
        //_Sens.modeRender = modeRender;
        // fist
        _Sens.fist = fist;
        // next
        _Sens.next = next;
        // napr
        _Sens.napr = napr;
        // sFist
        _Sens.sFist = sFist;
        // sNext
        _Sens.sNext = sNext;
        // offset
        _Sens.offset = offsetd;
        TimerRender->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::EventNewDateComPort(int RdByte)
{
        unsigned char bByte;
        unsigned long zakaz, fact;
        while( comPort->ReadBuf(&bByte, &(zakaz=1), &fact)==0 )
        {
                if (bBuffMax==0)
                {
                        // shift
                        for (int i=1; i<5; i++)
                        {
                                bBuff[i-1] = bBuff[i];
                        }
                        bBuff[4] = bByte;
                        // find code
                        if ( bBuff[0]!=0xe6 )         continue;
                        if ( bBuff[1]!=0x0d )         continue;
                        if ( bBuff[3]!=0xaa )         continue;
                        if ( bBuff[4]!=0x55 )         continue;
                        eventNewDateInd = 5;
                        bBuffMax = bBuff[2]; // len
                        continue;
                }
                else
                {
                        bBuff[eventNewDateInd] = bByte;
                        eventNewDateInd++;
                        if (eventNewDateInd<bBuffMax)   continue;
                        bBuffMax = 0;
                        // checked crc8
                        if ( bBuff[eventNewDateInd-1]!=crc8_buf(bBuff, eventNewDateInd-2) )   continue;
                        if ( SpeedButton1->Down )
                        {
                                TFileStream *FileArhive = NULL;
                                AnsiString str = Edit15->Text;
    try
    {
        FileArhive = new TFileStream(str.c_str(), fmOpenWrite + fmShareDenyNone);
        FileArhive->Seek(FileArhive->Size,1);
    }
    catch(...)
    {
        FileArhive = NULL;
    }
    if (FileArhive==NULL)
    {
        try
        {
            FileArhive = new TFileStream(str.c_str(), fmCreate);
//            FileArhive->Write(headline1.c_str(),headline1.Length());
//            FileArhive->Write(headline2.c_str(),headline2.Length());
        }
        catch(...)
        {
            FileArhive = NULL;
        }
    }
    if (FileArhive)
    {
    FileArhive->Write(bBuff, eventNewDateInd);
    delete FileArhive;
    FileArhive = NULL;
    }
                        }
                        BbSaveData();
                }
        }
}
//---------------------------------------------------------------------------
int __fastcall mabs(int x)
{
        if (x<0)        x=-x;
        return x;
}
void __fastcall TForm1::TimerRenderTimer(TObject *Sender)
{
        int mapI[8], mapIi;
        ((TTimer *)Sender)->Enabled = false;
        // ===========================
        for (int stlb=0; stlb<8; stlb++)
        {
                StringGrid1->Cells[stlb+1][0] = IntToStr(stlb);
                StringGrid1->Cells[stlb+1][1] = IntToStr(_Sens.sensor[stlb].position);
                StringGrid1->Cells[stlb+1][2] = IntToStr(_Sens.sensor[stlb].timeBegin);
                StringGrid1->Cells[stlb+1][3] = IntToStr(_Sens.sensor[stlb].timeEnd);
                if (stlb<7)
                {
                        SpeedB[stlb]->Left = -SpeedB[stlb]->Width-50;
                        SpeedE[stlb]->Left = -SpeedE[stlb]->Width-50;
                }
        }
        // ===========================
        // position
        for (int i=0; i<8; i++)
        {
                SensInd[i]->Left = (_Sens.sensor[i].position*sensMetr/1000)+sensOffset-(SensInd[i]->Width/2);
        }
        // in & out
        mapIi = 0;
        for (int i=0; i<8; i++)
        {
                if ( _Sens.sensor[i].timeBegin==0 )
                        SensInd[i]->Brush->Color = clBlack;
                else
                {
                        mapI[mapIi] = i;
                        mapIi++;
                        if ( (i<_Sens.fist) || (i>_Sens.next) )
                                SensInd[i]->Brush->Color = clGreen;
                        else
                                SensInd[i]->Brush->Color = clLime;
                }
        }
        // tube
        PanelSpeed->Left = (_Sens.sensor[_Sens.sFist].position*sensMetr/1000)+sensOffset;
        PanelSpeed->Width = ((_Sens.sensor[_Sens.sNext].position-_Sens.sensor[_Sens.sFist].position)
        *sensMetr/1000)-(SensInd[_Sens.sNext]->Width/2)+sensOffset;
        int sLen, sTime, dochet;
        switch(_Sens.napr)
        {
                case (1) :
                        sLen  = _Sens.sensor[_Sens.sNext].position -_Sens.sensor[_Sens.sFist].position;
                        sTime = _Sens.sensor[_Sens.sNext].timeBegin-_Sens.sensor[_Sens.sFist].timeBegin;
                        dochet = _Sens.offset*sLen/sTime;
                        break;
                case (2) :
                        sLen  = _Sens.sensor[_Sens.sNext].position-_Sens.sensor[_Sens.sFist].position;
                        sTime = _Sens.sensor[_Sens.sNext].timeEnd -_Sens.sensor[_Sens.sFist].timeEnd;
                        dochet = _Sens.offset*sLen/sTime;
                        break;
                case (3) :
                        sLen  = _Sens.sensor[_Sens.sNext].position-_Sens.sensor[_Sens.sFist].position;
                        sTime = _Sens.sensor[_Sens.sNext].timeEnd -_Sens.sensor[_Sens.sFist].timeEnd;
                        dochet = _Sens.offset*sLen/sTime;
                        break;
                case (4) :
                        sLen  = _Sens.sensor[_Sens.sNext].position-_Sens.sensor[_Sens.sFist].position;
                        sTime = _Sens.sensor[_Sens.sNext].timeEnd -_Sens.sensor[_Sens.sFist].timeEnd;
                        dochet = -_Sens.offset*sLen/sTime;
                        break;
                case (5) :
                        sLen  = _Sens.sensor[_Sens.sNext].position -_Sens.sensor[_Sens.sFist].position;
                        sTime = _Sens.sensor[_Sens.sNext].timeBegin-_Sens.sensor[_Sens.sFist].timeBegin;
                        dochet = -_Sens.offset*sLen/sTime;
                        break;
                case (6) :
                        sLen  = _Sens.sensor[_Sens.sNext].position -_Sens.sensor[_Sens.sFist].position;
                        sTime = _Sens.sensor[_Sens.sNext].timeBegin-_Sens.sensor[_Sens.sFist].timeBegin;
                        dochet = _Sens.offset*sLen/sTime;
                        break;
                case (7) :
                        sLen  = _Sens.sensor[_Sens.sNext].position -_Sens.sensor[_Sens.sFist].position;
                        sTime = _Sens.sensor[_Sens.sNext].timeBegin-_Sens.sensor[_Sens.sFist].timeBegin;
                        dochet = _Sens.offset*sLen/sTime;
                        break;
                case (8) :
                        sLen  = _Sens.sensor[_Sens.sNext].position-_Sens.sensor[_Sens.sFist].position;
                        sTime = _Sens.sensor[_Sens.sNext].timeEnd -_Sens.sensor[_Sens.sFist].timeEnd;
                        dochet = -_Sens.offset*sLen/sTime;
                        break;
                case (9) :
                        sLen  = _Sens.sensor[_Sens.sNext].position-_Sens.sensor[_Sens.sFist].position;
                        sTime = _Sens.sensor[_Sens.sNext].timeEnd -_Sens.sensor[_Sens.sFist].timeEnd;
                        dochet = _Sens.offset*sLen/sTime;
                        break;
                case (10) :
                        sLen  = _Sens.sensor[_Sens.sNext].position -_Sens.sensor[_Sens.sFist].position;
                        sTime = _Sens.sensor[_Sens.sNext].timeBegin-_Sens.sensor[_Sens.sFist].timeBegin;
                        dochet = -_Sens.offset*sLen/sTime;
                        break;
                case (11) :
                        sLen  = _Sens.sensor[_Sens.sNext].position -_Sens.sensor[_Sens.sFist].position;
                        sTime = _Sens.sensor[_Sens.sNext].timeBegin-_Sens.sensor[_Sens.sFist].timeBegin;
                        dochet = -_Sens.offset*sLen/sTime;
                        break;
                case (12) :
                        sLen  = _Sens.sensor[_Sens.sNext].position -_Sens.sensor[_Sens.sFist].position;
                        sTime = _Sens.sensor[_Sens.sNext].timeBegin-_Sens.sensor[_Sens.sFist].timeBegin;
                        dochet = -_Sens.offset*sLen/sTime;
                        break;
        }
        if ( (_Sens.sFist==_Sens.next) && (dochet<0) )
        {
                PanelTube->Left = ( (_Sens.sensor[_Sens.fist].position-dochet// +mabs(dochet)
                                        )*sensMetr/1000)+sensOffset;
                PanelTube->Width = ((_Sens.sensor[_Sens.next].position-_Sens.sensor[_Sens.fist].position
                                     +dochet)*sensMetr/1000);
        }
        else
        {
                PanelTube->Left = (_Sens.sensor[_Sens.fist].position*sensMetr/1000)+sensOffset;
                PanelTube->Width = ((_Sens.sensor[_Sens.next].position-_Sens.sensor[_Sens.fist].position
                                     +dochet)*sensMetr/1000);
        }
        L_fist->Caption = _Sens.fist;
        L_next->Caption = _Sens.next;
        Label2->Caption =  _Sens.napr;
        L_sFist->Caption = _Sens.sFist;
        L_sNext->Caption = _Sens.sNext;
        //L_OffSet->Caption = _Sens.offset;
        L_OffSet->Caption = dochet;
        Label1->Caption = _Sens.len;
        // speed
        for (int n=1; n<mapIi; n++)
        {
                int d1 = mapI[n-1];
                int d2 = mapI[n];
                int l = _Sens.sensor[d2].position-_Sens.sensor[d1].position;
                int tB = _Sens.sensor[d2].timeBegin-_Sens.sensor[d1].timeBegin;
                int tE = _Sens.sensor[d2].timeEnd-_Sens.sensor[d1].timeEnd;
                SpeedB[n-1]->Text = (float)l*Edit_kspeed->Text.ToInt()/tB;
                int sh = SensInd[d2]->Left-SensInd[d1]->Left-SpeedB[n-1]->Width;
                SpeedB[n-1]->Left = SensInd[d1]->Left+(sh/2)+sensOffset;//+(Shape1->Width/2);
                SpeedE[n-1]->Text = (float)l*Edit_kspeed->Text.ToInt()/tE;
                sh = SensInd[d2]->Left-SensInd[d1]->Left-SpeedE[n-1]->Width;
                SpeedE[n-1]->Left = SensInd[d1]->Left+(sh/2)+sensOffset;//+(Shape1->Width/2);
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
        if (SaveDialog1->Execute())
        {
                Edit15->Text = SaveDialog1->FileName;
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton1Click(TObject *Sender)
{
        if ( SpeedButton1->Down )
        {
                Shape9->Brush->Color = clLime;
        }
        else
        {
                Shape9->Brush->Color = clRed;
        }
}
//---------------------------------------------------------------------------

