var
   CurrentFileArea:String;

procedure FileAreaList(FileName:String);
var
   Buffer,Ws,Ts,FName,ExtStr,DateTimeStr,SizeStr,DescStr:String;
   Loop:Byte;

Begin
   LoadFromFile(FileName,Buffer);
   Ws:=Buffer;
   CLS(7,0);
   ClearLine(1,0,3);
   FastWrite(1,1,'|3F##|3E'+SVER+'|39Filename.Ext|3E'+SVER+'|39Size/Byte|3E'+SVER+'|39Date Rcvd |3E'+SVER+'|39Description');
   For Loop:=2 to 22 do begin
      If Ws<>'' then begin
         Ts:=StrReadLn(Ws); {strings unit}
//         FastWrite(1,Loop,Ts);
         FName:=Fetch(Ts,',');
         SizeStr:=Fetch(Ts,',');
         DateTimeStr:=Fetch(Ts,',');
         If StrToIntDef(Copy(DateTimeStr,1,2),0)<50 then begin
            DateTimeStr:=Copy(DateTimeStr,3,2)+'-'+Copy(DateTimeStr,5,2)+'-20'+Copy(DateTimeStr,1,2);
         End
         Else Begin
            DateTimeStr:=Copy(DateTimeStr,3,2)+'-'+Copy(DateTimeStr,5,2)+'-19'+Copy(DateTimeStr,1,2);
         End;
         DescStr:=Ts;
         ExtStr:=Uppercase(ExtractFileExtension(FName));
         FName:=PadRight(FName,12);
         If ExtStr='.EXE' then FName:='|02'+FName
         else If ExtStr='.COM' then FName:='|02'+FName
         else If ExtStr='.GIF' then FName:='|0D'+FName
         else If ExtStr='.PNG' then FName:='|0D'+FName
         else If ExtStr='.JPG' then FName:='|0D'+FName
         else If ExtStr='.ICO' then FName:='|0D'+FName
         else If ExtStr='.TXT' then FName:='|03'+FName
         else If ExtStr='.PAS' then FName:='|05'+FName
         else If ExtStr='.C'   then FName:='|04'+FName
         else If ExtStr='.CPP' then FName:='|04'+FName
         else If ExtStr='.H'   then FName:='|0C'+FName
         else If ExtStr='.LBR' then FName:='|0C'+FName
         else If ExtStr='.GZ'  then FName:='|0A'+FName
         else If ExtStr='.SQ'  then FName:='|0A'+FName
         else If ExtStr='.7Z'  then FName:='|0A'+FName
         else If ExtStr='.ZIP' then FName:='|0A'+FName
         else If ExtStr='.ARJ' then FName:='|0A'+FName
         else If ExtStr='.ARC' then FName:='|0A'+FName
         else If ExtStr='.RAR' then FName:='|0A'+FName
         else If ExtStr='.BZIP' then FName:='|0A'+FName
         else If ExtStr='.ICE' then FName:='|0B'+FName
         else If ExtStr='.JAR' then FName:='|0B'+FName
         else If ExtStr='.WAR' then FName:='|0B'+FName
         else If ExtStr='.LZH' then FName:='|0B'+FName
         else If ExtStr='.TAR' then FName:='|0B'+FName
         else If ExtStr='.XAR' then FName:='|0B'+FName
         else If ExtStr='.EAR' then FName:='|0B'+FName
         else If ExtStr='.TGZ' then FName:='|0B'+FName
	 else If ExtStr='.CAB' then FName:='|0B'+FName
	 else If ExtStr='.ZOO' then FName:='|0B'+FName
         else If ExtStr='.MP3' then FName:='|0E'+FName
         else If ExtStr='.RPM' then FName:='|0E'+FName
         else If ExtStr='.MSI' then FName:='|0E'+FName
         else If ExtStr='.DMG' then FName:='|07'+FName
         else If ExtStr='.ISO' then FName:='|07'+FName
         else FName:='|0E'+FName;
         FastWrite(1,Loop,'|0F'+PadLeft(IntToStr(Loop-1),2,'0')+'|0E'+SVER+FName+'|0E'+SVER+'|07'+
            PadLeft(IntToCommaStr(StrToIntDef(SizeStr,0)),9)+'|0E'+SVER+'|07'+DateTimeStr+'|0E'+SVER+'|07'+Copy(DescStr,1,43));
      End
      Else
         FastWrite(1,Loop,'|0F  |0E'+SVER+Space(12)+SVER+Space(9)+SVER+Space(10)+SVER);
   End;
   Pause;
End;

procedure FileAreaSelect;
var
   fc:TFindClass;
   ctr,Line:Byte;
   folders:Array[0..37] of String;
   filepath:String;

label
   nextfile;

function FormatFileName(S:String):String;
Begin
   Result:=StringReplace(S,'_',#32,[rfReplaceAll]);
End;

begin
   Filepath:='';
nextfile:
   If FileExists(ScriptRoot+'files'+PathSeparator+FilePath+'FILES.CSV') then begin
      FileAreaList(ScriptRoot+'files'+PathSeparator+FilePath+'FILES.CSV');

   End;
   Title;
   if (FilePath<>'') then FastWrite(2,4,'|1B'+FormatFileName(FilePath));
   fc.Init(faAnyFile);
   fc.Find(ScriptRoot+'files'+PathSeparator+FilePath,{$IFDEF UNIX}'*'{$ELSE}'*.*'{$ENDIF});
   Ctr:=0;
   Line:=4;
   while fc.getFound do begin
      If fc.getIsDirectory then begin
         If (fc.getFilename=".") or (fc.getFilename="..") then {absorb}
         else begin
            Inc(Ctr);
            if ctr mod 2=1 then begin
               Inc(Line);
               FastWrite(1,Line,'|0F'+PadLeft(IntToStr(Ctr),2,'0'));
               FastWrite(4,Line,'|0A'+FormatFileName(fc.getFilename));
            End
            else Begin
               FastWrite(41,Line,'|0F'+PadLeft(IntToStr(Ctr),2,'0'));
               FastWrite(44,Line,'|0A'+FormatFileName(fc.getFilename));
            End;
            folders[Ctr-1]:=fc.getFilename;
         end
      End;
      If (Line>22) and (Ctr mod 2=0) then begin
         Ws:=Ask2('|0BWhich folder would you like to open? |0D(|0FQ=Quit|0B,|0FN=Next|0D)|0E','  ');
         If Uppercase(Ws)='Q' then break;
         If Uppercase(Ws)<>'N' then begin
            FilePath:=FilePath+Folders[StrToIntDef(Ws,1)-1]+PathSeparator;
            goto nextfile;
         End;
         For Line:=5 to 24 do ClearLine(Line,7,0);
         For Line:=0 to 37 do folders[Line]:='';
         Line:=4;
         Ctr:=0;
      End;
      fc.Next;
   end;
   If Line>4 then begin
      Ws:=Ask2('|0BWhich folder would you like to open? |0D(|0FQ=Quit|0D)|0E','  ');
      If (Ws<>'Q') or (Ws<>'q') then begin
         FilePath:=FilePath+Folders[StrToIntDef(Ws,1)-1]+PathSeparator;
         goto nextfile;
      End;
   End;
   fc.Free;
end;
