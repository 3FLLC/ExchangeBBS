# ExchangeBBS
Xenophobe BBS (that I have been writing since 1983) updated to TCP/IP, multi-node, multi-operating-system. I also aquired the source to QuickBBS 2.80 and 2.81, and I am in the process of Integrating its features - new product ExchangeBBS.

### Features:
* ASCII 7Bit, 8Bit and UTF8
* ANSI 8 Color Background, 16 Color FG/BG, 256 Color FG/BG and True Color FG/BG
* i18b locationation (multiple languages supported - using QuickBBS 2.75/2.76 Language Files)
* Community Edition supports 5 concurrent logins, Pro = 250, Enterprise = 50,000
* Built-in Fidonet BinkP Mailer Support - My mailer is Rhenium (not uploaded yet)
* Built-in FTN Tosser, with rethreading and dbase driving dupe checker
* Built-in active source for XModem, XModem-CRC, 1K-XModem-CRC, WXModem-CRC, YModem, YModemG
  * All protocols have been optimized for 10 gigabit fiber networks
  * some protocols limit performance due to timing, packet sizes, non-windowed ACK/NAK logic
* Designed for download into /BBS/ or C:\BBS\ folder, and launch via ModernPascal.com's CodeRunner2
* Local login is available through telnet terminal, web based telnet terminal, and soon: mp2 welcome.p
* Supports terminal programs that are ANSI X3.64 compliant
* Uses FoxPro .DBF and .CDX for Users, Messagebase and configuration files.
* Uses physical file system for File Area, and CSV file FILES.CSV for Filename, Date, Size, Description
* Customizable ANSI Prompts
* Extensive Display File (see: /includes/bbskit.inc DisplayFileEx()).

### Functions:
* (A)bout Exchange BBS
* (B)BS Listing Database
* (C)hat with other nodes
* (D)oorway Games
* (E)mail Send/Receive
* (F)ile Area Submenu
* (G)oodbye, Sign-Off
* (H)elp with Commands
* (I)nternet Submenu
* (J)oin Network Messaging
* (K)ing of the Hill
* (L)og of Recent Callers
* (M)essage Base Submenu
* (N)ews Postings
* (O)perator Utilities
* (P)rofile Editor (full screen)
* (Q)WK Reader Submenu
* (R)umors Submenu
* (S)witch User ID
* (T)imebank Submenu
* (U)ser Database Submenu
* (V)erify your Account
* (W)ho is online now?
* (X)pert Mode Toggle
* (Y)our Statistics
* (Z)ip Search Engine

## Local Modes (Mailer, Tosser and BBS) run in pretty formatted full screen ANSI mode, or no output Daemon/TService mdoe.
