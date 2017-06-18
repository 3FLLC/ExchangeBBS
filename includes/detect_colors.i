{$IFDEF CODERUNNER}
/////////////////////////////////////////////////////////////////////////////
// DETECT ANSI COLOR CAPABILITY:
/////////////////////////////////////////////////////////////////////////////
   FastWriteln(#13#10+'|09Checking if your terminal supports 8, 16, 256, or True Color background colors:');
   FastWriteln('|0F8 Background Colors:');
   FastWriteln('|0F 01 |1F 02 |2F 03 |3F 04 |4F 05 |5F 06 |6F 07 |7F 08 |0F');
   FastWriteln('|0F16 Background Colors |0E(|0Ashowing 9 to 16, if it blinks, you do not support it|0E)|00');
   FastWriteln(#27'[100m  9 '+#27+'[104m 10 '+#27+'[102m 11 '+#27+'[106m 12 '+#27+'[101m 13 '+#27+'[105m 14 '+#27+'[103m 15 '+#27+'[107m 16 |0F');
   FastWriteln('|0F256 Background Colors |0E(|0Ashowing just a few colors|0E)|00');
   FastWriteln(#27'[48;5;10m 10 '+#27+'[48;5;30m 30 '+#27+'[48;5;50m 50 '+#27+'[48;5;70m 70 '+#27+'[48;5;90m 90 '+#27+'[48;5;110m110 '+#27+'[48;5;130m130 '+#27+'[48;5;140m140 |0F');
   FastWriteln('|0FTrue Color Background Colors |0E(|0Ashowing just a few colors|0E)|00');
   FastWriteln(#27'[48;2;255;87;51m orange '+#27+'[48;2;51;184;255m azule '+#27+'[48;2;233;51;255m purple '+#27+'[48;2;51;255;196m ocean grean '+#27+'[0m|0F');
   Case Session.Ask('|0AWhich background display correctly|0D? |0F(|0E8, 16, 256 or TC|0F)|0B','   ',False,40)[1] of
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
   Chain.Store('ANSI8',True);
   Chain.Store('ANSI16',True);
   Chain.Store('ANSI256',False);
   Chain.Store('ANSITC',False);
{$ENDIF}
