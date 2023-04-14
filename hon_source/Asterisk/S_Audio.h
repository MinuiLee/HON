#ifndef S_AUDIO_H
#define S_AUDIO_H
/*!*******************************************************************
\headerfile S_Audio.h <>
\author     Kim HyungSeob
\par        email: hn02415 \@ gmail.com
\brief
	    Header file for Audio System Class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include "S_Base.h"

/*!*******************************************************************
\class S_Audio
\brief
       Play sounds depends on information given by observer or
       included in Audio Component.
********************************************************************/
class S_Audio : public S_Base
{
public:
    /*!*******************************************************************
    \brief 
	   Constructor for Audio System Class.
           Set requirement as Audio Comopnent.
           Set Attack message to handle.

    \param systemMgr
	   SystemManager where register this system.
    ********************************************************************/
    S_Audio(SystemManager * systemMgr);

    /*!*******************************************************************
    \brief Update method that go through the list of the sounds and
           play the sound that marked on.
    ********************************************************************/
    void Update(Window *, float, bool editor_mode) override;

    /*!*******************************************************************
    \brief 
           Observer method that takes message of action and play specific
           sound for each message.

    \param message
           Message about action to handle.
    ********************************************************************/
    void Notify(Message & message) override;
};

#endif
