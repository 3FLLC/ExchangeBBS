
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

Function OneKey(S:String;B:Boolean):Char;
var
{$IFDEF CODERUNNER}
   Arr:TCharArray;
   Loop:Longint;
{$ELSE}
   Ch:Char;
{$ENDIF}

Begin
{$IFDEF CODERUNNER}
   SetLength(Arr,Length(S));
   For Loop:=1 to Length(S) do Arr[Loop-1]:=S[Loop];
   Result:=Uppercase(Session.ReadChar(Arr,120))[1];
{$ELSE}
   While True do Begin
      Ch:=UpCase(ReadKey);
      If Pos(Ch,S)>0 then begin
         Result:=Ch;
         Exit;
      End;
   End;
{$ENDIF}
End;

function Ask(Q,M:String):String; overload;
begin
{$IFDEF CODERUNNER}
   Result:=Session.Ask(Q,M,False,180);
{$ELSE}
   Result:=System.Ask(Q,M,False,False,180);
{$ENDIF}
end;

function Ask2(Q,M:String):String; overload;
begin
   CursorTo(1,24);
{$IFDEF CODERUNNER}
   Result:=Session.Ask(Q,M,False,180);
{$ELSE}
   Result:=System.Ask(Q,M,False,False,180);
{$ENDIF}
   ClearLine(24,7,0);
end;

procedure Pause(New:Boolean=False);
Begin
   If not new then CursorTo(1,24);
{$IFDEF CODERUNNER}
   Session.Prompt('|0D=|0APaused|0D= |0BPress any key to continue',40);
{$ELSE}
   FastWrite('|0D=|0APaused|0D= |0BPress any key to continue');
   ReadKey;
{$ENDIF}
   if not new then ClearLine(24,7,0);
end;

function YesNo:String;
Begin
   CursorTo(1,24);
{$IFDEF CODERUNNER}
   Result:=Session.Ask('','YN',False,40);
{$ELSE}
   Result:=Ask('','YN',False,False,40);
{$ENDIF}
   ClearLine(24,7,0);
end;

procedure DisplayFile(fn:String);
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
         If (Lc>22) then begin
            Pause(True);
            Lc:=0;
         End;
      End;
      CloseText(TFH);
      If Lc>0 then Pause(True);
   End;
End;

Procedure DisplayFileEx(fn:String);
Begin
   If Chain.ReadBoolean('UTF8') then begin
      If Chain.ReadBoolean('ANSITC') and FileExists(fn+'.utf8.tc') then DisplayFile(fn+'.utf8.tc')
      else If Chain.ReadBoolean('ANSI256') and FileExists(fn+'.utf8.256') then DisplayFile(fn+'.utf8.256')
      else If Chain.ReadBoolean('ANSI16') and FileExists(fn+'.utf8.16') then DisplayFile(fn+'.utf8.16')
      else If Chain.ReadBoolean('ANSI8') and FileExists(fn+'.utf8.8') then DisplayFile(fn+'.utf8.8')
      else If Chain.ReadBoolean('HASANSI') and FileExists(fn+'.utf8.ans') then DisplayFile(fn+'.utf8.ans')
      else DisplayFile(fn+'.utf8.asc');
   end
   else if Chain.ReadBoolean('HIGH8') then begin
      If Chain.ReadBoolean('ANSITC') and FileExists(fn+'.cp437.tc') then DisplayFile(fn+'.cp437.tc')
      else If Chain.ReadBoolean('ANSI256') and FileExists(fn+'.cp437.256') then DisplayFile(fn+'.cp437.256')
      else If Chain.ReadBoolean('ANSI16') and FileExists(fn+'.cp437.16') then DisplayFile(fn+'.cp437.16')
      else If Chain.ReadBoolean('ANSI8') and FileExists(fn+'.cp437.8') then DisplayFile(fn+'.cp437.8')
      else If Chain.ReadBoolean('HASANSI') and FileExists(fn+'.cp437.ans') then DisplayFile(fn+'.cp437.ans')
      else DisplayFile(fn+'.cp437.asc');
   end
   else begin
      If Chain.ReadBoolean('ANSITC') and FileExists(fn+'.tc') then DisplayFile(fn+'.tc')
      else If Chain.ReadBoolean('ANSI256') and FileExists(fn+'.256') then DisplayFile(fn+'.256')
      else If Chain.ReadBoolean('ANSI16') and FileExists(fn+'.16') then DisplayFile(fn+'.16')
      else If Chain.ReadBoolean('ANSI8') and FileExists(fn+'.8') then DisplayFile(fn+'.8')
      else If Chain.ReadBoolean('HASANSI') and FileExists(fn+'.ans') then DisplayFile(fn+'.ans')
      else DisplayFile(fn+'.asc');
   end;
End;
