
// ReadXls.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CReadXlsApp: 
// �йش����ʵ�֣������ ReadXls.cpp
//

class CReadXlsApp : public CWinApp
{
public:
	CReadXlsApp();

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CReadXlsApp theApp;