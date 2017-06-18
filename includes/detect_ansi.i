{$IFDEF CODERUNNER}
/////////////////////////////////////////////////////////////////////////////
// DETECT ANSI MODE:
/////////////////////////////////////////////////////////////////////////////
   Session.Writeln('Detecting ANSI...');
   Session.ReadStr(Session.CountWaiting,100); // Flush Buffer - could be FIDO //
   Session.AnsiWhereXY; // Ask terminal where cursor is (if it supports ANSI)
   Session.AnsiClrScr;
   Session.AnsiPipeWriteAt(11,1,'|0EExchangeBBS v0.1a2 (Build: 1170618)');
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
   FastWriteln('|09Checking if your terminal supports IBM extended ASCII or UTF-8...|0B');
   Session.Writeln(' +---+   '+#201#205#205#205#187+'   '#226#149#148#226#149#144#226#149#144#226#149#144#226#149#151);
   Session.Writeln(' | A |   '+#186#32#66#32#186   +'   '#226#149#145+' C '+#226#149#145);
   Session.Writeln(' +---+   '+#200#205#205#205#188+'   '#226#149#154#226#149#144#226#149#144#226#149#144#226#149#157);
   Session.Writeln('');
   Case UpCase(Session.Ask('|0AWhich graphics box look nicests|0D? |0F(|0EA,B, or C|0F)|0B',' ',False,40)[1]) of
      'A':Begin
         Session.SetASCIIMode;
         Chain.Store('ASCII',True);
         Chain.Store('HIGH8',False);
         Chain.Store('UTF8',False);
      End;
      'B':Begin
         Session.SetANSIMode;
         Chain.Store('ASCII',True);
         Chain.Store('HIGH8',True);
         Chain.Store('UTF8',False);
      End;
      'C':Begin
         Session.SetUTF8Mode;
         Chain.Store('ASCII',True);
         Chain.Store('HIGH8',False);
         Chain.Store('UTF8',True);
      End;
   End;
{$ELSE}
   Chain.Store('ASCII',True);
   Chain.Store('HIGH8',False);
   Chain.Store('UTF8',True);
{$ENDIF}
