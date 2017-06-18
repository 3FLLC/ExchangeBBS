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

Var
   ScriptRoot:String;
   TmpBool:Boolean;
   LastChar:Char;
   Ws:String;
   HasAnsi:Boolean;
   NeedsEcho:Boolean;
   SetTimeout:Int64;

{$I includes/FASTTMPT7.i}
{$I includes/bbskit.inc}
{$I includes/newsignup.inc}
{$I includes/loginprocess.inc}

procedure ShowTosserStats;
var
   DBF:THalcyonDataset;
   Field:TField;

Begin
   DBF.Init(Nil);
   DBF.SetFileName(ScriptRoot+'data/tosstats.dbf');
   DBF.Open;
   Field:=DBF.GetFieldByName('ELAPSE');
   FastWrite('        |5F*|09 Tosser: |03'+IntToCommaStr(Field.getAsInteger)+'s |0ECPU Time|09,');
   Field:=DBF.GetFieldByName('PACKETS');
   FastWrite(' Packets: |03'+IntToCommaStr(Field.GetAsInteger)+'|09');
   Field:=DBF.GetFIeldByName('MESSAGES');
   FastWrite(' Messages: |03'+IntToCommaStr(Field.GetAsInteger)+#13#10);
   DBF.Close;
   DBF.Free;
   Yield(3000);
End;

{$IFDEF PLUGINS}
{$I plugins/audit_connections.i}
{$I plugins/last_caller.i}
{$ENDIF}

Begin
   Randomize;
   ScriptRoot:=ExtractFilePath(ExecFilename);
   InitFastTTT;

{$I includes/detect_ansi.i}

   CLS(7,0);
   Chain.Retrieve('HASANSI',HasAnsi);
   If not HasAnsi then begin
      Session.SendFile(ScriptRoot+'ansasc/welcome.asc',True);
   End
   Else Begin
      If FileExists(ScriptRoot+'ansasc/welcome.ans') then
         Session.SendFile(ScriptRoot+'ansasc/welcome.ans',True)
      Else Session.SendFile(ScriptRoot+'ansasc/welcome.asc',True);
   End;
   ShowTosserStats;
   Pause;
   CLS(0,7);

   If not Connected then Exit;
   FBox(11,3,71,4,15,0,0);
   FBox(10,2,70,3,15,4,0);
   FastWrite(12,2,14,4,'Exchange Bulletin Board System');
   FastWrite(48,2,15,4,'(c) 2017 by Ozz Nixon');
   FastWrite(15,3,0,4,'This BBS is open source and is actively developed.');

   FBox(5,10,75,17,9,1,1);
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

// If you do not trust auto-detect, then implement this code:
   FastWrite(7,15,14,1,'[|7C'+#223+'|1E]|1E The following word may or may not be blinking:'+
      #27'[5m |1CA|1EN|1BS|1DI|1F '+#27'[0m');
   CursorTo(11,16);
   HasAnsi:=Ask('|1FIs it blinking|1E?|1B','YN')='Y';
   If not Connected then Exit;
   Chain.Store('HASANSI',HasAnsi); // user trumps auto detect!

{$IFDEF PLUGINS}
   last_caller;
{$ENDIF}

   If LoginProcess() then Chain.Run(ScriptRoot+"checkmail.p")
   else begin
      Session.Prompt('|0E[|7C'+#223+'|0E]|0E Press [|0FENTER|0E] to disconnect.|07',30);
      Session.Writeln('Goodbye.');
      Session.Disconnect;
   end;
End.
