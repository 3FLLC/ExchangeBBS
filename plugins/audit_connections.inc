procedure audit_connection;
begin
{$IFDEF CODERUNNER}
   FastWrite(10,6,15,5,PadRight(' Audit Connection Plugin v1.0',58)+'|4F X |57 ');
   FastWrite(10,7,0,6,PadRight(' Logged connection From: |6E'+Session.GetPeerIPAddress+'|60',68));
   If (Session.GetPeerIPAddress="162.211.65.233") then
      FastWrite(10,8,0,6,PadRight(' Reverse DNS on your IP: |6Ewww.3FDirectory.com - BBS Terminal|60',68))
   Else
      FastWrite(10,8,0,6,PadRight(' Reverse DNS on your IP: |6E'+Session.GetHostByIPAddress(Session.GetPeerIPAddress,1)+'|60',68));
{$ENDIF}
   CursorTo(1,11);
end;
