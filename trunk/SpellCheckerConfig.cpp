/*
* This file is part of SpellChecker plugin for Code::Blocks Studio
* Copyright (C) 2009 Daniel Anselmi
*
* SpellChecker plugin is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* SpellChecker plugin is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with SpellChecker. If not, see <http://www.gnu.org/licenses/>.
*
*/
#include "SpellCheckerConfig.h"
#include "SpellCheckerPlugin.h"
#include "sdk.h" // Code::Blocks SDK
#include "configmanager.h"

#define CFG_SPELLCHECK_ENABLE_ONLINE_CHECK _T("/SpellChecker/EnableOnlineChecker")
#define CFG_SPELLCHECK_DICTIONARY_NAME _T("/SpellChecker/Dictionary")

SpellCheckerConfig::SpellCheckerConfig(SpellCheckerPlugin *plugin)
    ://m_DictionaryPath(plugin->GetDic),
    selectedDictionary(-1),
    m_pPlugin(plugin)
{
    Load();
    ScanForDictionaries();
}

SpellCheckerConfig::~SpellCheckerConfig()
{
    //dtor
}

void SpellCheckerConfig::Reset()
{
    m_EnableOnlineChecker = true;
    m_strDictionaryName   = _T("de_CH");
}

bool SpellCheckerConfig::GetEnableOnlineChecker()
{
    return m_EnableOnlineChecker;
}
void SpellCheckerConfig::SetEnableOnlineChecker(bool val)
{
    m_EnableOnlineChecker = val;
}
wxString SpellCheckerConfig::GetDictionaryName()
{
    return m_strDictionaryName;
}
void SpellCheckerConfig::SetDictionaryName(wxString val)
{
    m_strDictionaryName = val;
}
int SpellCheckerConfig::GetSelectedDictionaryNumber()const
{
    return selectedDictionary;
}

void SpellCheckerConfig::ScanForDictionaries()
{
    m_dictionaries.clear();
    selectedDictionary = -1;
    //wxString filespec(_T("??_??.dic"));
    wxString filespec(_T("*.dic"));

    wxDir dir;
    if ( dir.Open(m_pPlugin->GetDictionaryPath()) )
    {
        wxString strfilename;
        bool cont = dir.GetFirst(&strfilename, filespec, wxDIR_FILES );
        while ( cont )
        {
            wxFileName fname(strfilename);
            wxString afffilename = m_pPlugin->GetDictionaryPath() + wxFileName::GetPathSeparator() + fname.GetName() + _T(".aff");
            if ( wxFileName::FileExists(afffilename) )
            {
                if ( fname.GetName() == m_strDictionaryName )
                    selectedDictionary = m_dictionaries.size();
                m_dictionaries.push_back(fname.GetName());

            }
            cont = dir.GetNext(&strfilename);
        }
    }
}
const std::vector<wxString> &SpellCheckerConfig::GetPossibleDictionaries()const
{
    return m_dictionaries;
}



void SpellCheckerConfig::Load()
{
    Reset();
    if (ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("editor")))
    {
        m_EnableOnlineChecker = cfg->ReadBool(CFG_SPELLCHECK_ENABLE_ONLINE_CHECK, true);
        m_strDictionaryName = cfg->Read(CFG_SPELLCHECK_DICTIONARY_NAME, _T("de_CH") );
    }
}
void SpellCheckerConfig::Save()
{
    if(ConfigManager* cfg = Manager::Get()->GetConfigManager(_T("editor")))
    {
        cfg->Write(CFG_SPELLCHECK_ENABLE_ONLINE_CHECK, m_EnableOnlineChecker);
        cfg->Write(CFG_SPELLCHECK_DICTIONARY_NAME, m_strDictionaryName);
    }
    m_pPlugin->ReloadSettings();
}
