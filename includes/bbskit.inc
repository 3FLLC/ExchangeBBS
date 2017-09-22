Procedure Beep;
Begin
{$IFDEF CODERUNNER}
   Session.Write(#7);
{$ELSE}
   Write(#7);
{$ENDIF}
End;

Procedure NewLine;
Begin
{$IFDEF CODERUNNER}
   Session.Writeln('');
{$ELSE}
   Writeln();
{$ENDIF}
End;

PROCEDURE BackSpace(Bksp:String);
BEGIN
   IF Bksp='' THEN Bksp:=#32;
{$IFDEF CODERUNNER}
   Session.Write(Bksp);
{$ELSE}
   Write(Bksp);
{$ENDIF}
END;

Function CountColors(S:String):Longint;
Begin
   For var loop:=1 to Length(S) do
      if S[Loop]='|' then Inc(Result);
End;

Procedure HangUp;
Begin
{$IFDEF CODERUNNER}
   Session.Disconnect;
{$ENDIF}
end;

Function Connected:Boolean;
Begin
{$IFDEF CODERUNNER}
   Result:=Session.IsConnected;
{$ELSE}
   Result:=True;
{$ENDIF}
end;

Function KeyWaiting:Boolean;
Begin
{$IFDEF CODERUNNER}
   Result:=Session.Keypressed;
{$ELSE}
   Result:=Keypressed;
{$ENDIF}
end;

Function GetChar:Char;
{$IFDEF CODERUNNER}
var
   Ws:String;
{$ENDIF}

Begin
   SetCursor(True);
   If KeyWaiting then Begin
{$IFDEF CODERUNNER}
      Ws:=Session.ReadStr(1,500);
      If Ws='' then Result:=#0
      else Result:=Ws[1];
{$ELSE}
      Result:=Readkey;
{$ENDIF}
   End;
   SetCursor(False);
End;

Function OneKey(S:String;B:Boolean):Char;
var
{$IFDEF CODERUNNER}
   Arr:TCharArray;
   Loop:Longint;
{$ELSE}
   Ch:Char;
{$ENDIF}

Begin
   SetCursor(True);
{$IFDEF CODERUNNER}
   SetLength(Arr,Length(S));
   For Loop:=1 to Length(S) do Arr[Loop-1]:=S[Loop];
   Result:=Uppercase(Session.ReadChar(Arr,120))[1];
{$ELSE}
   While True do Begin
      Ch:=UpCase(ReadKey);
      If Pos(Ch,S)>0 then begin
         Result:=Ch;
         SetCursor(False);
         Exit;
      End;
   End;
{$ENDIF}
   SetCursor(False);
End;

procedure Pause(New:Boolean=False);
Begin
   SetCursor(False);
   If not new then CursorTo(1,24);
{$IFDEF CODERUNNER}
   If Session.CountWaiting then Session.ReadStr(Session.CountWaiting,500);
   Session.Prompt('|0D=|0APaused|0D= |0BPress any key to continue',300);
   If Session.CountWaiting then Session.ReadStr(Session.CountWaiting,500);
{$ELSE}
   FastWrite('|0D=|0APaused|0D= |0BPress any key to continue');
   ReadKey;
{$ENDIF}
   if not new then ClearLine(24,7,0);
end;

Function PauseBar(New:Boolean=False):String; // C or S or '' (disconnected).
var
   Ws:String;
   IsContinue:Boolean;

Begin
   SetCursor(False);
   If not new then CursorTo(1,24);
   IsContinue:=True;
{$IFDEF CODERUNNER}
   If Session.CountWaiting then Session.ReadStr(Session.CountWaiting,500);
   Session.PipeWrite('|0D=|0APaused|0D= |1E(C)|1Fontinue|0A |0C(S)|07top|0A');
   While Connected do Begin
       If KeyWaiting then begin
          Ws:=Session.ReadStr(Session.CountWaiting,500);
          If Ws=#27+'[C' then IsContinue:=not IsContinue {right arrow in ANSI}
          else If Ws=#27+'[D' then IsContinue:=not IsContinue {left arrow in ANSI}
          else If (Ws='C') or (Ws='c') then Begin
              IsContinue:=True;
              Ws:=#13;
          end
          else If (Ws='S') or (Ws='s') then Begin
              IsContinue:=False;
              Ws:=#13;
          end
          else If Copy(Ws,1,1)=#13 then begin
             If IsContinue then Result:='C'
             Else Result:='S';
             break;
          End;
          Session.Write(#27+'[26D');
          If IsContinue then Session.PipeWrite('|0D=|0APaused|0D= |1E(C)|1Fontinue|0A |0C(S)|07top|0A')
          Else Session.PipeWrite('|0D=|0APaused|0D= |07(C)|08ontinue|0A |1E(S)|1Ftop|0A');
       End
   End;
{$ELSE}
   Session.PipeWrite('|0D=|0APaused|0D= |1EC|1Fontinue|0A |1CS|17top|0A');
   ReadKey;
{$ENDIF}
   if not new then ClearLine(24,7,0);
end;

function Ask(Q,M:String):String; overload;
begin
   SetCursor(True);
{$IFDEF CODERUNNER}
   Result:=Session.Ask(Q,M,False,180);
{$ELSE}
   Result:=System.Ask(Q,M,False,180);
{$ENDIF}
   SetCursor(False);
end;

function Ask(X,Y:Byte;M:String):String; overload;
begin
   SetCursor(True);
   CursorTo(X,Y);
{$IFDEF CODERUNNER}
   Result:=Session.Ask('',M,False,180);
{$ELSE}
   Result:=System.Ask('',M,False,180);
{$ENDIF}
   SetCursor(False);
end;

function Ask2(Q,M:String):String; overload;
begin
   SetCursor(True);
   CursorTo(1,24);
{$IFDEF CODERUNNER}
   Result:=Session.Ask(Q,M,False,180);
{$ELSE}
   Result:=System.Ask(Q,M,False,180);
{$ENDIF}
   ClearLine(24,7,0);
   SetCursor(False);
end;

function YesNo:String;
Begin
   SetCursor(True);
   CursorTo(1,24);
{$IFDEF CODERUNNER}
   Result:=Session.Ask('?','YN',False,40);
{$ELSE}
   Result:=Ask('?','YN',False,40000);
{$ENDIF}
   ClearLine(24,7,0);
   SetCursor(False);
end;

(*
FUNCTION GetInput(N:Char;Caps:Byte;L:INTEGER):String;
     { n - echochar '' = Echo Character Entered}
     { cap - 0 = as entered  1 = caps 2 = digits 0-9 only }
     { l=length }
VAR
   Answer,SoFar,Tmp:String;
   i,pp:Byte;
   A:Char;

BEGIN
   Result:='';
   Answer:='';
   SoFar:='';
   tmp:='';
   REPEAT
      REPEAT
         A:=GetChar;
         If not Connected then Exit;
      UNTIL FuncKey = FALSE ;
      If Cap=2 then Begin
         If not (A in ['0','1','2','3','4','5','6','7','8','9',#10,#13,#8,#16,#127]) then A:=#0;
      End;
      CASE A OF
         #0:BEGIN
            END;
         #1..#6,#9,#14,#15,#17..#28,#128..#255:Beep;
         #08,#16,#127:BEGIN
            IF LENGTH(SoFar)>=1 THEN BEGIN
               DELETE(tmp,LENGTH(tmp),1);
               BackSpace(#8#32#8);
            END;
         END;
         #13:Tmp:=SoFar+A;
         ELSE BEGIN
            IF Cap=1 THEN A:=UPCASE(a);
            IF LENGTH(SoFar)=L THEN Beep
            ELSE BEGIN
               Tmp:=SoFar+A;
                  IF n = '' THEN sDisplay(C_Back,C_Fore,C_Blink,a)
                  ELSE sDisplay(C_Back,C_Fore,C_Blink,n) ;
               END ;
            END ;
         END ; {case}
         SoFar := tmp ;
      UNTIL (a = #13) ;
      pp := POS(#13,SoFar) ;
      Answer := COPY(SoFar,1,pp-1) ;
      GetInput := Answer ;
   NewLine;
END;
*)

procedure DisplayFile(fn:String;doPause:Boolean);
var
   TFH:Text;
   Lc:Word;
   Ws:String;

Begin
   If FileExists(fn) then begin
      SetColor(14,0);
      AssignText(TFH, fn);
      Reset(TFH);
      Lc:=0;
      While not EndOfText(TFH) do begin
         Ws:=ReadLnText(TFH);
         Inc(Lc);
         FastWriteLn(Ws);
         If (Lc>22) and (doPause) then begin
            Pause(True);
            Lc:=0;
         End;
      End;
      CloseText(TFH);
      If (Lc>0) and doPause then Pause(True);
   End;
End;

Procedure DisplayFileEx(fn:String;doPause:Boolean=false);
Begin
{ By setting Session.setUTF8Mode, ***Modern Pascal*** translate cp437 characters to UTF8 for us! }
   If Chain.ReadBoolean('HASUTF8') or Chain.ReadBoolean('HIGH8') then begin
      If Chain.ReadBoolean('ANSITC') and FileExists(fn+'.cp437.tc') then DisplayFile(fn+'.cp437.tc',doPause)
      else If Chain.ReadBoolean('ANSI256') and FileExists(fn+'.cp437.256') then DisplayFile(fn+'.cp437.256',doPause)
      else If Chain.ReadBoolean('ANSI16') and FileExists(fn+'.cp437.16') then DisplayFile(fn+'.cp437.16',doPause)
      else If Chain.ReadBoolean('ANSI8') and FileExists(fn+'.cp437.8') then DisplayFile(fn+'.cp437.8',doPause)
      else If (Chain.ReadBoolean('HASUTF8') or Chain.ReadBoolean('HIGH8')) and FileExists(fn+'.cp437.ans') then DisplayFile(fn+'.cp437.ans',doPause)
      else DisplayFile(fn+'.cp437.asc',doPause);
   end
   else begin
      If Chain.ReadBoolean('ANSITC') and FileExists(fn+'.tc') then DisplayFile(fn+'.tc',doPause)
      else If Chain.ReadBoolean('ANSI256') and FileExists(fn+'.256') then DisplayFile(fn+'.256',doPause)
      else If Chain.ReadBoolean('ANSI16') and FileExists(fn+'.16') then DisplayFile(fn+'.16',doPause)
      else If Chain.ReadBoolean('ANSI8') and FileExists(fn+'.8') then DisplayFile(fn+'.8',doPause)
      else If Chain.ReadBoolean('HASANSI') and FileExists(fn+'.ans') then DisplayFile(fn+'.ans',doPause)
      else DisplayFile(fn+'.asc',doPause);
   end;
End;
