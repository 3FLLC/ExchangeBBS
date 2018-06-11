
uses
   databases, hashes, datetime, // needed to work with user profile/account
   environment, strings, ANSISockets, display; // needed to start a module

var
   ScriptRoot:String;

{$I includes/screencrt.inc}
{$I includes/bbskit.inc}
{$I plugins/title.inc}
{$I plugins/profile.inc}

var
   Ws,Ts,Email,FName,LName,Fullname,Nickname,City,State,PostalCode,Country,Password:String;
   Attempts:Longint;
   DBF:THalcyonDataset;
   IndexFiles:TStringList;
   Field:TField;
   i64:Int64;
   i32:Int32;

label
   TryPasswordAgain;

Begin
   Attempts:=0;
   ScriptRoot:=ExtractFilePath(ExecFilename);
   InitFastTTT;
   If not Chain.ReadBoolean('HASANSI') then Session.SetASCIIMode;
   If Chain.ReadBoolean('HASANSI') then Session.SetANSIMode;
   If Chain.ReadBoolean('HASUTF8') then Session.SetUTF8Mode;

   Title;
   Ts:=Ask('|0FVerify your email is   |0C: |30',PadRight(Chain.ReadString('EMAILADDR'),53));
   If Lowercase(Ts)=Chain.ReadString('EMAILADDR') then begin
      Email:=Chain.ReadString('EMAILADDR');
      Ws:=Ask('|0FWhat is your first name|0C: |30',Space(36));
      If (Ws<>'') then begin
         FName:=Uppercase(Ws);
         Ws:=Ask('|0FWhat is your last name |0C: |30',Space(36));
         If (Ws<>'') then begin
            LName:=Uppercase(Ws);
            Fullname:=Copy(FName+#32+LName,1,36);
            Ws:=Ask('|0FHandle/Alias/Nickname  |0C: |30',Space(36));
            If (Ws<>'') then begin
               Nickname:=Ws;
               Ws:=Ask('|0FCity you live in       |0C: |30',Space(35));
               If (Ws<>'') then begin
                  City:=Uppercase(Ws);
                  Ws:=Ask('|0FState you live in      |0C: |30',Space(35));
                  If (Ws<>'') then begin
                     State:=Uppercase(Ws);
                     Ws:=Ask('|0FYour postal code       |0C: |30','############');
                     If (Ws<>'') then begin
                        PostalCode:=Ws;
                        Ws:=Ask('|0FWhat country are you in|0C: |30',Space(35));
                        If (Ws<>'') then begin
                           Country:=Uppercase(Ws);
TryPasswordAgain:
                           NewLine;
                           Ws:=Ask('|0FEnter a strong password|0C: |30',PadRight('',50,'*'));
                           Ts:=Ask('|0FVerify your password   |0C: |30',PadRight('',50,'*'));
                           If (Ts=Ws) then begin
                              Password:=SHA1(Ws);
                              DBF.Init(Nil);
                              DBF.setFilename(ScriptRoot+'data/users.dbf');
                              IndexFiles:=DBF.getIndexFiles;
                              IndexFiles.Add(ScriptRoot+'data/users.cdx');
                              DBF.Open;
                              DBF.setIndexTag('PK');
                              DBF.Append;
                              Field:=DBF.getFieldByName('EMAIL');
                              Field.SetAsString(Email);
                              Field:=DBF.getFieldByName('HANDLE');
                              Field.SetAsString(Nickname);
                              Field:=DBF.getFieldByName('FIDONAME');
                              Field.SetAsString(Fullname);
                              Field:=DBF.getFieldByName('FNAME');
                              Field.SetAsString(FName);
                              Field:=DBF.getFieldByName('LNAME');
                              Field.SetAsString(LName);
                              Field:=DBF.getFieldByName('CITY');
                              Field.SetAsString(City);
                              Field:=DBF.getFieldByName('STATE');
                              Field.SetAsString(State);
                              Field:=DBF.getFieldByName('ZIPCODE');
                              Field.SetAsString(PostalCode);
                              Field:=DBF.getFieldByName('COUNTRY');
                              Field.SetAsString(Country);
                              Field:=DBF.getFieldByName('PASSWORD');
                              Field.SetAsString(Password);
                              Field:=DBF.getFieldByName('SECLEVEL');
                              Field.SetAsInteger(10);
                              Field:=DBF.getFieldByName('LASTON');
                              Field.SetAsInteger(Timestamp);
                              Field:=DBF.getFieldByName('CREATEDON');
                              Field.SetAsInteger(Timestamp);
                              Field:=DBF.getFieldByName('PWRDCHGON');
                              Field.SetAsInteger(Timestamp);
                              Field:=DBF.getFieldByName('LASTIP');
                              Field.SetAsString(Session.GetPeerIPAddress);
                              DBF.Post;
                              Chain.Store('RECNO',DBF.getRecNo);
                              FastWriteln('|0ACongratulations! |02You are account #'+IntToStr(DBF.getRecordCount)+', enjoy the system.');
                              DBF.Close;
                              DBF.Free;

                              i64:=Timestamp+60*15;
                              Chain.Store('TIMELIMIT',i64);
                              i64:=0;
                              Chain.Store('LASTON',i64);
                              i32:=10;
                              Chain.Store('SECLEVEL',i32);

                              FastWriteln('|0E* |02Check your email for a verification code to get full access.');
                              Pause;
                              EditProfile;
                              Chain.Run(ScriptRoot+'checkmail.p');
                           End
                           Else Begin
                              FastWriteln('|4F Passwords do not match... ');
                              Inc(Attempts);
                              If Attempts<4 then Goto TryPasswordAgain
                              Else Begin
                                 FastWriteln('|4E Aborting too many failed attempts. ');
                                 Chain.Run(ScriptRoot+'goodbye.p');
                              End;
                           End;
                        End;
                     End;
                  End;
               End;
            End;
         End;
      End;
   End
   Else Begin

   End;
   DisposeFastTTT;
End.
