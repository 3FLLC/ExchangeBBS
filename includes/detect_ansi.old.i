{$IFDEF CODERUNNER}
/////////////////////////////////////////////////////////////////////////////
// DETECT ANSI MODE:
/////////////////////////////////////////////////////////////////////////////
   Session.Writeln('Detecting ANSI...');
   Session.ReadStr(Session.CountWaiting,100); // Flush Buffer - could be FIDO //
   Session.AnsiWhereXY; // Ask terminal where cursor is (if it supports ANSI)
   Session.AnsiClrScr;
   Session.AnsiPipeWriteAt(11,1,'|0EExchangeBBS v0.1a2 (Build: 1170505)');
   Session.AnsiPipeWriteAt(20,2,'|0C(c) 2015, 2016 and 2017 |0Bby MP Solutions, LLC.'+#13#10#13#10);
   Yield(350);
   If Session.CountWaiting=0 then Begin
      Session.SetASCIIMode;
      Chain.Store('HASANSI',False);
   end
   else begin
      Session.ReadStr(Session.CountWaiting,100);
      Session.SetANSIMode;
      Chain.Store('HASANSI',True);
   end;
   Session.Writeln('Checking if your terminal supports IBM extended ASCII or UTF-8...');
   If Session.Ask('Is this a diamond-question mark: '+#188#207,'YN',False,40)='Y' then Begin
      Session.SetUTF8Mode;
      Chain.Store('UTF8',True);
   End
   Else Chain.Store('UTF8',False);
{$ENDIF}
