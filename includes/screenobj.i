(* * * * * * * * * * * * * * * * * * * * * * *
 * Low-Level Screen Object                   *
 * By: G.E. Ozz Nixon Jr.                    *
 * Inspired by Technojocks Turbo Toolkit     *
 * Coded while jamming to Scorpions!   \m/   *
 * * * * * * * * * * * * * * * * * * * * * * *)

type
   TScreenBlock = packed record
      Attribute:Byte;
      Character:Char;
   End;
   TVideoMem = Array of Array of TScreenBlock;
   TRegion = packed record
      Width:Word;
      Height:Word;
      Top:Word;
      Left:Word;
      Region:TVideoMem;
   End;

   PScreenObj = ^TScreenObj;
   TScreenObj = Class
      MaxHeight:Word;
      MaxWidth:Word;
      VideoMem:TVideoMem;
      SavedVideoMem:Array of TVideoMem;
      CursorOn:private procedure of object;
      CursorOff:private procedure of object;
      BlinkOn:private procedure of object;
      BlinkOff:private procedure of object;
      ClrScr:private procedure of object;
      ClrEol:private procedure of object;
      GotoXY:private procedure(X,Y:Word) of object;
      DelLine:private procedure of object;
      InsLine:private procedure of object;
      Window:private procedure(X,Y,W,H:Word) of object;
      RestoreWindow:private procedure of object;
      PrintCh:private procedure(Ch:Char) of object; // Internal Usage Only
      Print:private procedure(S:String) of object;
      PrintLn:private procedure(S:String='') of object;
      GetChar:private function(X,Y:Word):Char of object;
      GetAttr:private function(X,Y:Word):Byte of object;
      SetAttr:private procedure(X,Y:Word;Attr:Byte) of object;
      RegionSave:private procedure(X,Y,W,H:Word;var Dest:TRegion) of object;
      RegionLoad:private procedure(var Source:TRegion) of object;
      WhereIsXY:private procedure(var X,Y:Word) of object;
   end;

Procedure TScreenObj.CursorOn;
begin
{$IFDEF CODERUNNER}
   Session.Write(#27+'[?25h');
{$ELSE}
   System.CursorOn;
{$ENDIF}
End;

Procedure TScreenObj.CursorOff;
begin
{$IFDEF CODERUNNER}
   Session.Write(#27+'[?25l');
{$ELSE}
   System.CursorOff;
{$ENDIF}
End;

Procedure TScreenObj.BlinkOn;
begin
{$IFDEF CODERUNNER}
   Session.Write(#27+'[5m');
{$ENDIF}
End;

Procedure TScreenObj.BlinkOff;
begin
{$IFDEF CODERUNNER}
   Session.Write(#27+'[25m');
{$ENDIF}
End;

Procedure TScreenObj.WhereIsXY(var X,Y:Word);
Begin
   X:=WhereX;
   Y:=WhereY;
End;

Procedure TScreenObj.RegionLoad(Var Source:TRegion);
var
   Loop,Loop2:LongInt;
   SavedAttr:Byte;

Begin
   SavedAttr:=TextAttr;
   For Loop:=Source.Height-1 downto 0 do begin
      System.GotoXy(Source.Left,Loop+Source.Top);
      For Loop2:=Source.Left to Source.Width do begin
         TextAttr:=Source.Region[Loop][Loop2-Source.Left].Attribute;
         PrintCh(Source.Region[Loop][Loop2-Source.Left].Character);
      End;
      SetLength(Source.Region[Loop],0);
      SetLength(Source.Region,Length(Source.Region)-1);
   End;
   Source.Width:=0;
   Source.Height:=0;
   TextAttr:=SavedAttr;
End;

Procedure TScreenObj.RegionSave(X,Y,W,H:Word;Var Dest:TRegion);
var
   Loop,Loop2:Word;

begin
   Dest.Top:=Y;
   Dest.Left:=X;
   Dest.Width:=Succ(W-X);
   Dest.Height:=Succ(H-Y);
   SetLength(Dest.Region,Dest.Height);
   For Loop:=0 to Dest.Height-1 do begin
      SetLength(Dest.Region[Loop],Dest.Width);
      For Loop2:=Dest.Left to Dest.Width do begin
         Dest.Region[Loop][Loop2-Dest.Left].Attribute:=VideoMem[Pred(Loop+Y)][Pred(Loop2)].Attribute;
         Dest.Region[Loop][Loop2-Dest.Left].Character:=VideoMem[Pred(Loop+Y)][Pred(Loop2)].Character;
      End;
   End;
end;

procedure TScreenObj.SetAttr(X,Y:Word;Attr:Byte);
var
   SavedAttr:Byte;

begin
   SavedAttr:=TextAttr;
   VideoMem[Y-1][X-1].Attribute:=Attr;
   GotoXy(X,Y);
   TextAttr:=Attr;
{$IFDEF CODERUNNER}Session.{$ENDIF}PipeWrite(VideoMem[Y-1][X-1].Character);
   TextAttr:=SavedAttr;
end;

function TScreenObj.GetAttr(X,Y:Word):Byte;
begin
   Result:=VideoMem[Y-1][X-1].Attribute;
end;

function TScreenObj.GetChar(X,Y:Word):Char;
begin
   Result:=VideoMem[Y-1][X-1].Character;
end;

procedure TScreenObj.PrintCh(Ch:Char); // DO NOT CALL DIRECT!
Begin
   case Ch of
      #8:{absorb};  // inherit the X/Y
      #9:{absorb};  // inherit the X/Y
      #10:{absorb}; // inherit the X/Y
      #12:{absorb}; // inherit the X/Y
      #13:{absorb}; // inherit the X/Y
      else begin
         VideoMem[WhereY-1][WhereX-1].Attribute:=TextAttr;
         VideoMem[WhereY-1][WhereX-1].Character:=Ch;
      end;
   end;
End;

procedure TScreenObj.Print(S:String);
Begin
{$IFDEF CODERUNNER}
   Session.PipeWrite(S);
{$ELSE}
   PipeWriteUTF8(S);
   For var Loop:=1 to Length(S) do PrintCh(S[Loop]); // store in video buffer //
{$ENDIF}
end;

procedure TScreenObj.PrintLn(S:String);
Begin
   Self.Print(S);
{$IFDEF CODERUNNER}Session.{$ENDIF}Writeln('');
end;

procedure TScreenObj.RestoreWindow;
var
   Loop,Loop2:Longint;
   SavedAttr:Byte;

begin
   System.Window(1,1,MaxWidth,MaxHeight);
   If Length(SavedVideoMem)=0 then exit;
   SavedAttr:=TextAttr;
   For Loop:=MaxHeight-1 downto 0 do begin
{$IFDEF CODERUNNER}Session.GotoXy{$ELSE}System.GotoXy{$ENDIF}(1,Loop+1);
      For Loop2:=0 to MaxWidth-1 do begin
         TextAttr:=SavedVideoMem[High(SavedVideoMem)][Loop][Loop2].Attribute;
{$IFDEF CODERUNNER}Session.{$ENDIF}PipeWrite(SavedVideoMem[High(SavedVideoMem)][Loop][Loop2].Character);
         VideoMem[Loop][Loop2].Attribute:=TextAttr;
         VideoMem[Loop][Loop2].Character:=SavedVideoMem[High(SavedVideoMem)][Loop][Loop2].Character;
      End;
      If Loop=MaxHeight-1 then begin
// compensate for scroll:
         System.GotoXy(1,Loop);
         System.InsLine;
      End;
   End;
// Deallocate Colums, Lines then Parent:
   SetLength(SavedVideoMem,Length(SavedVideoMem)-1);
   TextAttr:=SavedAttr;
end;

procedure TScreenObj.Window(X,Y,W,H:Word);
var
   Loop,Loop2:Longint;

begin
   System.Window(X,Y,W,H);
// extend saved:
   SetLength(SavedVideoMem,Length(SavedVideoMem)+1);
// allocate lines in saved:
   SetLength(SavedVideoMem[High(SavedVideoMem)], MaxHeight);
// clone active memory:
   For Loop:=0 to MaxHeight-1 do begin
// allocate columns in saved:
      SetLength(SavedVideoMem[High(SavedVideoMem)][Loop],MaxWidth);
      For Loop2:=0 to MaxWidth-1 do begin
         SavedVideoMem[High(SavedVideoMem)][Loop][Loop2].Attribute:=VideoMem[Loop][Loop2].Attribute;
         SavedVideoMem[High(SavedVideoMem)][Loop][Loop2].Character:=VideoMem[Loop][Loop2].Character;
      End;
   End;
end;

procedure TScreenObj.InsLine;
var
   Loop,Loop2:Longint;

begin
{$IFDEF CODERUNNER}Session.{$ELSE}System.{$ENDIF}InsLine;
   For Loop:=MaxHeight-1 downto WhereY-2 do
      For Loop2:=0 to MaxWidth-1 do begin
         VideoMem[Loop][Loop2].Attribute:=VideoMem[Loop-1][Loop2].Attribute;
         VideoMem[Loop][Loop2].Character:=VideoMem[Loop-1][Loop2].Character;
      End;
   For Loop:=0 to MaxWidth-1 do begin
      VideoMem[WhereY-1][Loop].Attribute:=System.TextAttr;
      VideoMem[WhereY-1][Loop].Character:=#32;
   End;
end;

procedure TScreenObj.DelLine;
var
   Loop,Loop2:Longint;

Begin
{$IFDEF CODERUNNER}Session.{$ELSE}System.{$ENDIF}DelLine;
   If WhereY<MaxHeight then
      For Loop:=WhereY-1 to MaxHeight-2 do
         For Loop2:=0 to MaxWidth-1 do begin
            VideoMem[Loop][Loop2].Attribute:=VideoMem[Loop+1][Loop2].Attribute;
            VideoMem[Loop][Loop2].Character:=VideoMem[Loop+1][Loop2].Character;
         End;
   For Loop:=0 to MaxWidth-1 do begin
      VideoMem[WhereY-1][Loop].Attribute:=System.TextAttr;
      VideoMem[WhereY-1][Loop].Character:=#32;
   End;
end;

procedure TScreenObj.GotoXy(X,Y:Word);
begin
{$IFDEF CODERUNNER}Session.{$ELSE}System.{$ENDIF}GotoXy(X,Y);
end;

procedure TScreenObj.ClrEol;
var
   Loop:Longint;

Begin
{$IFDEF CODERUNNER}Session.{$ELSE}System.{$ENDIF}ClrEOL;
   For Loop:=WhereX to MaxWidth-1 do begin
      VideoMem[WhereY-1][Loop].Attribute:=System.TextAttr;
      VideoMem[WhereY-1][Loop].Character:=#32;
   End;
end;

procedure TScreenObj.ClrScr;
var
   Loop,Loop2:Longint;

begin
{$IFDEF CODERUNNER}Session.{$ELSE}System.{$ENDIF}ClrScr;
// Adds 0.02s to this call:
   For Loop:=0 to MaxHeight-1 do
      For Loop2:=0 to MaxWidth-1 do begin
         VideoMem[Loop][Loop2].Attribute:=7;
         VideoMem[Loop][Loop2].Character:=#32;
      End;
end;

procedure TScreenObj.Free;
var
   Loop:Longint;

begin
   For Loop:=0 to MaxHeight-1 do
      SetLength(VideoMem[Loop],0);
   SetLength(VideoMem,0);
end;

procedure TScreenObj.Init;
var
   Loop:Longint;

Begin
{$IFDEF CODERUNNER}Session.SetANSIMode;{$ENDIF}
   with Self do begin
      TMethod(@WhereIsXY) := [@TScreenObj.WhereIsXY, @Self];
      TMethod(@RegionLoad) := [@TScreenObj.RegionLoad, @Self];
      TMethod(@RegionSave) := [@TScreenObj.RegionSave, @Self];
      TMethod(@SetAttr) := [@TScreenObj.SetAttr, @Self];
      TMethod(@GetAttr) := [@TScreenObj.GetAttr, @Self];
      TMethod(@GetChar) := [@TScreenObj.GetChar, @Self];
      TMethod(@PrintLn) := [@TScreenObj.PrintLn, @Self];
      TMethod(@Print) := [@TScreenObj.Print, @Self];
      TMethod(@PrintCh) := [@TScreenObj.PrintCh, @Self];
      TMethod(@RestoreWindow) := [@TScreenObj.RestoreWindow, @Self];
      TMethod(@Window) := [@TScreenObj.Window, @Self];
      TMethod(@InsLine) := [@TScreenObj.InsLine, @Self];
      TMethod(@DelLine) := [@TScreenObj.DelLine, @Self];
      TMethod(@GotoXY) := [@TScreenObj.GotoXY, @Self];
      TMethod(@ClrEol) := [@TScreenObj.ClrEol, @Self];
      TMethod(@ClrScr) := [@TScreenObj.ClrScr, @Self];
      TMethod(@CursorOn) := [@TScreenObj.CursorOn, @Self];
      TMethod(@CursorOff) := [@TScreenObj.CursorOff, @Self];
      TMethod(@BlinkOn) := [@TScreenObj.BlinkOn, @Self];
      TMethod(@BlinkOff) := [@TScreenObj.BlinkOff, @Self];
      TMethod(@Free) := [@TScreenObj.Free, @Self];
      MaxHeight:=ScreenHeight;
      MaxWidth:=ScreenWidth;
      SetLength(VideoMem, MaxHeight);
      For Loop:=0 to MaxHeight-1 do
         SetLength(VideoMem[Loop],MaxWidth);
   end;
End;

{$IFDEF STANDALONE}
var
   Screen:TScreenObj;
{$ENDIF}
