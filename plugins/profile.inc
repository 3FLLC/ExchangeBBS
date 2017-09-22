Procedure DrawDownbox(X,Y,W,H,Fg,Bg,Fg2:Byte);
var
    Rows:Byte;

Begin
   FastWrite(X,Y,'|'+IntToHex((Bg shl 4)+Fg)+PadRight(SLTL,Succ(W-X),SHOR)+'|'+IntToHex((Bg shl 4)+Fg2)+SLTR); // TOP LINE //
   For Rows:=Y+1 to Succ(H-Y) do begin // VERICLE
      FastWrite(X,Rows,'|'+IntToHex((Bg shl 4)+Fg)+SVER);
      FastWrite(W-X,Rows,'|'+IntToHex((Bg shl 4)+Fg2)+SVER);
   End;
   FastWrite(X,H,'|'+IntToHex((Bg shl 4)+Fg2)+PadRight(SLBL,Succ(W-X),SHOR)+SLBR); // BOTTOM LINE //
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
   DrawDownBox(1,4,19,11,0,1,9);
   FastWrite(2, 5,'|131|18] |1AEmail Address  |19'+SVER+' |1F'+PadRight(fEmail.getAsString,50)+'|07');
   FastWrite(2, 6,'|132|18] |1AFirst Name     |19'+SVER+' |1F'+PadRight(fFName.getAsString,35)+'|07');
   FastWrite(2, 7,'|133|18] |1ALast Name      |19'+SVER+' |1F'+PadRight(fLName.getAsString,35)+'|07');
   FastWrite(2, 8,'|134|18] |1AFull Name      |19'+SVER+' |1F'+PadRight(fFidoname.getAsString,36)+'|07');
   FastWrite(2, 9,'|135|18] |1AHandle/Nickname|19'+SVER+' |1F'+PadRight(fHandle.getAsString,36)+'|07');
   FastWrite(2,10,'|136|18] |1AGender         |19'+SVER+'');
   If (fGender.getAsString='M') then FastWrite(22,10,'|1FMale  ')
   Else FastWrite(22,10,'|1FFemale');
   FastWrite(35,10,'|08[|037|08] |0APassword |0D: |1F********|07');

   FastWrite(1,12,'|08[|038|08] |0BAddress        |0D: |1F'+PadRight(fAddress.getAsString,35)+'|07');
   FastWrite(1,13,'|08[|039|08] |0BCity           |0D: |1F'+PadRight(fAddress.getAsString,35)+'|07');
   FastWrite(1,14,'|08[|03A|08] |0BState          |0D: |1F'+PadRight(fAddress.getAsString,35)+'|07');
   FastWrite(1,15,'|08[|03B|08] |0BZipcode        |0D: |1F'+PadRight(fZipcode.getAsString,12)+'|07');
   FastWrite(35,15,'|08[|03C|08] |0BCountry  |0D: |1F'+PadRight(fCountry.getAsString,30)+'|07');

   FastWrite(1,17,'|0CAccess: |0FQWK |0C| |0FFIDO |0C| |0FNETMAIL |0C| |0FSMTP Sender |0C| |0FPOP Recv |0C| |0FIMAP Recv |0C| |0FNNTP |0C| |0FWEB');
   If fQwk.getAsBoolean then FastWrite(9,18,'|2EYES')
   Else FastWrite(9,18,'|4FNO');
   If fFido.getAsBoolean then FastWrite(15,18,'|2EYES')
   Else FastWrite(15,18,'|4FNO');
   If fNet.getAsBoolean then FastWrite(22,18,'|2EYES')
   Else FastWrite(22,18,'|4FNO');
   If fSMTP.getAsBoolean then FastWrite(32,18,'|2EYES')
   Else FastWrite(32,18,'|4FNO');
   If fPop4.getAsBoolean then FastWrite(46,18,'|2EYES')
   Else FastWrite(46,18,'|4FNO');
   If fIMap4.getAsBoolean then FastWrite(57,18,'|2EYES')
   Else FastWrite(57,18,'|4FNO');
   If fNNTP.getAsBoolean then FastWrite(69,18,'|2EYES')
   Else FastWrite(69,18,'|4FNO');
   If fWeb.getAsBoolean then FastWrite(76,18,'|2EYES')
   Else FastWrite(76,18,'|4FNO');

   FastWrite(1,20,'|03 Security Level |0D: |1E'+PadRight(IntToCommaStr(fSecLevel.getAsInteger),6)+'|07');
   FastWrite(42,20,'|03 Created On     |0D: |17'+FormatTimestamp('ddd, mmm dd yy hh:ss',fCreatedOn.getAsInteger)+'|07');
   FastWrite(1,21,'|03 Verified On    |0D: |17'+FormatTimestamp('ddd, mmm dd yy hh:ss',fVerifiedOn.getAsInteger)+'|07');
   FastWrite(42,21,'|03 Pswd Changed On|0D: |17'+FormatTimestamp('ddd, mmm dd yy hh:ss',fPassChange.getAsInteger)+'|07');
   If Chain.varExists('LASTON') then begin
      FastWrite(1,22,'|03 Last On        |0D: |17'+FormatTimestamp('ddd, mmm dd yy hh:ss',Chain.ReadLargeInt('LASTON'))+'|07');
      FastWrite(42,22,'|03 Last On From   |0D: |17'+Chain.ReadString('LASTIP')+'|07');
   End;
end;
(*
      Schema.Add('BIRTHYYYY,N,6,0');    //
*)

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

      FastWrite(1,24,10,0,'Command (1..9,A,B,C,Q=Quit) [_]');/*print the menu*/
      CursorTo(30,24);
      Action:=OneKey('ABCabc123456789Qq',True);
      If Action='' then Break;
      ClearLine(24,7,0);
      Case Action[1] of
         'Q','q':Break;
      End;
   End;
   DBF.Close;
   DBF.Free;
End;
