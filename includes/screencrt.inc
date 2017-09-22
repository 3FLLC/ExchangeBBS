{%Unit v7.0}

(* * * * * * * * * * * * * * * * * * * * * * *
 * Screen CRT                                *
 * By: G.E. Ozz Nixon Jr.                    *
 * Inspired by Technojocks Turbo Toolkit     *
 * Coded while jamming to Scorpions!   \m/   *
 * * * * * * * * * * * * * * * * * * * * * * *)

{$i screenobj.i}

var
   TMPTSO:TScreenObj;

Procedure InitFastTTT;
begin
//{$IFDEF CODERUNNER}Session.{$ENDIF}Write(#$FF#$FB#01#$FF#$FB#03#$FF#$FD#18#$FF#$FD#00#27+'[=3;7h'#27+'(U'+#27+'[8;25;80t'+#27'[0;40m'+#8#8#8#8#8#8#8#8);
{$IFDEF CODERUNNER}Session.{$ENDIF}Write(#$FF#$FB#01#$FF#$FB#03#$FF#$FD#18#$FF#$FD#00#27+'[?7h'#27+'(U'+#27+'[8;25;80t'+#27'[0;40m'+#27'[?25l'+#8#8#8#8#8#8#8#8#8#8);
   TMPTSO.Init();
end;

Function Attr(F,B:byte):byte;
begin
   Result:=(B Shl 4) or F;
end;

Procedure SetCursor(On:Boolean);
begin
   If On then TMPTSO.CursorOn
   else TMPTSO.CursorOn;
End;

Procedure SetBlink(On:Boolean);
begin
   If On then TMPTSO.BlinkOn
   else TMPTSO.BlinkOff;
End;

Procedure CLS; overload;
begin
   TMPTSO.ClrScr;
End;

Procedure CLS(Fg,Bg:Byte); overload;
begin
   TextAttr:=Attr(Fg,Bg);
{$IFDEF CODERUNNER}Session.AnsiColor(Fg,Bg);{$ENDIF}
   TMPTSO.ClrScr;
End;

Procedure ClearLine(Row,FG,Bg:Byte);
var
   SavedAttr:Byte;

begin
   SavedAttr:=TextAttr;
   TextAttr:=Attr(Fg,Bg);
{$IFDEF CODERUNNER}Session.AnsiColor(Fg,Bg);{$ENDIF}
   TMPTSO.GotoXy(1,Row);
   TMPTSO.ClrEol;
   TextAttr:=SavedAttr;
end;

Procedure SetColor(Fg,Bg:Byte);
Begin
   TextAttr:=Attr(Fg,Bg);
{$IFDEF CODERUNNER}Session.AnsiColor(Fg,Bg);{$ENDIF}
End;

Procedure Attrib(X1,Y1,X2,Y2,F,B:byte);
var
   I,I2,X,A : byte;
begin
   A := Attr(F,B);
   X := Succ(X2-X1);
   For I := Y1 to Y2 do
      For I2:=X1 to X do
         TMPTSO.SetAttr(I2,I,A);
end;

Procedure CursorTo(X,Y:Word);
begin
   TMPTSO.GotoXy(X,Y);
end;

Procedure FastWrite(Col,Row,Attr:byte;St:String); overload;
var
   SavedAttr:Byte;

begin
   SavedAttr:=TextAttr;
   TextAttr:=Attr;
{$IFDEF CODERUNNER}Session.AnsiColor(Attr and $F,Attr shr 4);{$ENDIF}
   TMPTSO.GotoXy(Col,Row);
   TMPTSO.Print(St);
   TextAttr:=SavedAttr;
end;

Procedure FastWrite(Col,Row,FG,BG:byte;St:String); overload;
var
   SavedAttr:Byte;

begin
   SavedAttr:=TextAttr;
   TextAttr:=Attr(FG,BG);
{$IFDEF CODERUNNER}Session.AnsiColor(Fg,Bg);{$ENDIF}
   TMPTSO.GotoXy(Col,Row);
   TMPTSO.Print(St);
   TextAttr:=SavedAttr;
end;

Procedure FastWrite(Col,Row:byte;St:String); overload;
begin
   TMPTSO.GotoXy(Col,Row);
   TMPTSO.Print(St);
end;

Procedure FastWrite(St:String); overload;
begin
   TMPTSO.Print(St);
end;

Procedure FastWriteLN(St:String);
begin
   TMPTSO.PrintLn(St);
end;

Procedure ClearText(x1,y1,x2,y2,F,B:integer); overload;
var
   Y:integer;
   attrib:byte;

begin
   If x2>TMPTSO.MaxWidth then x2:=TMPTSO.MaxWidth;
   Attrib:=attr(F,B);
   For Y:=y1 to y2 do
      Fastwrite(X1,Y,attrib,StringOfChar(#32,X2-X1+1));
end;

Procedure ClearText(x1,y1,x2,y2,F,B:integer;Ch:char); overload;
var
   Y,I:integer;
   attrib:byte;

begin
   If X2>TMPTSO.MaxWidth then X2:=TMPTSO.MaxWidth;
   Attrib:=Attr(F,B);
   For Y:=Y1 to Y2 do
{$IFDEF UNIX}
   For I:=X1 to X2 do
      FastWrite(I,Y,attrib,Ch);
{$ELSE}
      Fastwrite(X1,Y,attrib,StringOfChar(Ch,Succ(X2-X1)));
{$ENDIF}
end;

Procedure Clickwrite(Col,Row,F,B:byte;St:String);
var
   I:Integer;
   L,A:byte;

begin
   A:=attr(F,B);
   L:=length(St);
   For I:=L downto 1 do begin
      Fastwrite(Col,Row,A,copy(St,I,succ(L-I)));
      sound(500);delay(20);nosound;delay(30);
   end;
end;

Procedure Box(X1,Y1,X2,Y2,F,B,boxtype:integer);
var
   I:integer;
   corner1,corner2,corner3,corner4,horizline,vertline:char;
   attrib:byte;

begin
   case boxtype of
      0:begin
         corner1:=#32;
         corner2:=#32;
         corner3:=#32;
         corner4:=#32;
         horizline:=#32;
         vertline:=#32;
      end;
      1:begin // single line
         corner1:=#218;
         corner2:=#191;
         corner3:=#192;
         corner4:=#217;
         horizline:=#196;
         vertline:=#179;
      end;
      2:begin // double sides
         corner1:=#214;
         corner2:=#183;
         corner3:=#211;
         corner4:=#189;
         horizline:=#196;
         vertline:=#186;
      end;
      3:begin // double top/bottom
         corner1:=#213;
         corner2:=#184;
         corner3:=#212;
         corner4:=#190;
         horizline:=#205;
         vertline:=#179;
      end;
      4:begin // double all
         corner1:=#201;
         corner2:=#187;
         corner3:=#200;
         corner4:=#188;
         horizline:=#205;
         vertline:=#186;
      end;
      5:begin // 7bit
         corner1:='+';
         corner2:='+';
         corner3:='+';
         corner4:='+';
         horizline:='-';
         vertline:='|';
      end;
      else begin
         corner1:=chr(Boxtype);
         corner2:=chr(Boxtype);
         corner3:=chr(Boxtype);
         corner4:=chr(Boxtype);
         horizline:=chr(Boxtype);
         vertline:=chr(Boxtype);
      end;
   end;{case}
   attrib := attr(F,B);
   FastWrite(X1,Y1,attrib,corner1);
{$IFDEF UNIX}
   For I:=X1+1 to X2-1 do
      FastWrite(I,Y1,attrib,horizline);
{$ELSE}
   FastWrite(X1+1,Y1,attrib,StringOfChar(horizline,X2-X1-1));
{$ENDIF}
   FastWrite(X2,Y1,attrib,corner2);
   For I := Y1+1 to Y2-1 do begin
      FastWrite(X1,I,attrib,vertline);
      FastWrite(X2,I,attrib,vertline);
   end;
   FastWrite(X1,Y2,attrib,corner3);
{$IFDEF UNIX}
   For I:=X1+1 to X2-1 do
      FastWrite(I,Y2,attrib,horizline);
{$ELSE}
   FastWrite(X1+1,Y2,attrib,StringOfChar(horizline,X2-X1-1));
{$ENDIF}
   FastWrite(X2,Y2,attrib,corner4);
end;

Procedure FillBox(X1,Y1,X2,Y2,F,B,boxtype:integer);
begin
   Box(X1,Y1,X2,Y2,F,B,boxtype);
   ClearText(succ(X1),succ(Y1),pred(X2),pred(Y2),F,B);
end;

Procedure GrowBox(X1,Y1,X2,Y2,F,B,boxtype:integer);
var
   TX1,TY1,TX2,TY2,Ratio : integer;

begin
   If 2*(Y2-Y1+1)>X2-X1+1 then Ratio:=2
   else Ratio:=1;
   TX2:=(X2-X1) div 2+X1+2;
   TX1:=TX2-3;                 {needs a box 3 by 3 minimum}
   TY2:=(Y2-Y1) div 2+Y1+2;
   TY1:=TY2-3;
   If (X2-X1)<3 then begin
      TX2:=X2;
      TX1:=X1;
   end;
   If (Y2-Y1)<3 then begin
      TY2:=Y2;
      TY1:=Y1;
   end;
   repeat
      FillBox(TX1,TY1,TX2,TY2,F,B,BoxType);
      If TX1>=X1+(1*Ratio) then TX1:=TX1-(1*Ratio)
      else TX1:=X1;
      If TY1>Y1 then TY1:=TY1-1;
      If TX2+(1*Ratio)<=X2 then TX2:=TX2+(1*Ratio)
      else TX2:=X2;
      If TY2+1<=Y2 then TY2:=TY2+1;
      Yield(10);
   Until (TX1=X1) and (TY1=Y1) and (TX2=X2) and (TY2=Y2);
   CursorTo(TX1,TY1);
   FillBox(TX1,TY1,TX2,TY2,F,B,BoxType);
end;

Procedure HorizLine(X1,X2,Y,F,B,lineType:byte);
var
   Horizline : char;
   attrib : byte;

begin
   case LineType of                     {5.00a}
      0:HorizLine := ' ';
      //2,4,7,9:Horizline := 'Í';
      //1,3,6,8:HorizLine := 'Ä';
      else HorizLine := Chr(LineType);
   end; {case}
   Attrib:=attr(F,B);
   If X2>X1 then FastWrite(X1,Y,attrib,StringOfChar(HorizLine,X2-X1+1))
   else FastWrite(X1,Y,attrib,StringOfChar(HorizLine,X1-X2+1));
end;   {horizline}

Procedure VertLine(X,Y1,Y2,F,B,lineType:byte);
var
   I : integer;
   vertline : char;
   attrib : byte;

begin
   case LineType of                {5.00a}
      0:VertLine := ' ';
//    2,3,7,9:Vertline := 'º';      {5.02a}
//    1,4,6,8:VertLine := '³';      {5.02a}
      else VertLine := Chr(LineType);
   end; {case}
   Attrib := attr(F,B);
   If Y2 > Y1 then
      For I := Y1 to Y2 do Fastwrite(X,I,Attrib,Vertline)
      else For I := Y2 to Y1 do Fastwrite(X,I,Attrib,Vertline);
end;   {vertline}

Procedure WriteBetween(X1,X2,Y,F,B:byte; St:String);
var
   X:integer;

begin
   If length(St) >= X2 - X1 + 1 then FastWrite(X1,Y,F,B,St)
   else begin
      X:= X1 + (X2 - X1 + 1 - length(St)) div 2 ;
      FastWrite(X,Y,F,B,St);
   end;
end;

Procedure WriteCenter(LineNO,F,B:integer;St:String);
begin
   Fastwrite(40 - length(St) div 2,Lineno,attr(F,B),St);
end;

Procedure WriteVert(X,Y,F,B:integer; St:String);
var
   I:Byte;

begin
   If length(St) > 26 - Y then delete(St,27 - Y,Length(St));
   For I := 1 to length(St) do Fastwrite(X,Y-1+I,attr(F,B),St[I]);
end;

Procedure ViewPort(X,Y,W,H:Word);
Begin
   TMPTSO.Window(X,Y,W,H);
End;

Function  EGAVGASystem: boolean;
Begin
   Result:=True;
end;

Procedure DisposeFastTTT;
begin
{$IFNDEF CODERUNNER}
   SetColor(7,0);
   Write(' '#8);
{$ENDIF}
   TMPTSO.Free;
end;
