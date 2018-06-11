Const
   SHOR=#196; {Γ~T~@}
   SVER=#179; {Γ~T~B} {≥}
   SCRS=#197; {Γ~T╝}
   STLT=#180; {Γ~Tñ}
   STRT=#195; {Γ~T~\}
   STUP=#193; {Γ~T┤}
   STDN=#194; {Γ~T¼}
   SLTL=#218; {Γ~T~L}
   SLTR=#191; {Γ~T~P}
   SLBL=#192; {Γ~T~T}
   SLBR=#217; {Γ~T~X}

Procedure DrawDownbox(X,Y,W,H,Fg,Bg,Fg2:Byte);
var
    Rows:Byte;

Begin
   FastWrite(X,Y,'|'+IntToHex((Bg shl 4)+Fg)+PadRight(SLTL,Succ(W-X),SHOR)+'|'+IntToHex((Bg shl 4)+Fg2)+SLTR); // TOP LINE //
   For Rows:=Y+1 to (H-Y) do begin // VERICLE
      FastWrite(X,Rows,'|'+IntToHex((Bg shl 4)+Fg)+SVER);
      FastWrite((W-X)+2,Rows,'|'+IntToHex((Bg shl 4)+Fg2)+SVER);
   End;
   FastWrite(X,Succ(H-Y),'|'+IntToHex((Bg shl 4)+Fg2)+PadRight(SLBL,Succ(W-X),SHOR)+SLBR); // BOTTOM LINE //
End;

Procedure EditProfile;
Var
   DBF:THalcyonDataset;
   IndexFiles:TStringList;
   fEmail,fHandle,fFidoname,fFName,fLname,fAddress,fCity,fState,fZipcode,
   fCountry,fGender,fBirthYear,fLastOn,fCreatedOn,fVerifiedOn,fPassChange,
   fSecLevel,fLastIP,fQwk,fFido,fNet,fSMTP,fPOP4,fIMAP4,fNNTP,fWeb:TField;
   Action:String;

procedure ShowFieldData;
begin
   DrawDownBox(1,4,19,20,0,1,9);
   FastWrite(2, 5,'|1A   Email Address  |19'+SVER+' |18'+PadRight(fEmail.getAsString,50)+'|07');
   FastWrite(2, 6,'|131|18] |1AFirst Name     |19'+SVER+' |0A'+PadRight(fFName.getAsString,35)+'|07');
   FastWrite(2, 7,'|132|18] |1ALast Name      |19'+SVER+' |0A'+PadRight(fLName.getAsString,35)+'|07');
   FastWrite(2, 8,'|1A   Full Fido Name |19'+SVER+' |0A'+PadRight(fFidoname.getAsString,36)+'|07');
   FastWrite(2, 9,'|133|18] |1AHandle/Nickname|19'+SVER+' |0A'+PadRight(fHandle.getAsString,36)+'|07');
   FastWrite(2,10,'|134|18] |1AGender         |19'+SVER+'');
   If (fGender.getAsString='M') then FastWrite(22,10,'|0CMale  ')
   Else FastWrite(22,10,'|09Female');
   FastWrite(2,11,'|1A   Password       |19'+SVER+' |18********HASH********|07');
   FastWrite(2,12,'|135|18] |1BAddress        |19'+SVER+' |0A'+PadRight(fAddress.getAsString,35)+'|07');
   FastWrite(2,13,'|136|18] |1BCity           |19'+SVER+' |0A'+PadRight(fAddress.getAsString,35)+'|07');
   FastWrite(2,14,'|137|18] |1BState          |19'+SVER+' |0A'+PadRight(fAddress.getAsString,35)+'|07');
   FastWrite(2,15,'|138|18] |1BZipcode        |19'+SVER+' |0A'+PadRight(fZipcode.getAsString,12)+'|07');
   FastWrite(2,16,'|139|18] |1BCountry        |19'+SVER+' |0A'+PadRight(fCountry.getAsString,30)+'|07');

   FastWrite(1,19,'|0CAccess: |0FQWK |0C| |0FFIDO |0C| |0FNETMAIL |0C| |0FSMTP Sender |0C| |0FPOP Recv |0C| |0FIMAP Recv |0C| |0FNNTP |0C| |0FWEB');
   If fQwk.getAsBoolean then FastWrite(9,20,'|2EYES')
   Else FastWrite(9,20,'|4FNO');
   If fFido.getAsBoolean then FastWrite(15,20,'|2EYES')
   Else FastWrite(15,20,'|4FNO');
   If fNet.getAsBoolean then FastWrite(22,20,'|2EYES')
   Else FastWrite(22,20,'|4FNO');
   If fSMTP.getAsBoolean then FastWrite(32,20,'|2EYES')
   Else FastWrite(32,20,'|4FNO');
   If fPop4.getAsBoolean then FastWrite(46,20,'|2EYES')
   Else FastWrite(46,20,'|4FNO');
   If fIMap4.getAsBoolean then FastWrite(57,20,'|2EYES')
   Else FastWrite(57,20,'|4FNO');
   If fNNTP.getAsBoolean then FastWrite(69,20,'|2EYES')
   Else FastWrite(69,20,'|4FNO');
   If fWeb.getAsBoolean then FastWrite(76,20,'|2EYES')
   Else FastWrite(76,20,'|4FNO');

   FastWrite(1,21,'|03 Security Level |0D: |1E'+PadRight(IntToCommaStr(fSecLevel.getAsInteger),6)+'|07');
   FastWrite(42,21,'|03 Created On     |0D: |17'+FormatTimestamp('ddd, mmm dd yy hh:ss',fCreatedOn.getAsInteger)+'|07');
   FastWrite(1,22,'|03 Verified On    |0D: |17'+FormatTimestamp('ddd, mmm dd yy hh:ss',fVerifiedOn.getAsInteger)+'|07');
   FastWrite(42,22,'|03 Pswd Changed On|0D: |17'+FormatTimestamp('ddd, mmm dd yy hh:ss',fPassChange.getAsInteger)+'|07');
   If Chain.varExists('LASTON') then begin
      FastWrite(1,23,'|03 Last On        |0D: |17'+FormatTimestamp('ddd, mmm dd yy hh:ss',Chain.ReadLargeInt('LASTON'))+'|07');
      FastWrite(42,23,'|03 Last On From   |0D: |17'+Chain.ReadString('LASTIP')+'|07');
   End;
end;
(*
      Schema.Add('BIRTHYYYY,N,6,0');    //
*)

var
   WS:String;

Begin
   Title(1);
   CursorTo(3,3);
   FastWrite('|4CAccount #'+IntToStr(Chain.ReadLongint('RECNO')));
   DBF.Init(Nil);
   DBF.setFilename(ScriptRoot+'data/users.dbf');
   IndexFiles:=DBF.getIndexFiles;
   IndexFiles.Add(ScriptRoot+'data/users.cdx');
   DBF.Open;
   DBF.setIndexTag('PK');
   DBF.First;
   While DBF.getRecNo<>Chain.ReadLongint('RECNO') do DBF.Next;
   //DBF.setRecNo(Chain.ReadLongint('RECNO'));

   fEmail:=DBF.getFieldByName('EMAIL');
   fFName:=DBF.getFieldByName('FNAME');
   fLName:=DBF.getFieldByName('LNAME');
   fHandle:=DBF.getFieldByName('HANDLE');
   fFidoname:=DBF.getFieldByName('FIDONAME');

   fAddress:=DBF.getFieldByName('ADDRESS');
   fCity:=DBF.getFieldByName('CITY');
   fState:=DBF.getFieldByName('STATE');
   fZipcode:=DBF.getFieldByName('ZIPCODE');
   fCountry:=DBF.getFieldByName('COUNTRY');

   fGender:=DBF.getFieldByName('GENDER');
   fBirthYear:=DBF.getFieldByName('BIRTHYYYY');

   fSecLevel:=DBF.getFieldByName('SECLEVEL');
   fCreatedOn:=DBF.getFieldByName('CREATEDON');
   fVerifiedOn:=DBF.getFieldByName('VERIFIEDON');
   fPassChange:=DBF.getFieldByName('PWRDCHGON');

   fQwk:=DBF.getFieldByName('OKQWK');
   fFido:=DBF.getFieldByName('OKFIDO');
   fNet:=DBF.getFieldByName('OKNET');
   fSMTP:=DBF.getFieldByName('OKSMTP');
   fPOP4:=DBF.getFieldByName('OKPOP4');
   fIMAP4:=DBF.getFieldByName('OKIMAP4');
   fNNTP:=DBF.getFieldByName('OKNNTP');
   fWeb:=DBF.getFieldByName('OKWEB');

   While true do begin
      ShowFieldData;

      FastWrite(1,24,10,0,'Command (1..9,Q=Quit) [_]');/*print the menu*/
      CursorTo(30,20);
      Action:=OneKey('123456789Qq',True);
      If Action='' then Break;
      ClearLine(24,7,0);
      Case Action[1] of
         '1':Begin
            CursorTo(21 ,6);
            Ws:=Ask('|30',PadRight(fFName.getAsString,36));
            If (Ws<>'') then begin
               DBF.Edit;
               fFName.SetAsString(Uppercase(Ws));
               DBF.Post;
            End;
         End;
         '2':Begin
            CursorTo(21 ,7);
            Ws:=Ask('|30',PadRight(fLName.getAsString,36));
            If (Ws<>'') then begin
               DBF.Edit;
               fLName.SetAsString(Uppercase(Ws));
               DBF.Post;
            End;
         End;
         '3':Begin
            CursorTo(21 ,9);
            Ws:=Ask('|30',PadRight(fHandle.getAsString,37));
            If (Ws<>'') then begin
               DBF.Edit;
               fHandle.SetAsString(Ws);
               DBF.Post;
            End;
         End;
         '4':Begin
            DBF.Edit;
            If fGender.getAsString='M' then fGender.setAsString('F')
            else fGender.setAsString('M');
            DBF.Post;
         End;
         '5':Begin
            CursorTo(21 ,12);
            Ws:=Ask('|30',PadRight(fAddress.getAsString,36));
            If (Ws<>'') then begin
               DBF.Edit;
               fAddress.SetAsString(Ws);
               DBF.Post;
            End;
         '6':Begin
            CursorTo(21 ,13);
            Ws:=Ask('|30',PadRight(fCity.getAsString,36));
            If (Ws<>'') then begin
               DBF.Edit;
               fCity.SetAsString(Ws);
               DBF.Post;
            End;
         '7':Begin
            CursorTo(21 ,14);
            Ws:=Ask('|30',PadRight(fState.getAsString,36));
            If (Ws<>'') then begin
               DBF.Edit;
               fState.SetAsString(Ws);
               DBF.Post;
            End;
         '8':Begin
            CursorTo(21 ,15);
            Ws:=Ask('|30',PadRight(fZipcode.getAsString,13));
            If (Ws<>'') then begin
               DBF.Edit;
               fZipcode.SetAsString(Ws);
               DBF.Post;
            End;
         End;
         '9':Begin
            CursorTo(21 ,16);
            Ws:=Ask('|30',PadRight(fCountry.getAsString,36));
            If (Ws<>'') then begin
               DBF.Edit;
               fCountry.SetAsString(Ws);
               DBF.Post;
            End;
         End;
         'Q','q':Break;
      End;
   End;
   DBF.Close;
   DBF.Free;
End;
