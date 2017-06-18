
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
