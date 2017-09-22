{% MainUnit: welcome.p}

{$IFDEF CODERUNNER}
/////////////////////////////////////////////////////////////////////////////
// DETECT ANSI MODE:
/////////////////////////////////////////////////////////////////////////////
   Session.ReadStr(Session.CountWaiting,100); // Flush Buffer - could be FIDO //
   Session.Writeln(#27+'[3h'+#12'Detecting ANSI... Hello, Hola, Salut...');
   Yield(100);
   Session.AnsiWhereXY; // Ask terminal where cursor is (if it supports ANSI)
   Yield(300);
   Session.Writeln('We also detect EMSI, WaZoo and YooHoo');
   Yield(300);
   Session.AnsiClrScr;
   Yield(350);
   Chain.Store('HASUTF8',False);
   If Session.CountWaiting=0 then Begin
      Session.SetASCIIMode;
      Chain.Store('HASANSI',False);
   end
   else begin
      Session.ReadStr(Session.CountWaiting,100);
      Session.SetANSIMode;
      Chain.Store('HASANSI',True);
   end;
   Title;

   Session.AnsiPipeWriteAt(11,5,'|0CRegistered: |0A50,000 NODES |0C(|08Build: |071170914|0C) |08Open Source');
   Session.AnsiPipeWriteAt(11,6,'|0E(c)opyright 1983 to 2017 |0Bby MP Solutions, LLC.');
   Session.AnsiPipeWriteAt(11,7,'|0FProgramming and Design: |0Bby G.E. Ozz Nixon Jr.|0A');
   Session.AnsiPipeWriteAt(11,8,'|0DThe Internet BBS for |0BWindows, |0AMac OS X, |0ELinux|0A'+#13#10#13#10);

   FastWrite(1,10,'Language Awareness:'+#13#10+
      '    |0F(|0C1|0F)|08.....|0EEnglish    |0F(|0C4|0F)|08.....|0EItalian    |0F(|0C7|0F)|08........|0EJive'+#13#10+
      '    |0F(|0C2|0F)|08.....|0ESpanish    |0F(|0C5|0F)|08......|0EGerman'+#13#10+
      '    |0F(|0C3|0F)|08......|0EFrench    |0F(|0C6|0F)|08.....|0ESwedish'+#13#10#13#10
      '    |0C(|0E0|0C)|08....|0DEnd Call|0B'+#13#10#13#10);
   If not Session.Connected then Exit;
   Ws:=Ask('Language? ','##');
   If Ws='' then Ws:='0';
   Case StrToIntDef(Ws,0) of
      0:Begin
         Session.Write(#13#10#13#10#7+'Goodbye.');
         Yield(300);
         Session.Write(#7+' Adios.');
         Yield(300);
         Session.Writeln(#7+' Au Revoir.');
         Yield(300);
         Session.Disconnect;
         Exit;
      End;
      1:i18n.loadFromFile(ScriptRoot+'languages/en.txt');
      2:i18n.loadFromFile(ScriptRoot+'languages/es.txt');
      3:i18n.loadFromFile(ScriptRoot+'languages/fr.txt');
      98:Begin
         Session.SetUTF8Mode;
         CLS;
         var Offset:=1;
         For var Loop:=174 to 255 do begin
            Session.AnsiPipeWriteAt(Offset,Loop mod 16,IntToStr(Loop)+'='+Chr(Loop));
            If Loop mod 16=0 then Inc(Offset,8);
         End;
         Session.Disconnect;
         Exit;
      End;
      else i18n.loadFromFile(ScriptRoot+'languages/en.txt');
   End;
   For var xyloop:=10 to 17 do
      ClearLine(xyloop,7,0);
   FastWrite(1,10,'|0F'+i18n.getValues('characterset')+'|0B'+#13#10#13#10);
   Session.Writeln(' +---+   '+#201#205#205#205#187+'   '#226#149#148#226#149#144#226#149#144#226#149#144#226#149#151);
   Session.Writeln(' | A |   '+#186#32#66#32#186   +'  '#226#149#145+' C '+#226#149#145);
   Session.Writeln(' +---+   '+#200#205#205#205#188+'   '#226#149#154#226#149#144#226#149#144#226#149#144#226#149#157);
   Session.Writeln('');
   Ws:=Ask('|0A'+i18n.getValues('whichcharset')+'|0D? |0F(|0EA,B, or C|0F)|0B',' ')
   If Ws='' then begin
      Session.Disconnect;
      Exit;
   End
   Else Case UpCase(Ws[1]) of
      'A':Begin
         Session.SetASCIIMode;
         Chain.Store('ASCII',True);
         Chain.Store('HIGH8',False);
         Chain.Store('HASUTF8',False);
      End;
      'B':Begin
         Session.SetANSIMode;
         Chain.Store('ASCII',True);
         Chain.Store('HIGH8',True);
         Chain.Store('HASUTF8',False);
      End;
      'C':Begin
         Session.SetUTF8Mode;
         Chain.Store('ASCII',True);
         Chain.Store('HIGH8',False);
         Chain.Store('HASUTF8',True);
      End;
   End;
{$ELSE}
   Chain.Store('ASCII',True);
   Chain.Store('HIGH8',False);
   Chain.Store('UTF8',True);
{$ENDIF}
