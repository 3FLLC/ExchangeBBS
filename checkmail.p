Program CheckMail.v1170607;

/////////////////////////////////////////////////////////////////////////////
// MODULE ID: CHECKMAIL
// Description: Scan INBOX and ECHO folders for messages addressed to user.
/////////////////////////////////////////////////////////////////////////////
uses
   Variants,
   Classes,
   Databases,
   Display,     // Color Names
   Strings,     //
   ANSISockets, // Extends Session to Support ANSI/AVATAR/PIPE/WWIV commands!
   Environment, // Add FILE routines, etc.
   Chains;      // Add Chain Memory and Chaining...

var
   ScriptRoot:String;
   Username:String;
   AreaList,IndexFiles:TStringList;
   Loop,AreaNo,Offset:Longint;
   Ws,Ss:String;
   Match:Boolean;
   DBF:THalcyonDataset;
   Field,Field2:TField;
   tries:longint;

{$I includes/screencrt.inc}
{$I includes/bbskit.inc}

Begin
   ScriptRoot:=ExtractFilePath(ExecFilename);
   InitFastTTT;
   If not Chain.ReadBoolean('HASANSI') then Session.SetASCIIMode;
   If Chain.ReadBoolean('HASANSI') then Session.SetANSIMode;
   If Chain.ReadBoolean('HASUTF8') then Session.SetUTF8Mode;
   CLS;
{$IFDEF CODERUNNER}
   If Session.CountWaiting then Session.ReadStr(Session.CountWaiting,500);
{$ENDIF}
   Chain.Retrieve('USERNAME',Username);
   AreaList.Init;
   if fileexists(ScriptRoot+'data/ECHOAREA.LST') then Begin
      AreaList.LoadFromFile(ScriptRoot+'data/ECHOAREA.LST');
      FastWriteln('|0AChecking |0B'+IntToCommaStr(AreaList.getCount)+'|0A Fidonet Echos for messages addressed to you.');
      Offset:=1;
      For Loop:=0 to AreaList.getCount-1 do begin
         Ws:=AreaList.getStrings(Loop);
         FastWrite('|0A'+PadRight(Fetch(Ws,'='),22)+'|0B');
         AreaNo:=StrToIntDef(Fetch(Ws,','),0);
         If AreaNo>0 then begin
            Ss:=ScriptRoot+'mbase/msg'+IntToHex(AreaNo,5);
            Match:=False;
            If FileExists(Ss+'.dbf') then begin
               DBF.Init(Nil);
               DBF.setFilename(Ss+'.dbf');
               IndexFiles:=DBF.getIndexFiles;
               IndexFiles.Add(Ss+'.cdx');
               DBF.Open;
               DBF.setIndexTag('PK');
               DBF.First;
               FastWrite(PadLeft(IntToCommaStr(DBF.getRecordCount),7)+" |08");
               Field:=DBF.getFieldByName('MSGTO');
               Field2:=DBF.getFieldByName('MSGNUMBER');
               If DBF.Locate('MSGTO',Username,[]) then begin
                  If Field.getAsString=UserName then Repeat
                     Match:=True;
                     If Tries<>Field2.getAsInteger then begin
                        FastWrite('|0D*|0F'+Field2.getAsString+'|0A,');
                        Tries:=Field2.getAsInteger;
                     end
                     else break;
                     DBF.Next;
                     if Field.getAsString=UserName then
                        If Tries<>Field2.getAsInteger then begin
                           FastWrite('|0D*|0F'+Field2.getAsString+'|0A,');
                           Tries:=Field2.getAsInteger;
                        end
                        else break;
                  Until (not DBF.LocateNext('MSGTO',Username,[]));
               End;
               DBF.Close;
               DBF.Free;
               If not Match then FastWriteln('no mail')
               Else FastWriteln('|09done');
            end
            Else FastWriteln('      0 |0CEmpty');
            Inc(Offset);
         End;
         Yield(1);
         If Offset>21 then begin
            if PauseBar='S' then begin
               FastWriteln(#13#10+'|0CCancelled.'+#13#10);
               Break;
            End;
            CLS;
            FastWriteln('|0AChecking |0B'+IntToCommaStr(AreaList.getCount)+'|0A Fidonet Echos for new messages.');
            Offset:=1;
         End;
      End;
   End;
   AreaList.Free;
   Pause;
   Chain.Run(ScriptRoot+'mainmenu.p');
End.
