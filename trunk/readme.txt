This is a first version of the SpellChecker plugin.

It can be configured (through OnlineSpellChecking.xml file) which lexer-styles have to be spell-checked. Sample config file provides: spell-check only comments in C/C++, VHDL and Verilog and spell-check sources for LaTeX.
It needs dictionaries (from OpenOffice) to work correctly. The current code is looking in ConfigManager::GetDataFolder() + wxFileName::GetPathSeparator() + _T("SpellChecker") for the these files: OnlineSpellChecking.xml, *.aff, *.dic, th_*.idx and th_*.dat.

Hunspell is used for the spell checker and wxSpellChecker (from wxCode) for the GUI. Code::Blocks project files (msw and linux) are provided. (tested on winXP-sp3 and fedora 11)
MyThes is used for the thesaurus. The files th_*.idx and th_*.dat are expected in the same directory. Thesaurus will be disabled if the files for the selected language can not be found.

Currently, hunspell and wxSpellChecker are not in the repository. The MSW project needs the files from "http://downloads.sourceforge.net/hunspell/hunspell-1.2.8.tar.gz.". Both project files need the wxSpellChecker library from wxCode (apply wxspellchecker.patch). This gives the following directory structure:
* SpellChecker
  * hunspell-1.2.8
  * rc
  * wxsmith
  * wxspellchecker
(MyThes is provided in the repo in mythes.?xx)

Dictionaries for hunspell and MyThes can be downloaded from "http://wiki.services.openoffice.org/wiki/Dictionaries" OOo 2.x files. OOo 3.x Extensions (.oxt) are zip'ed archives containing these files.

If the CB_STATUS_BAR is defined and the patch cbStatusBar.patch is applyed to C::B, then the plugin provides a "status-field" to switch between dictionaries. The prject files have a NO_CB_STATUS_BAR define prepared, you have to simply remove the NO_. (seems not to work yet on linux.)

Thanks for any comments/hints/test-reports/questions
