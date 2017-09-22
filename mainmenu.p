Program MainMenu.v1170607;

/////////////////////////////////////////////////////////////////////////////
// MODULE ID: MAINMENU
// Description:
/////////////////////////////////////////////////////////////////////////////
uses
   Variants,
   Classes,
   Databases,
   Math,        // GAMES - RANDOM
   Datetime,    // Formating Timestamp (profile for example)
   Display,     // Color Names
   Strings,     //
   ANSISockets, // Extends Session to Support ANSI/AVATAR/PIPE/WWIV commands!
   Environment, // Add FILE routines, etc.
   Chains;      // Add Chain Memory and Chaining...

Const
   SHOR=#196; {─}
   SVER=#179; {│} {�}
   SCRS=#197; {┼}
   STLT=#180; {┤}
   STRT=#195; {├}
   STUP=#193; {┴}
   STDN=#194; {┬}
   SLTL=#218; {┌}
   SLTR=#191; {┐}
   SLBL=#192; {└}
   SLBR=#217; {┘}

var
   ScriptRoot:String;
   Ws,Ss:String;

{$I includes/screencrt.inc}
{$I includes/bbskit.inc}
{$I plugins/title.inc}
{$I plugins/filelist.inc}
{$I plugins/profile.inc}
{$I plugins/kinghill.inc}
{$I /var/www/CMS/im.inc}

Function MinutesLeft:String;
var
   i64:Int64;

Begin
   If Chain.ReadLargeint('TIMELIMIT')<Timestamp then begin
      FastWriteln('|0CYou have used all of your time for this connection.'+#13#10#13#10+
         '|0EYou can reconnect and enjoy another hour if your would like.');
      Result:='0';
      Hangup;
   End
   Else Begin
      i64:=Chain.ReadLargeint('TIMELIMIT')-Timestamp;
      Result:=IntToStr(i64 div 60);
   End;
End;

procedure AboutExchangeBBS;
Begin
   Title;
   FastWrite(1,5,'|0BExchange Bulletin Board System is a collection of 35 years experience in BBS'+#13#10+
      'development. Originally started using a "|07Tandy Model I|0B" computer from Radio'+#13#10+
      'shack, using the purchased source code to (|0EY|0B)our (|0EB|0B)ulletin (|0EB|0B)oard (|0ES|0B)ystem.'+#13#10#13#10+
      'Which used a video fossil, everything you wrote to the screen was sent to the'+#13#10+
      'modem. And of course anything received from the modem was redirected to the'+#13#10+
      'local keyboard buffer. It took a little buffering in itself, as the keyboard'+#13#10
      'buffer was 4 bytes.'+#13#10#13#10);
   FastWrite("|0EYBBS was written in Tandy's Basic, and was about 30 pages long printed out."+#13#10#13#10+
      '|0AIt had just the basic functionality, new user sign-up, sysop feedback, chat,'+#13#10+
      'one message base, one public and one private file area, you could upload and'+#13#10+
      'download using xmodem-checksum protocol, a bbs listing, and goodbye screen.'+#13#10#13#10+
      '|03It did not have any doorway or online games, no oneliners, etc.'+#13#10);
   if PauseBar="S" then exit;
   Title;
   FastWrite(1,5,'|0BYBBS evolved in to |07Cygnus-X|0B (name inspired by |0CRush |09Hemisphere LP|0B), when the'+#13#10+
      'code was ported to my "|07Tandy Model 4p|0B" just before I moved from Tampa to'+#13#10+
      'Fort Walton Beach Florida.'+#13#10#13#10+
      'By now (1985-ish), I had implemented support for ANSI color, and embedded a'+#13#10+
      'couple BBS games - |0FKing of the Hill|0B (thanks to Joe Griffith in Tampa), and'+#13#10+
      'some card games, and a basic chess engine (Thanks to Eric T. of "The Zoo BBS"'+#13#10+
      'in Tampa). These few features made the BBS very attractive in FWB.'+#13#10#13#10+
      'Cygnus-X was later rewritten from scratch when I got a |07Tandy 1000|0B (PC Clone)...'+#13#10+
      'where I attempted to implement |0AFidonet|0B support with very little guidance or'+#13#10+
      'knowledge of the |0AFTSC specifications|0B. This version of the code was called'+#13#10+
      '|0EXenophone BBS, |0Bwhich I still have to this date - a self contained BBS.'+#13#10);
   if PauseBar="S" then exit;
   Title;
   FastWrite(1,5,'|0B35 years later, I developed this from scratch using a compiler I have been'+#13#10+
      'writing since 2000, called |0DModern Pascal|0B. |0EExchangeBBS|0B is a mental repository'+#13#10+
      'of everything I had incorporated over the years.'+#13#10#13#10+
      'ExchangeBBS supports multiple languages even a humours one called |0EJive|0B.'+#13#10#13#10+
      'ExchangeBBS utilized the |0EHalcyon7 xBase|0B code which is built-in to Modern Pascal'+#13#10+
      'for all data storage, even the hundred thousand plus messages.'+#13#10#13#10+
      'ExchangeBBS incorporates different echo mail and mail relay solutions,'+#13#10+
      'allowing the BBS to be a true exchange of information. I also own gigabytes'+#13#10+
      'of CD-ROM collections from the BBS era, all are online for users to download'+#13#10+
      'to your system using one of the transfer protocols that I have recoded.'+#13#10#13#10+
      'Another fine point to share, is all of this BBS is released to the public as'+#13#10+
      '|0CFree Open Source|0B on |0CGitHub|0B and here on the system itself.');
   Pause;
End;

Procedure BBSListing;
Begin
   CLS(14,0);
   DisplayFileEx(ScriptRoot+'ansasc/bbslisting',True);
//   Pause;
End;

Procedure DoorGames;
Begin
   While Connected do begin
      If not Chain.ReadBoolean('EXPERT') then begin
         CLS(7,0);
         DisplayFileEx(ScriptRoot+'ansasc/doors');
      End;
{$IFDEF CODERUNNER}
      If Session.CountWaiting then Session.ReadStr(Session.CountWaiting,500);
{$ENDIF}
      Ws:=#13#10#27+"[0;40;37m  "+#27+"[1;33m�"+#27+"[0;33m�"+#27+"[1;30m�"+#27+"[0m "+#27+"[1;31mD"+#27+"[35mo"+#27+
         "[0;35mo"+#27+"[31mr"+#27+"[32ms "+#27+"[1;31mM"+#27+"[35me"+#27+"[0;35mn"+#27+"[31mu"+#13#10#27+
         "[37m�"+#27+"[1m�"+#27+"[33mٳ�"+#27+"[0;33m��"+#27+"[37m "+#27+"[1;35m"+MinutesLeft()+#27+"[36m "+#27+"[0;35mMinutes Left"+#13#10#27+
         "[37m  "+#27+"[1;30m�"+#27+"[0;33m��"+#27+"[37m "+#27+"[1;31mC"+#27+"[35mh"+#27+"[0;35mo"+#27+"[31mi"+#27+"[37mc"+#27+"[1;30me:|0E";
      //Ws:=Ask(#13#10+'|0E[|0D'+FormatTimestamp('HH:NN am/pm',Timestamp)+'|0E] [|0A'+MinutesLeft()+' |0Bminutes left|0E] Which door would you like to run?',' ');
      Ws:=Ask(Ws,' ');
      If Ws<>'' then
         Case Ws[1] of
            'I','i':doIMLogin();
            'K','k':if Chain.ReadLongint('SECLEVEL')>9 then doKingOfTheHill //koth
                    Else begin
                       FastWriteln('This feature requires a permanent account.'+#13#10);
                       Pause;
                    End;
            'X','x':Break;
         End;
   End;
End;

Procedure NewsListing;
Begin
   CLS(14,0);
   DisplayFileEx(ScriptRoot+'ansasc/news',True);
//   Pause;
End;

Begin
   ScriptRoot:=ExtractFilePath(ExecFilename);
   InitFastTTT;
   If not Chain.ReadBoolean('HASANSI') then Session.SetASCIIMode;
   If Chain.ReadBoolean('HASANSI') then Session.SetANSIMode;
   If Chain.ReadBoolean('HASUTF8') then Session.SetUTF8Mode;
   While Connected do begin
      If not Chain.ReadBoolean('EXPERT') then begin
         CLS(7,0);
         DisplayFileEx(ScriptRoot+'ansasc/mainmenu');
      End;
{$IFDEF CODERUNNER}
      If Session.CountWaiting then Session.ReadStr(Session.CountWaiting,500);
{$ENDIF}
      Ws:=#13#10#27+"[0;40;37m  "+#27+"[1;33m�"+#27+"[0;33m�"+#27+"[1;30m�"+#27+"[0m "+#27+"[1;34mM"+#27+"[0;36ma"+#27+
         "[1;30mi"+#27+"[0;34mn"+#27+"[37m "+#27+"[1;36mM"+#27+"[34me"+#27+"[0;36mn"+#27+"[1;30mu"+#13#10#27+
         "[0m�"+#27+"[1m�"+#27+"[33mٳ�"+#27+"[0;33m��"+#27+"[37m "+#27+"[1;36m"+MinutesLeft()+" "+#27+"[0;36mMinutes Left"+#13#10#27+
         "[37m  "+#27+"[1;30m�"+#27+"[0;33m��"+#27+"[37m "+#27+"[1;36mC"+#27+"[34mom"+#27+"[0;36mma"+#27+"[1;30mnd"+#27+"[0;34m:";
      //Ws:=Ask(#13#10+'|0E[|0D'+FormatTimestamp('HH:NN am/pm',Timestamp)+'|0E] [|0A'+MinutesLeft()+' |0Bminutes left|0E] What would you like to do?',' ');
      Ws:=Ask(Ws+'|0E',' ');
      If Ws<>'' then
         Case Ws[1] of
            'A','a':AboutExchangeBBS;
            'B','b':BBSListing;
            'D','d':DoorGames;
            'F','f':FileAreaSelect;
            'G','g':Break;
            'K','k':if Chain.ReadLongint('SECLEVEL')>9 then doKingOfTheHill //koth
                    Else begin
                       FastWriteln('This feature requires a permanent account.'+#13#10);
                       Pause;
                    End;
            'N','n':NewsListing;
            'P','p':if Chain.ReadLongint('SECLEVEL')>9 then EditProfile
                    Else begin
                       FastWriteln('This feature requires a permanent account.'+#13#10);
                       Pause;
                    End;
            'X','x':Begin
               Chain.Store('EXPERT',not Chain.ReadBoolean('EXPERT'));
               If Chain.ReadBoolean('EXPERT') then FastWriteln('Expert mode on.')
               Else FastWriteln('Expert mode off.');
            End;
            '?':If Chain.ReadBoolean('EXPERT') then begin
               CLS(7,0);
               DisplayFileEx(ScriptRoot+'ansasc/mainmenu');
            End;
            '@':Begin
               DisposeFastTTT;
               Chain.Run(ScriptRoot+'mainmenu.p');
            End;
         End;
   End;
   DisposeFastTTT;
   Chain.Run(ScriptRoot+'goodbye.p');
End.
