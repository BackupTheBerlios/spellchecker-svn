#include "OnlineSpellChecker.h"

#include <sdk.h> // Code::Blocks SDK
#include <editorcolourset.h>
#include <cbstyledtextctrl.h>
#include "SpellCheckEngineInterface.h"
#include "SpellCheckHelper.h"
//#include <hunspell.hxx>




OnlineSpellChecker::OnlineSpellChecker(wxSpellCheckEngineInterface *pSpellChecker, SpellCheckHelper *pSpellHelper):
//OnlineSpellChecker::OnlineSpellChecker():
    //EditorHooks::HookFunctorBase
    HookFunctorBase(),
    alreadychecked(false),
    oldctrl(NULL),
    m_pSpellChecker(pSpellChecker),
    m_pSpellHelper(pSpellHelper),
    m_doChecks(false)
{
}
OnlineSpellChecker::~OnlineSpellChecker()
{
}


void OnlineSpellChecker::Call(cbEditor* ctrl, wxScintillaEvent &event) const
{
    // return if this event is not fired from the active editor (is it possible that an editor which is not active fires an event?)
    if ( Manager::Get()->GetEditorManager()->GetActiveEditor() != ctrl  ) return;

    // check the event type if it is an update event
    if ( event.GetEventType() == wxEVT_SCI_UPDATEUI )
        OnEditorUpdateUI(ctrl);
    else if ( event.GetEventType() == wxEVT_SCI_CHARADDED )
        OnEditorChange(ctrl);
    else if ( event.GetEventType() == wxEVT_SCI_CHANGE  )
        OnEditorChange(ctrl);
}

void OnlineSpellChecker::OnEditorChange(cbEditor* ctrl) const
{
    // clear internal states to force a refresh at next UpdateUI;
    alreadychecked = false;
}

const int OnlineSpellChecker::GetIndicator()const
{
    const int theIndicator = 11;
    return theIndicator;
}
const wxColor OnlineSpellChecker::GetIndicatorColor()const
{
    //wxColour indicatorColour(cfg->ReadColour(_T("/???/colour"), wxColour(255, 0, 0)));
    return wxColour(255,0,0);
}
void OnlineSpellChecker::OnEditorUpdateUI(cbEditor* ctrl) const
{
    if ( !m_doChecks ) return;
    DoSetIndications(ctrl);
}

void OnlineSpellChecker::DoSetIndications(cbEditor* ctrl)const
{
    cbStyledTextCtrl *stc = ctrl->GetLeftSplitViewControl();
    //Returns a pointer to the left (or top) split-view cbStyledTextCtrl. This function always returns a valid pointer.
    cbStyledTextCtrl *stcr =ctrl->GetRightSplitViewControl();

    // whatever the current state is, we've already done it once
    if ( alreadychecked && oldctrl == ctrl )
        return;
    alreadychecked = true;
    oldctrl = ctrl;

    // Set Styling:
    stc->SetIndicatorCurrent(GetIndicator());
    stc->IndicatorSetStyle(GetIndicator(), wxSCI_INDIC_SQUIGGLE);
    stc->IndicatorSetForeground(GetIndicator(), GetIndicatorColor() );
    if ( stcr )
    {
        stcr->SetIndicatorCurrent(GetIndicator());
        stcr->IndicatorSetStyle(GetIndicator(), wxSCI_INDIC_SQUIGGLE);
        stcr->IndicatorSetForeground(GetIndicator(), GetIndicatorColor() );
    }

    // clear all style indications set in a previous run:
    ClearAllIndications(stc);

    int wordstart = 0;
    int wordend = wordstart;
    for( int pos = wordstart ;  pos < stc->GetLength() ; )
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


void OnlineSpellChecker::EnableOnlineChecks(bool check)
{
    m_doChecks = check;

    alreadychecked = false;

    EditorManager *edm = Manager::Get()->GetEditorManager();
    for ( int i = 0 ; i < edm->GetEditorsCount() ; ++i)
    {
        cbEditor *ed = edm->GetBuiltinEditor( i );

        if ( !ed ) continue;
        if ( check == false )
            // clear all indications set in a previous run
            ClearAllIndications(ed->GetControl());
        else
            OnEditorUpdateUI(ed);
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
