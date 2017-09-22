procedure last_caller;
var
   DBF:THalcyonDataset;
   Field:TField;

begin
   If FileExists(ScriptRoot+'data/todaycallers.dbf') then begin
      DBF.Init(Nil);
      DBF.setFilename(ScriptRoot+'data/todaycallers.dbf');
      DBF.Open;
      DBF.Last;
      CLS;
      FastWrite(10,18,15,1,PadRight(' Last Caller Plugin v1.0',58)+'|4F X |57 ');
      FillBox(10,19,72,23,15,2,1);
      Field:=DBF.getFieldByName('USERNAME');
      FastWrite(10,19,14,2,Center('Last Caller',62));
      FastWrite(10,20,13,2,Center(Field.getAsString,62));
      Field:=DBF.getFieldByName('FROM');
      FastWrite(10,21,13,2,Center(Field.getAsString,62));
      FastWrite(10,22,14,2,Center('Local Time',62));
      FastWrite(10,13,10,2,Center(FormatTimestamp('ddd, mmm dd yyyy hh:nn:ss',Timestamp),62));
      CursorTo(1,12);
      DBF.Close;
      DBF.Free;
      Pause;
      CLS(7,0);
   End;
end;
