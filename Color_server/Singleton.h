#pragma once

template <class T>
class CSingleTon
{
protected:
	static T* m_pInst;

public:
	static inline T* GetInst()
	{
		if (!m_pInst) m_pInst = new T;
		return m_pInst;
	}
	static void DestoryInst()
	{
		if (m_pInst){
			delete m_pInst;
			m_pInst = NULL;
		}
		
	}
protected:
	CSingleTon() {}
	~CSingleTon() {}
};

template <class T>
T* CSingleTon<T>::m_pInst = NULL;