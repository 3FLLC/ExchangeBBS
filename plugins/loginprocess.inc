{%MainUnit welcome.p}

Function GetPassword:String;
Begin
   CursorTo(24,11);
   Result:=Ask('|70','********************');
   CursorTo(1,16);
End;

Function LoginProcess:Boolean;
Var
   Attempts:Byte;
   Ws,Ts:String;
   DBF:THalcyonDataset;
   IndexFiles:TStringList;
   Field:TField;
   I64:Int64;
   I32:Int32;

Begin
   Cls(7,0);
   DisplayFileEx(ScriptRoot+'ansasc/login');
   FastWrite(1,18,'|0BThank you for defining your terminal capabilities...');
   FastWrite(1,19,'|0FPlease login, to skim our software use |5Fguest@domain.com|0F, pw: |5Fguest|0F');
   Attempts:=0;
   While Attempts<3 do begin
      CursorTo(24,7);
      Ws:=Ask('|74',Space(50));
      CursorTo(1,16);
      if (Pos('@',Ws)<1) then begin
         FastWriteln('|4EInvalid, email address format!|07');
         Inc(Attempts);
      End
      else begin
         if lowercase(ws)!="guest@domain.com" then begin
            DBF.Init(Nil);
            DBF.setFilename(ScriptRoot+'data/users.dbf');
            IndexFiles:=DBF.getIndexFiles;
            IndexFiles.Add(ScriptRoot+'data/users.cdx');
            DBF.Open;
            DBF.setIndexTag('PK');
            If DBF.Find(Lowercase(Ws),True,False) then begin
               Field:=DBF.getFieldByName('PASSWORD');
               ClearLine(16,7,0);
               ClearLine(17,7,0);
               Ts:=GetPassword;
               If SHA1(Ts)=Field.getAsString then begin
                  Chain.Store('EMAILADDR',Lowercase(Ws));
                  Field:=DBF.getFieldByName('SECLEVEL');
                  Chain.Store('SECLEVEL',Field.getAsInteger);
                  If Field.getAsInteger<20 then i64:=Timestamp+60*35
                  Else If Field.getAsInteger<30 then i64:=Timestamp+60*45
                  Else If Field.getAsInteger<61 then i64:=Timestamp+60*60
                  Else If Field.getAsInteger<101 then i64:=Timestamp+60*90
                  Else If Field.getAsInteger<201 then i64:=Timestamp+60*120
                  Else i64:=Timestamp+60*300;
                  Chain.Store('TIMELIMIT',i64);
                  Field:=DBF.getFieldByName('FIDONAME');
                  Chain.Store('USERNAME',Field.getAsString);
                  Field:=DBF.getFieldByName('HANDLE');
                  Chain.Store('HANDLE',Field.getAsString);
                  Chain.Store('RECNO',DBF.getRecNo);
                  Chain.Store('EXPERT',False);
                  Field:=DBF.getFieldByName('FNAME');
                  ClearLine(16,7,0);
                  ClearLine(17,7,0);
                  ClearLine(18,7,0);
                  ClearLine(19,7,0);
                  FastWriteln('|0BWELCOME BACK '+Field.getAsString+'!     ');
                  Field:=DBF.getFieldByName('LASTON');
                  i64:=Field.getAsInteger;
                  FastWriteln('|0EYou were last on '+FormatTimestamp('ddd, mmm dd yyyy hh:nn',i64));
                  Chain.Store('LASTON',i64);
                  DBF.Edit;
                  Field:=DBF.getFieldByName('LASTON');
                  Field.SetAsInteger(Timestamp);
                  Field:=DBF.getFieldByName('LASTIP');
                  Chain.Store('LASTIP',Field.getAsString);
{$IFDEF CODERUNNER}
                  Field.SetAsString(Session.GetPeerIPAddress);
{$ELSE}
                  Field.SetAsString('CONSOLE');
{$ENDIF}
                  DBF.Post;
                  DBF.Close;
                  DBF.Free;
                  Result:=True;
                  Pause;
                  Exit;
               End
               Else FastWrite(24,11,'|70'+Space(22));
               FastWrite(1,16,'|4EIncorrect password.'); //+#32+SHA1(Ts));
               Inc(Attempts);
            End
            Else Begin
               FastWriteln('|04Account not found.            ');
               If Ask('|0E[|7C'+#220+'|0E]|0F Are you a |05*|0DNEW|05*|0F user|0E?|0B','NY')='Y' then begin
                  DBF.Close;
                  DBF.Free;
                  Chain.Store('EMAILADDR',Lowercase(Ws));
                  Chain.Run(ScriptRoot+'newuser.p');
               End;
            end;
            DBF.Close;
            DBF.Free;
         end
         else begin
            If lowercase(GetPassword)=="guest" then begin
               Chain.Store('USERNAME',"GUEST USER");
               Chain.Store('HANDLE',"GUEST");
               Chain.Store('RECNO',0);
               Chain.Store('EXPERT',False);
               i64:=Timestamp+60*15;
               Chain.Store('TIMELIMIT',i64);
               i64:=0;
               Chain.Store('LASTON',i64);
               i32:=1;
               Chain.Store('SECLEVEL',i32);
               Result:=True;
               Exit;
            end;
            FastWriteln('|4EIncorrect password.'); //+#32+SHA1(Ts));
            Inc(Attempts);
         end;
      end;
   End;
   Result:=False;
End;
