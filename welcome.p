Program welcome;

/////////////////////////////////////////////////////////////////////////////
// MODULE ID: WELCOME
// Description: Display Welcome and Ask for Login or Sign-up
/////////////////////////////////////////////////////////////////////////////

uses
   Classes,
   Math,
   Datetime,
   Databases,
   hashes,
   Display,     // Color Names
   Strings,     //
   ANSISockets, // Extends Session to Support ANSI/AVATAR/PIPE/WWIV commands!
   Environment, // Add FILE routines, etc.
   Chains;      // Add Chain Memory and Chaining...

{$DEFINE PLUGINS}
{$DEFINE EXCHANGEBBS}

Var
   ScriptRoot:String;
   Ws:String; // used in detect ANSI and COLOR plugins
   HasAnsi:Boolean;
   NeedsEcho:Boolean;
   i18n:TStringList;

{$I includes/screencrt.inc}
{$I includes/bbskit.inc}
{$I plugins/title.inc}
{$I plugins/newsignup.inc}
{$I plugins/loginprocess.inc}

{###################  doCreateTotals  #######################################
#   Purpose....: Create Missing Statistics Summary Database                 #
#   In params..: *                                                          #
#   Out params.: *                                                          #
#   Returns....: *                                                          #
#   Created....: 1170909 by Ozz                                             #
############################################################################}
Procedure doCreateTotals;
Var
   Schema:TStringList;
   DBF:THalcyonDataset;
   Field:TField;

Begin
   FastWriteln('|09Totals Initialized...|0A');
   DBF.Init(Nil);
   Schema.Init;
   Schema.Add('CALLS,N,20,0');       // accepted connects
   Schema.Add('UPLOADS,N,20,0');     // total files.csv lines
   Schema.Add('DNLOADS,N,20,0');     // accumulated downloads
   Schema.Add('DNAREAS,N,20,0');     // total files.csv files
   DBF.CreateDBF(ScriptRoot+'data/totals.dbf','',FoxPro,Schema);
   Schema.Free;
   DBF.setFilename(ScriptRoot+'data/totals.dbf');
   DBF.Open;
   DBF.Append;
   Field:=DBF.getFieldByName('CALLS');
   Field.setAsInteger(198769);
   Field:=DBF.getFieldByName('UPLOADS');
   Field.setAsInteger(1633);
   Field:=DBF.getFieldByName('DNLOADS');
   Field.setAsInteger(98769);
   Field:=DBF.getFieldByName('DNAREAS');
   Field.setAsInteger(108);
   DBF.Post;
   DBF.Free;
End;

{###################  IncrementCallers  #####################################
#   Purpose....: Increment the number of system connections ASAP            #
#   In params..: *                                                          #
#   Out params.: *                                                          #
#   Returns....: *                                                          #
#   Created....: 1170909 by Ozz                                             #
############################################################################}
Procedure IncrementCallers;
var
   DBF:THalcyonDataset;
   Field:TField;

Begin
   DBF.Init(Nil);
   DBF.SetFileName(ScriptRoot+'data/totals.dbf');
   DBF.Open;
   DBF.Edit;
   Field:=DBF.GetFieldByName('CALLS');
   Field.setAsInteger(Field.getAsInteger+1);
   DBF.Post;
   DBF.Close;
   DBF.Free;
End;

{###################  ShowTosserStats  ######################################
#   Purpose....: Originally show message stats only - expanded to system.   #
#   In params..: *                                                          #
#   Out params.: *                                                          #
#   Returns....: *                                                          #
#   Created....: 1170909 by Ozz                                             #
############################################################################}
procedure ShowTosserStats;
var
   DBF:THalcyonDataset;
   Field:TField;
   AllMessages,AllFiles,AllDownloads,AllFileAreas,AllCallers,AllUsers:LongWord;

Begin
   DBF.Init(Nil);
   DBF.SetFileName(ScriptRoot+'data/tosstats.dbf');
   DBF.Open;
   Field:=DBF.GetFieldByName('ELAPSE');
   FastWrite(#13#10+'   |19o|1CO|19o |1EFidonet Tosser: |13'+IntToCommaStr(Field.getAsInteger)+'s |12CPU Time|1F,');
   Field:=DBF.GetFieldByName('PACKETS');
   FastWrite(' |1EPackets: |13'+IntToCommaStr(Field.GetAsInteger)+'|1F,');
   Field:=DBF.GetFIeldByName('MESSAGES');
   AllMessages:=Field.GetAsInteger;
   FastWrite(' |1EMessages: |13'+IntToCommaStr(Field.GetAsInteger)+#13#10);
   DBF.Close;
   DBF.Free;

   DBF.Init(Nil);
   DBF.SetFileName(ScriptRoot+'data/totals.dbf');
   DBF.Open;
   Field:=DBF.GetFieldByName('CALLS');
   AllCallers:=Field.getAsInteger;
   Field:=DBF.GetFieldByName('UPLOADS');
   AllFiles:=Field.getAsInteger;
   Field:=DBF.GetFieldByName('DNLOADS');
   AllDownloads:=Field.getAsInteger;
   Field:=DBF.GetFieldByName('DNAREAS');
   AllFileAreas:=Field.getAsInteger;
   DBF.Close;
   DBF.Free;

   DBF.Init(Nil);
   DBF.SetFileName(ScriptRoot+'data/users.dbf');
   DBF.Open;
   AllUsers:=DBF.getRecordCount;
   DBF.Close;
   DBF.Free;

   FastWrite('   |19o|1CO|19o |1EThe Current System Date is |1C'+FormatTimeStamp('ddd mmm dd yyyy',TimeStamp)+'|1E and Time is |1C'+FormatTimeStamp('hh:nn |1Eam/pm',TimeStamp)+#13#10+
      '   |19o|1CO|19o |1ENumber of Files......|30'+PadLeft(IntToCommaStr(AllFiles),9)+
      '|1E   Number of File Areas.|30'+PadLeft(IntToCommaStr(AllFileAreas),9)+'|17'+#13#10+
      '   |19o|1CO|19o |1ENumber of Downloads..|30'+PadLeft(IntToCommaStr(AllDownloads),9)+'|17'+#13#10+
      '   |19o|1CO|19o |1ENumber of Users......|30'+PadLeft(IntToCommaStr(AllUsers),9)+
      '|1E   Number of Calls......|30'+PadLeft(IntToCommaStr(AllCallers),9)+'|17'+#13#10);
   //FastWrite('   |5F*|09 The last caller was '+PreviousCaller+' @ '+FormatTimeStamp('mmm dd yyyy hh:nn |0Eam/pm',PreviousCalled)+#13#10);
End;

{$IFDEF PLUGINS}
   {$I plugins/audit_connections.inc}
   {$I plugins/last_caller.inc}
{$ENDIF}

{################### Main() #################################################
#   Purpose....: Initial Code for Exchange Bulletin Board System            #
#   In params..: *                                                          #
#   Out params.: *                                                          #
#   Returns....: *                                                          #
#   Created....: 1150701 by Ozz                                             #
############################################################################}
Begin
   Randomize;
   ScriptRoot:=ExtractFilePath(ExecFilename);
   InitFastTTT1stTime;
   If not FileExists(ScriptRoot+'data/totals.dbf') then doCreateTotals;
   IncrementCallers;
{$IFDEF CODERUNNER}
   If not Session.Connected then Exit;
{$ENDIF}
   i18n.init;
Try

{$I plugins/detect_ansi.inc}

   Chain.Retrieve('HASANSI',HasAnsi);
   If not HasAnsi then begin
      Cls;
{$IFDEF CODERUNNER}
      Session.SendFile(ScriptRoot+'ansasc/welcome.asc',True);
{$ENDIF}
   End
   Else Begin
{$I plugins/detect_colors.inc}
      CLS(7,1);
      DisplayFileEx(ScriptRoot+'ansasc/welcome');
   End;
   ShowTosserStats;
   Pause;

   If not Connected then Exit;
   Title(1);

   FillBox(5,10,75,17,9,1,1);
{$IFDEF PLUGINS}
   audit_connection;
{$ELSE}
   FastWrite(1,7,14,0,'[|7C'+#223+'|0E]|0D Logged connection From: |1F'+Session.GetPeerIPAddress+#13#10+
      '|05    Reverse DNS on your IP: |1F'+Session.GetHostByIPAddress(Session.GetPeerIPAddress,1));
{$ENDIF}

   FastWrite(7,11,14,1,'[|7C'+#223+'|1E]|1B Test for ECHO... testing your terminal settings:');
   CursorTo(11,12);
   Ask('|1EType the letters OK and press [|1FENTER|1E]:','OK');
   If not Connected then Exit;

   CursorTo(7,13);
   NeedsEcho:=Ask('|1E[|7C'+#223+'|1E]|1F Did you see OOKK or OKOK on the previous line|1E?|1B','NY')='Y';
   If not Connected then Exit;
   Chain.Store('NEEDSECHO',NeedsEcho);

   FastWrite(7,15,14,1,'[|7C'+#223+'|1E]|1E The following word may or may not be blinking:'+
      #27'[5m |1CA|1EN|1BS|1DI|1F '+#27'[0m');
   CursorTo(11,16);
   If Ask('|1FIs it blinking|1E?|1B','YN')='Y' then begin
      Chain.Store('ANSIBLINK',True);
   end
   else begin
      Chain.Store('ANSIBLINK',False);
   end;

   If not Connected then Exit;
{$IFDEF PLUGINS}
   last_caller;
{$ENDIF}

   If LoginProcess() then Chain.Run(ScriptRoot+"checkmail.p")
   else Chain.Run(ScriptRoot+"goodbye.p");

finally
   i18n.Free;
end;
End.
