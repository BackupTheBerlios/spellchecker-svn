This is a first version of the SpellChecker plugin.

It can be configured (through OnlineSpellChecking.xml file) which lexer-styles have to be spell-checked. Sample config file provides: spell-check only comments in C/C++, VHDL and Verilog and spell-check sources for LaTeX.
It needs dictionaries (from OpenOffice) to work correctly. The current code is looking in ConfigManager::GetDataFolder() + wxFileName::GetPathSeparator() + _T("SpellChecker") for the these files: OnlineSpellChecking.xml, *.aff, *.dic, th_*.idx and th_*.dat.

Hunspell is used for the spell checker and wxSpellChecker (from wxCode) for the GUI. Code::Blocks project files (msw and linux) are provided (tested on winXP-sp3 and fedora 11). For linux, hunspell is assumed to be installed as a system library.
MyThes is used for the thesaurus. The files th_*.idx and th_*.dat are expected in the same directory as the dictionaries mentioned above. Thesaurus will be disabled if the files for the selected language can not be found.

Dictionaries for hunspell and MyThes can be downloaded from "http://wiki.services.openoffice.org/wiki/Dictionaries" OOo 2.x files. OOo 3.x Extensions (.oxt) are zip'ed archives containing these files.

If the CB_STATUS_BAR is defined and the patch cbStatusBar.patch is applyed to C::B, then the plugin provides a "status-field" to switch between dictionaries. The prject files have a NO_CB_STATUS_BAR define prepared, you have to simply remove the NO_.

Thanks for any comments/hints/test-reports/questions