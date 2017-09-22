Procedure CreateKOTH;
Var
   IndexFiles,Schema:TStringList;
   DBF:THalcyonDataset;

Begin
   FastWriteln('|09New Game Initialized...|0A');
   DBF.Init(Nil);
   Schema.Init;
   Schema.Add('USERRECNO,N,20,0');   // users.dbf RECNO is the KEY!
   Schema.Add('CREATEDON,N,20,0');   // when was this record created?
   Schema.Add('LASTGEN,N,20,0');     // last tie generated Utils?
   Schema.Add('KINGB4,L,1,0');       // has been king in past?
   Schema.Add('HASTRIED,L,1,0');     // has attacked the hill?
   Schema.Add('UTILS,N,11,0');       // army size
   DBF.CreateDBF(ScriptRoot+'data/kinghill.dbf','',FoxPro,Schema);
   Schema.Free;
   DBF.setFilename(ScriptRoot+'data/kinghill.dbf');
   DBF.Open;
   DBF.IndexOn(ScriptRoot+'data/kinghill.cdx','PK','USERRECNO','.NOT.DELETED()',Unique,Ascending);
   DBF.Close;
   DBF.Free;
End;

Procedure doKingOfTheHill;
var
   DBF:tHalcyonDataset;
   IndexFiles:TStringList;
   Field:TField;
   Ws:String;
   Gen:Longint;

Begin
   Title; // Clears screen, writes my standard banner and ready to roll forward:
   If not FileExists(ScriptRoot+'data/kinghill.dbf') then createKOTH;
   DBF.Init(Nil);
   DBF.setFilename(ScriptRoot+'data/kinghill.dbf');
   IndexFiles:=DBF.getIndexFiles;
   IndexFiles.Add(ScriptRoot+'data/kinghill.cdx');
   DBF.Open;
   DBF.setIndexTag('PK');
   If not DBF.Find(IntToStr(Chain.ReadLongint('RECNO')),True,False) then begin
       FastWriteln('|0AWelcome newbie to an easy to use online game. Basically, you');
       FastWriteln('login every day and generate more armies. Once you think you');
       FastWriteln('have a large enough army - you attack the hill. If you have enough');
       FastWriteln('you will become king of the hill. As king your account automatically');
       FastWriteln('gets more time, more access to features and more. However, if you');
       FastWriteln('and fail, you will mose some of your armies and can try again on');
       FastWriteln('another day. Note: While king, everyone who attacks you will make');
       FastWriteln('you lose some armies. To regain, you must come down the hill (it');
       FastWriteln('is done secretely). The longer you are down the more of your army');
       FastWriteln('gets rebuilt. 24 hours = 100%.');
       DBF.Append;
       Field:=DBF.getFieldByName('USERRECNO');
       Field.setAsInteger(Chain.ReadLongint('RECNO'));
       Field:=DBF.getFieldByName('CREATEDON');
       Field.setAsInteger(Timestamp);
       Field:=DBF.getFieldByName('LASTGEN');
       Field.setAsInteger(0);
       Field:=DBF.getFieldByName('UTILS');
       Field.setAsInteger(1000);
       DBF.Post;
       FastWriteln('|0BAs a newbie you automatically get 1000 army men.');
       Pause;
       Title;
   End
   Else Begin
     Field:=DBF.getFieldByName('UTILS');
     FastWriteln('|0AWelcome back, you currently have '+IntToStr(Field.getAsInteger)+' army men.');
   End;
   Field:=DBF.getFieldByName('LASTGEN');
   If Field.getAsInteger<(Timestamp-65880) then begin
      FastWriteln('|0FIt have been long enough you can generate more army men,');
      Ws:=Ask('would you like to do so now','YN');
      If Ws='Y' then begin
         DBF.Edit;
         Field:=DBF.getFieldByName('LASTGEN');
         Field.setAsInteger(Timestamp);
         Gen:=Random(100)+(Chain.ReadLargeint('TIMELIMIT')-Timestamp);
         Field:=DBF.getFieldByName('UTILS');
         Field.setAsInteger(Gen+Field.getAsInteger);
         DBF.Post;
         If Gen>100 then FastWriteln('|0CWow! |0EYou generated another |0D'+IntToStr(Gen)+'|0E army men.')
         else FastWriteln('|0EAnother |0D'+IntToStr(Gen)+' |0Earmy men have joined your forces.');
      End;
   End
   Else Begin
      Gen:=(65880)-(Timestamp-Field.GetAsInteger); // Once every 18 hours //
      If Gen>0 then begin
         FastWrite(#13#10+'|0FYou have to wait |0A');
         If Gen>3600 then begin
            FastWrite(IntToStr(Gen div 3600)+' |0Fhours, and |0A');
            Gen:=Gen mod (Gen div 3600);
         End;
         FastWriteln(IntToStr(Gen)+' |0Fmore minutes before you can');
         FastWriteln('generate more army men.');
      End;
   End;
   Field:=DBF.getFieldByName('UTILS');
   If Field.getAsInteger<1000 then Fastwriteln(#13#10+'|04You do not have enough men to attack the hill.')
   else Begin
      Gen:=Field.getAsInteger;
      NewLine;
      Ws:=Ask('Would you like to attack the hill?','YN');
      If Ws='Y' then begin
         DBF.Edit;
         Field:=DBF.getFieldByName('HASTRIED');
         Field.setAsBoolean(True);
         DBF.Post;
      End;
   End;
Pause;
   DBF.Close;
   DBF.Free;
End;
