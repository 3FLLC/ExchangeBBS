{% MainUnit: welcome.p}

{$IFDEF CODERUNNER}
   For xyloop:=5 to 17 do
      ClearLine(xyloop,7,0);
/////////////////////////////////////////////////////////////////////////////
// DETECT ANSI COLOR CAPABILITY:
/////////////////////////////////////////////////////////////////////////////
   FastWrite(1,5,'|0D'+i18n.getValues('colors')+':'+#13#10#13#10);
   FastWriteln('|5E8|0E '+i18n.getValues('bgcolors')+':');
   FastWriteln('|0F 01 |1F 02 |2F 03 |3F 04 |4F 05 |5F 06 |6F 07 |7F 08 |0F');
   FastWriteln(#13#10+'|5E16|0E '+i18n.getValues('bgcolors')+' |0E(|0A'+i18n.getValues('ifblinking')+'|0E)|00');
   FastWriteln(#27'[100m  9 '+#27+'[104m 10 '+#27+'[102m 11 '+#27+'[106m 12 '+#27+'[101m 13 '+#27+'[105m 14 '+#27+'[103m 15 '+#27+'[107m 16 |0F'+#27+'[0m');
   FastWriteln(#13#10+'|5E256|0E '+i18n.getValues('bgcolors')+' |0E(|0A'+i18n.getValues('justafew')+'|0E)|00');
   FastWriteln(#27'[48;5;10m 10 '+#27+'[48;5;30m 30 '+#27+'[48;5;50m 50 '+#27+'[48;5;70m 70 '+#27+'[48;5;90m 90 '+#27+'[48;5;110m110 '+#27+'[48;5;130m130 '+#27+'[48;5;140m140 |0F'+#27+'[0m');
   FastWriteln(#13#10+'|5ETrue Color|0E '+i18n.getValues('bgcolors')+' |0E(|0A'+i18n.getValues('justafew')+'|0E)|00');
   FastWriteln(#27'[48;2;255;87;51m orange '+#27+'[48;2;51;184;255m azule '+#27+'[48;2;233;51;255m purple '+#27+'[48;2;51;255;196m ocean grean '+#27+'[0m|0F'+#27+'[0m'+#13#10);
   Ws:=Session.Ask('|0A'+i18n.getValues('displaycorrectly')+'|0D? |0F(|0E8, 16, 256 or TC|0F)|0B','   ',False,40);
   If Ws='' then begin
      Session.Disconnect;
      Exit;
   End;
   Case Ws[1] of
      '8':Begin
         Chain.Store('ANSI8',True);
         Chain.Store('ANSI16',False);
         Chain.Store('ANSI256',False);
         Chain.Store('ANSITC',False);
      End;
      '1':Begin
         Chain.Store('ANSI8',True);
         Chain.Store('ANSI16',True);
         Chain.Store('ANSI256',False);
         Chain.Store('ANSITC',False);
      End;
      '2':Begin
         Chain.Store('ANSI8',True);
         Chain.Store('ANSI16',True);
         Chain.Store('ANSI256',True);
         Chain.Store('ANSITC',False);
      End;
      'T','t':Begin
         Chain.Store('ANSI8',True);
         Chain.Store('ANSI16',True);
         Chain.Store('ANSI256',True);
         Chain.Store('ANSITC',True);
      End;
      else Begin
         Chain.Store('ANSI8',True);
         Chain.Store('ANSI16',False);
         Chain.Store('ANSI256',False);
         Chain.Store('ANSITC',False);
      End;
   End;
{$ELSE}
   Title;
   Chain.Store('ANSI8',True);
   Chain.Store('ANSI16',True);
   Chain.Store('ANSI256',False);
   Chain.Store('ANSITC',False);
{$ENDIF}
