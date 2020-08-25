#pragma once
#include "Object.h"

#include "Define.h"
#include "State.h"

void CObject::ChangeState(CState *_pState)
{
	if (!_pState)
	{
		cout << m_num << " Obj ChangeState Failed" << endl;
		return;
	}
	if (m_pState) m_pState->End(this);
	m_pState = _pState;
	m_pState->Begin(this);
};