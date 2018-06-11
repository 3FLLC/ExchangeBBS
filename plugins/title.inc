procedure Title(bg:byte=0);
begin
   CLS(7,bg);
   FastWrite(1,1,4,7,'|4E                    |74');
   ClearLine(21,1,0,7);
   ClearLine(21,2,0,7);
   FastWrite(1,2,4,7,'|4E ExchangeBBS v3.1   |74 Linux 64bit, Open Source (github)       |70Author: |71Ozz Nixon |07');
   FastWrite(1,3,4,7,'|4E                    |74');
   ClearLine(21,3,0,7);
   NewLine;
   NewLine;
End;
