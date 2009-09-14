#include "OnlineSpellChecker.h"

#include <sdk.h> // Code::Blocks SDK
#include <editorcolourset.h>
#include <cbstyledtextctrl.h>
#include "SpellCheckEngineInterface.h"
#include "SpellCheckHelper.h"
//#include <hunspell.hxx>

EditorPos::EditorPos():
    linea(-1),
    lineb(-1),
    ed(NULL)
{}



OnlineSpellChecker::OnlineSpellChecker(wxSpellCheckEngineInterface *pSpellChecker, SpellCheckHelper *pSpellHelper):
//OnlineSpellChecker::OnlineSpellChecker():
    //EditorHooks::HookFunctorBase
    HookFunctorBase(),
    m_pSpellChecker(pSpellChecker),
    m_pSpellHelper(pSpellHelper),
    m_doChecks(false)
{
    old1 = new EditorPos;
    old2 = new EditorPos;
    //LoadConfiguration();
}
OnlineSpellChecker::~OnlineSpellChecker()
{
    delete old1;
    delete old2;
}


void OnlineSpellChecker::Call(cbEditor* ctrl, wxScintillaEvent &event) const
{
    // return if this event is not fired from the active editor (is it possible that an editor which is not active fires an event?)
    if ( Manager::Get()->GetEditorManager()->GetActiveEditor() != ctrl  ) return;

    // check the event type if it is an update event
    if ( event.GetEventType() == wxEVT_SCI_UPDATEUI )
        OnEditorUpdateUI(ctrl);
    else if ( event.GetEventType() == wxEVT_SCI_CHARADDED )
        OnEditorCharAdded(ctrl);
    else if ( event.GetEventType() == wxEVT_SCI_CHANGE  )
        OnEditorCharAdded(ctrl);
}

void OnlineSpellChecker::OnEditorCharAdded(cbEditor* ctrl) const
{
    // to refresh at next UpdateUI;
    if ( ctrl->GetControl() == ctrl->GetLeftSplitViewControl() )
    {
        old1->linea = -1;
        old1->lineb = -1;
    }
    if ( ctrl->GetControl() == ctrl->GetRightSplitViewControl() )
    {
        old2->linea = -1;
        old2->lineb = -1;
    }
}

const int OnlineSpellChecker::GetIndicator()const
{
    const int theIndicator = 11;
    return theIndicator;
}
void OnlineSpellChecker::OnEditorUpdateUI(cbEditor* ctrl) const
{
    if ( !m_doChecks ) return;
    DoSetIndications(ctrl, ctrl->GetLeftSplitViewControl(), old1);
    DoSetIndications(ctrl, ctrl->GetRightSplitViewControl(), old2);
}

void OnlineSpellChecker::DoSetIndications(cbEditor* ctrl, cbStyledTextCtrl* stc, EditorPos *old)const
{
    if ( stc )
    {
        int linea = stc->DocLineFromVisible(stc->GetFirstVisibleLine());
        int lineb = stc->DocLineFromVisible(stc->GetFirstVisibleLine() + stc->LinesOnScreen());

        // whatever the current state is, we've already done it once
        if ( old->linea == linea && old->lineb == lineb && old->ed == ctrl )
            return;
        old->linea = linea;
        old->lineb = lineb;
        old->ed = ctrl;

        stc->SetIndicatorCurrent(GetIndicator());
        // Set Styling:
        stc->IndicatorSetStyle(GetIndicator(), wxSCI_INDIC_SQUIGGLE);
        //wxColour indicatorColour(cfg->ReadColour(_T("/???/colour"), wxColour(255, 0, 0)));
        wxColour indicatorColour(255,0,0);
        stc->IndicatorSetForeground(GetIndicator(), indicatorColour );

        // clear all style indications set in a previous run:
        ClearIndicatorLineRange(stc, linea, lineb);

        int wordstart = stc->PositionFromLine(linea);
        int wordend = wordstart;
        for( int pos = wordstart ;  pos < stc->GetLineEndPosition(lineb) ; )
        {
            wxChar ch = stc->GetCharAt(pos);
            if ( SpellCheckHelper::IsWhiteSpace(ch) )
            {
                wxString lang = Manager::Get()->GetEditorManager()->GetColourSet()->GetLanguageName(ctrl->GetLanguage() );
                if ( (wordstart != wordend) && m_pSpellHelper->HasStyleToBeChecked(lang, stc->GetStyleAt(pos))  )
                {
                    wxString word = stc->GetTextRange(wordstart, wordend);
                    if ( !m_pSpellChecker->IsWordInDictionary(word) )
                        stc->IndicatorFillRange(wordstart, wordend-wordstart);
                }
                pos++;
                wordstart = pos;
                wordend = pos;
            }
            else
            {
                pos++;
                wordend = pos;
            }
        }
    }
}


void OnlineSpellChecker::EnableOnlineChecks(bool check)
{
    m_doChecks = check;

    old1->linea = -1;
    old1->lineb = -1;
    old2->linea = -1;
    old2->lineb = -1;

    EditorManager *edm = Manager::Get()->GetEditorManager();
    for ( int i = 0 ; i < edm->GetEditorsCount() ; ++i)
    {
        cbEditor *ed = edm->GetBuiltinEditor( i );

        if ( !ed ) continue;
        if ( check == false )
        {
            // clear all style indications set in a previous run
            ClearAllIndications(ed->GetLeftSplitViewControl());
            ClearAllIndications(ed->GetRightSplitViewControl());
        }
        else
        {
            OnEditorUpdateUI(ed);
        }
    }
}

void OnlineSpellChecker::ClearAllIndications(cbStyledTextCtrl* stc)const
{
    if ( stc )
    {
        stc->SetIndicatorCurrent(GetIndicator());
        stc->IndicatorClearRange(0, stc->GetLength());
    }
}

void OnlineSpellChecker::ClearIndicatorLineRange(cbStyledTextCtrl* stc, int linestart, int linestop)const
{
    int start = stc->PositionFromLine(linestart);
    int stop = stc->GetLineEndPosition(linestop);

    stc->SetIndicatorCurrent(GetIndicator());
    stc->IndicatorClearRange(start, stop-start);
}
