//---------------------------------------------------------------------------


#pragma hdrstop

#include "ComPort.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

__fastcall TvMass::TvMass(int n)
{
        len = n;
        vsMem = new TvsMem[len];
        for (int i=0;i<len;i++)
        {
                vsMem[i].UkMass = NULL;
                vsMem[i].LenMass = 0;
        }
        ind_rd = 0;
        ind_wr = 0;
        InitializeCriticalSection(&csvm);
}

__fastcall TvMass::~TvMass()
{
        for (int i=0;i<len;i++)
        {
                if (vsMem[i].LenMass>0)
                {
                        delete[] vsMem[i].UkMass;
                        vsMem[i].LenMass = 0;
                }
        }
        delete[] vsMem;
        DeleteCriticalSection(&csvm);
}

int __fastcall TvMass::adr(int adr)
{
        if (adr>=0)
        {
                while( adr>=len )
                        adr = adr - len;
        }
        else
        {
                while( adr<0 )
                        adr = adr + len;
        }
        return adr;
}

void __fastcall TvMass::Push(unsigned char *buff, int lens)
{
        EnterCriticalSection(&csvm);
        int future =adr(ind_wr+1);
        if ( future!=ind_rd )
        {
                vsMem[ind_wr].UkMass = new unsigned char[lens];
                vsMem[ind_wr].LenMass = lens;
                for (int i=0;i<lens;i++)
                        vsMem[ind_wr].UkMass[i] = buff[i];
                ind_wr = future;
        }
        LeaveCriticalSection(&csvm);
}
int  __fastcall TvMass::ReadCurent(unsigned char **buff, int &lens)
{
        int stat = 1;
        EnterCriticalSection(&csvm);
        if ( ind_rd!=ind_wr )
        {
                *buff = vsMem[ind_rd].UkMass;
                lens = vsMem[ind_rd].LenMass;
                stat = 0;
        }
        LeaveCriticalSection(&csvm);
        return stat;
}
int  __fastcall TvMass::BuffLevel()
{
        EnterCriticalSection(&csvm);
        int temp = adr(ind_wr-ind_rd);
        LeaveCriticalSection(&csvm);
        return temp;
}
void __fastcall TvMass::ReadNext()
{
        EnterCriticalSection(&csvm);
        if ( ind_rd!=ind_wr )
        {
                delete[] vsMem[ind_rd].UkMass;
                vsMem[ind_rd].UkMass = NULL;
                vsMem[ind_rd].LenMass = 0;
                ind_rd = adr(ind_rd+1);
        }
        LeaveCriticalSection(&csvm);
}





__fastcall TComPort::TComPort()
{
        COMport = NULL;
        NitRead = NULL;
        NitRead = NULL;
        EventNewDate = NULL;
        //
        OutxCtsFlow = false;
        OutxDsrFlow = false;
        DTR_CONTROL = DTR_DISABLE;
        DsrSensitivity = false;
        TXContinueOnXoff = true;
        OutX = false;
        InX = false;
        ErrorChar = false;
        pNull = false;
        RTS_CONTROL = RTS_DISABLE;
        AbortOnError = false;
        pXonLim = 2048;
        pXoffLim = 512;
        ByteSize = ByteSize8;
        StopBits = StopBitsTWO;
        pXonChar = 17;
        pXoffChar = 19;
        pErrorChar = 0;
        pEofChar = 0;
        pEvtChar = 0;
        //
}

__fastcall TComPort::~TComPort()
{
        if (NitRead) {
                if (NitRead->Suspended) NitRead->Resume();
                //NitRead->Terminate();
        }
        if (NitWrite) {
                if (NitWrite->Suspended)
                {
                        NitWrite->Resume();
                        Sleep(1000);
                }
                NitWrite->Terminate();
        }
        if (NitRead) {
                //NitRead->WaitFor();
                //NitRead = NULL;
        }
        if (NitWrite) {
                NitWrite->WaitFor();
                //while ( NitWrite->work )
                  //      Sleep(10);
                NitWrite = NULL;
        }
        if (COMport) {
                CloseHandle(COMport);
                COMport = NULL;
        }
        CloseHandle(COMport);
        COMport = NULL;
}

int  __fastcall TComPort::Open(AnsiString PortName,
                               eBaudRate  BaudRate,
                               eParity    Parity)
{
        if (COMport) {
                Close();
        }
        COMport = CreateFile( PortName.c_str(),
                              GENERIC_READ | GENERIC_WRITE,
                              0,
                              NULL,
                              OPEN_EXISTING,
                              FILE_FLAG_OVERLAPPED, //FILE_ATTRIBUTE_NORMAL, //NULL,
                              NULL );
        if (COMport == INVALID_HANDLE_VALUE) {
                COMport = NULL;
                return 1;
        }
        //
        DCB dcb;
        dcb.DCBlength = sizeof(DCB);
        if (!GetCommState(COMport, &dcb) ) {
                Close();
                return 2;
        }
        dcb.BaudRate = BaudRate;
        dcb.fParity = Parity;
        //
        dcb.fOutxCtsFlow      = OutxCtsFlow;
        dcb.fOutxDsrFlow      = OutxDsrFlow;
        dcb.fDtrControl       = DTR_CONTROL;
        dcb.fDsrSensitivity   = DsrSensitivity;
        dcb.fTXContinueOnXoff = TXContinueOnXoff;
        dcb.fOutX             = OutX;
        dcb.fInX              = InX;
        dcb.fErrorChar        = ErrorChar;
        dcb.fNull             = pNull;
        dcb.fRtsControl       = RTS_CONTROL;
        dcb.fAbortOnError     = AbortOnError;
        dcb.XonLim            = pXonLim;
        dcb.XoffLim           = pXoffLim;
        dcb.ByteSize          = ByteSize;
        dcb.StopBits          = StopBits;
        dcb.XonChar           = pXonChar;
        dcb.XoffChar          = pXoffChar;
        dcb.ErrorChar         = pErrorChar;
        dcb.EofChar           = pEofChar;
        dcb.EvtChar           = pEvtChar;
        if (!SetCommState(COMport, &dcb)) {
                Close();
                return 3;
        }
        //
        COMMTIMEOUTS touts;
        touts.ReadIntervalTimeout         = 0;
        touts.ReadTotalTimeoutMultiplier  = 0;
        touts.ReadTotalTimeoutConstant    = 0;
        touts.WriteTotalTimeoutMultiplier = 0;
        touts.WriteTotalTimeoutConstant   = 0;
        if (!SetCommTimeouts(COMport, &touts)) {
                Close();
                return 4;
        }
        //
        SetupComm(COMport, 4096, 4096);
        PurgeComm(COMport, PURGE_RXCLEAR);
        //
        //NitRead = new TComPortRd(COMport);
        //NitRead->EventNewDate = this->fEventNewDate;
        NitWrite = new TComPortWr(COMport);

        //NitRead->Resume();
        NitWrite->Resume();
//        NitRead->setevent();
        return 0;
}

void __fastcall TComPort::Close()
{
        if (NitRead) {
                NitRead->Terminate();
                NitRead->Resume();
                NitRead = NULL;
        }
        if (COMport)
        {
                CloseHandle(COMport);
        }
}
// callback функция пришли новые данные
void __fastcall TComPort::fEventNewDate(int RdByte)
{
        if (EventNewDate) EventNewDate(RdByte);
}
//
int  __fastcall TComPort::ReadBuf(unsigned char *Buf, DWORD *len_zakaz, DWORD *len_ok)
{
        if ( (NitRead) && (COMport) )
        {
                return NitRead->ReadBuf(Buf, len_zakaz, len_ok);
        }
        else
        {
                *len_ok = 0;
                return 1;
        }
}

int  __fastcall TComPort::WriteBufferLevel()
{
        return NitWrite->Mass->BuffLevel();
}


//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TComPortRd::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TComPortRd::TComPortRd(HANDLE rdComPort)
        : TThread(true)
{
        ComPort = rdComPort;
        EventNewDate = NULL;
        // FreeOnTerminate = true;
        work = true;
}
//---------------------------------------------------------------------------
void __fastcall TComPortRd::Execute()
{
        //---- Place thread code here ----
        //
        over.hEvent = CreateEvent(NULL, true, true, NULL);
        SetCommMask(ComPort, EV_RXCHAR);
        while (!Terminated)
        {
                WaitCommEvent(ComPort, &mask, &over);
                signal = WaitForSingleObject(over.hEvent, 1500);
                if (signal==WAIT_OBJECT_0)
                {
                        if (GetOverlappedResult(ComPort, &over, &temp1, true) )
                        {
                                //
                                if ( (mask & EV_RXCHAR) != 0)
                                {
                                        ClearCommError(ComPort, &temp2, &curstat);
                                        btr = curstat.cbInQue;
                                        if (btr) Synchronize(EventNewDateSynhr);
                                }
                        }
                }
                else
                {
                        Sleep(10);
                }
        }
        // закрыть событие
        CloseHandle(over.hEvent);
        work = false;
}
void __fastcall TComPortRd::EventNewDateSynhr()
{
        EventNewDate(btr);
}
int  __fastcall TComPortRd::ReadBuf(unsigned char *Buf, DWORD *len_zakaz, DWORD *len_ok)
{
        DWORD Len;
        int stat;
        ClearCommError(ComPort, &temp2, &curstat);
        Len = curstat.cbInQue;
        if ( (*len_zakaz)>Len ) *len_zakaz = Len;
        if ( (*len_zakaz)>0 )
        {
                stat = ReadFile(ComPort, Buf, *len_zakaz, len_ok, &over);
                if (stat) stat = 0;
                else
                {
                        stat = 1;
                        len_ok = 0;
                }
        }
        else
        {
                stat = 1;
                len_ok = 0;
        }
        return stat;
}
//---------------------------------------------------------------------------
void __fastcall TComPort::wr(unsigned char *Buf, DWORD len)
{
        if (NitWrite)
        {
                NitWrite->WriteBuff(Buf, len);
        }
}






//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TComPortWr::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TComPortWr::TComPortWr(HANDLE wrComPort)
        : TThread(true)
{
        ComPort = wrComPort;
        //FreeOnTerminate = true;
        //Mass = new TvMass(32);
        work = true;
}
//---------------------------------------------------------------------------
void __fastcall TComPortWr::Execute()
{
        unsigned char *BuffWr;
        int dlina;
        DWORD temp, signal;
        bool flnowr;
        over.hEvent = CreateEvent(NULL, true, true, NULL);
        flnowr = true;
        //Suspend();
        //---- Place thread code here ----
        while (!Terminated)
        {
                if (flnowr)
                {
                        //if ( Mass->ReadCurent(&BuffWr, dlina) )
                        //{
                                Sleep(10);
                        //        continue;
                        //}
                        WriteFile(ComPort, BuffWr, dlina, &temp, &over);
                        flnowr = false;
                }
                if (!flnowr)
                {
                        signal = WaitForSingleObject(over.hEvent, 100);
                        if((signal == WAIT_OBJECT_0) && (GetOverlappedResult(ComPort, &over, &temp, true)))
                        {
                                //ok
                                flnowr = true;
                                Mass->ReadNext();
                        }
                        else
                        {
                                //false
                        }
                }
        }
        //delete[] Mass;
        // закрыть событие
        CloseHandle(over.hEvent);
        work = false;
}
//---------------------------------------------------------------------------
void __fastcall TComPortWr::WriteBuff(unsigned char *Buf, DWORD len)
{
        Mass->Push(Buf, len);
        Resume();
        /*
        DWORD dwBytesWritten;
        WriteFile(COMport, Buf, len, &dwBytesWritten, NULL );
        return dwBytesWritten;
        */
}


