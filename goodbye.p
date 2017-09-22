Program Goodbye.v1170607;

/////////////////////////////////////////////////////////////////////////////
// MODULE ID: GOODBYE
// Description:
/////////////////////////////////////////////////////////////////////////////
uses
   Variants,
   Classes,
   DateTime,    // FormatTimestamp()
   Databases,
   Display,     // Color Names
   Strings,     //
   ANSISockets, // Extends Session to Support ANSI/AVATAR/PIPE/WWIV commands!
   Environment, // Add FILE routines, etc.
   Chains;      // Add Chain Memory and Chaining...

{$i includes/screencrt.i}
{$I includes/bbskit.inc}

begin
   If not Chain.ReadBoolean('HASANSI') then Session.SetASCIIMode;
   If Chain.ReadBoolean('HASANSI') then Session.SetANSIMode;
   If Chain.ReadBoolean('HASUTF8') then Session.SetUTF8Mode;
   //FastWriteln('|0E[|0A'+FormatTimestamp('ddd, mmm dd yyyy hh:nn',Timestamp)+'|0B Server Time, Eastern Time Zone|0E]'+#13#10);
   Session.Prompt('|0E[|7C'+#223+'|0E]|0E Press [|0FENTER|0E] to disconnect.|07',30);
   Session.Writeln('Goodbye.');
   Session.Disconnect;
end.
