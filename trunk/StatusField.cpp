#include "StatusField.h"

#include <wx/stattext.h>
#include <wx/statbmp.h>
#include <wx/image.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>

#include <vector>

#include "SpellCheckerConfig.h"
#define LANGS 10
namespace
{
    const int idCommand[LANGS]  = {wxNewId(),wxNewId(),wxNewId(),wxNewId(),wxNewId(),
                                   wxNewId(),wxNewId(),wxNewId(),wxNewId(),wxNewId()};
};

SpellCheckerStatusField::SpellCheckerStatusField(wxWindow* parent, SpellCheckerConfig *sccfg, const wxString &bitmappath)
    :wxPanel(parent, wxID_ANY),
    m_sccfg(sccfg),
    m_bitmappath(bitmappath)
{
    //ctor
    m_text = new wxStaticText(this, wxID_ANY, m_sccfg->GetDictionaryName());

    wxBitmap bm(wxImage( m_bitmappath + wxFILE_SEP_PATH + m_sccfg->GetDictionaryName() + _T(".png"), wxBITMAP_TYPE_PNG ));
    m_bitmap = new wxStaticBitmap(this, wxID_ANY, bm);

    if ( bm.IsOk() )
        m_text->Hide();
    else
        m_bitmap->Hide();

    Connect(wxEVT_SIZE, wxSizeEventHandler(SpellCheckerStatusField::OnSize), NULL, this);
    Connect(idCommand[0],idCommand[LANGS-1], wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(SpellCheckerStatusField::OnSelect), NULL, this);

    m_text->Connect(wxEVT_RIGHT_UP, wxMouseEventHandler(SpellCheckerStatusField::OnRightUp), NULL, this);
    m_bitmap->Connect(wxEVT_RIGHT_UP, wxMouseEventHandler(SpellCheckerStatusField::OnRightUp), NULL, this);
    Connect(wxEVT_RIGHT_UP, wxMouseEventHandler(SpellCheckerStatusField::OnRightUp), NULL, this);
}

SpellCheckerStatusField::~SpellCheckerStatusField()
{
    //dtor
    Disconnect(wxEVT_SIZE, wxSizeEventHandler(SpellCheckerStatusField::OnSize), NULL, this);
    Disconnect(idCommand[0],idCommand[LANGS-1], wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(SpellCheckerStatusField::OnSelect), NULL, this);

    m_text->Disconnect(wxEVT_RIGHT_UP, wxMouseEventHandler(SpellCheckerStatusField::OnRightUp));
    m_bitmap->Disconnect(wxEVT_RIGHT_UP, wxMouseEventHandler(SpellCheckerStatusField::OnRightUp));
    Disconnect(wxEVT_RIGHT_UP, wxMouseEventHandler(SpellCheckerStatusField::OnRightUp));
}
//void SpellCheckerStatusField::SetLanguage(const wxString &language)
void SpellCheckerStatusField::Update()
{
    m_text->SetLabel(m_sccfg->GetDictionaryName() );
    wxBitmap bm(wxImage( m_bitmappath + wxFILE_SEP_PATH + m_sccfg->GetDictionaryName()  + _T(".png"), wxBITMAP_TYPE_PNG ));
    if ( bm.IsOk() )
    {
        m_text->Hide();
        m_bitmap->Hide();
        m_bitmap->SetBitmap(bm);
        m_bitmap->Show();
    }
    else
    {
        m_bitmap->Hide();
        m_text->Show();
    }
    DoSize();
}

void SpellCheckerStatusField::OnSize(wxSizeEvent &event)
{
    DoSize();
}
void SpellCheckerStatusField::DoSize()
{
    //m_bitmap->SetSize(this->GetSize());
    wxSize msize = this->GetSize();
    wxSize bsize = m_bitmap->GetSize();

    m_text->SetSize(msize);
    m_bitmap->Move(msize.x/2 - bsize.x/2, msize.y/2 - bsize.y/2);
}

void SpellCheckerStatusField::OnRightUp(wxMouseEvent &event)
{
    m_sccfg->ScanForDictionaries();
    wxMenu *popup = new wxMenu();
    std::vector<wxString> dicts = m_sccfg->GetPossibleDictionaries();
    for ( int i = 0 ; i < dicts.size()&& i < LANGS ; i++ )
        popup->Append( idCommand[i], dicts[i], _T(""), wxITEM_CHECK)->Check(dicts[i] == m_sccfg->GetDictionaryName() );
    PopupMenu(popup);
    delete popup;
}
void SpellCheckerStatusField::OnSelect(wxCommandEvent &event)
{
    int idx;
    for ( idx = 0 ; idx < LANGS ; idx++)
        if ( event.GetId() == idCommand[idx])
            break;

    std::vector<wxString> dicts = m_sccfg->GetPossibleDictionaries();

    if ( idx < dicts.size() )
    {
        m_sccfg->SetDictionaryName(dicts[idx]);
        m_sccfg->Save(); // save it
    }

}

