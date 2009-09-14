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
#include "SpellCheckSettingsPanel.h"

#include "SpellCheckerConfig.h"
#include <configmanager.h>
#include <logmanager.h>

#include <wx/dir.h>

#include <map>
#include <vector>

//(*InternalHeaders(SpellCheckSettingsPanel)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(SpellCheckSettingsPanel)
//*)

BEGIN_EVENT_TABLE(SpellCheckSettingsPanel,wxPanel)
	//(*EventTable(SpellCheckSettingsPanel)
	//*)
END_EVENT_TABLE()

SpellCheckSettingsPanel::SpellCheckSettingsPanel(wxWindow* parent, SpellCheckerConfig *cfg)
:m_sccfg(cfg)
{
	//(*Initialize(SpellCheckSettingsPanel)
	wxXmlResource::Get()->LoadObject(this,parent,_T("SpellCheckSettingsPanel"),_T("wxPanel"));
	m_checkEnableOnlineSpellChecker = (wxCheckBox*)FindWindow(XRCID("ID_CHECKBOX1"));
	StaticText3 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT3"));
	m_choiceDictionary = (wxChoice*)FindWindow(XRCID("ID_CHOICE3"));

	Connect(XRCID("ID_CHOICE3"),wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&SpellCheckSettingsPanel::OnDictionarySelect);
	//*)
    m_checkEnableOnlineSpellChecker->SetValue( m_sccfg->GetEnableOnlineChecker() );

	InitDictionaryChoice();
}

SpellCheckSettingsPanel::~SpellCheckSettingsPanel()
{
	//(*Destroy(SpellCheckSettingsPanel)
	//*)
}


void SpellCheckSettingsPanel::InitDictionaryChoice()
{
    m_sccfg->ScanForDictionaries();
    std::vector<wxString> dics = m_sccfg->GetPossibleDictionaries();
    int sel = m_sccfg->GetSelectedDictionaryNumber();

    m_choiceDictionary->Clear();
    for ( unsigned int i = 0 ; i < dics.size(); i++ )
        m_choiceDictionary->AppendString(dics[i]);

    if ( sel != -1 )
        m_choiceDictionary->Select(sel);

}
wxString SpellCheckSettingsPanel::GetTitle() const {return _T("SpellChecker");}

wxString SpellCheckSettingsPanel::GetBitmapBaseName() const {return _T("SpellChecker");}

void SpellCheckSettingsPanel::PostConfig()
{
    m_sccfg->SetEnableOnlineChecker(m_checkEnableOnlineSpellChecker->GetValue());
    wxString dic = m_choiceDictionary->GetStringSelection();
    if ( !dic.IsEmpty() )
    {
        m_sccfg->SetDictionaryName( dic );
    }
}

void SpellCheckSettingsPanel::OnApply()
{
    PostConfig(); // to update the configuration object
    m_sccfg->Save(); // save it
}

void SpellCheckSettingsPanel::OnCancel()
{
    //wxMessageBox(_T("de haut nid"));
}


void SpellCheckSettingsPanel::OnDictionarySelect(wxCommandEvent& event)
{

}
