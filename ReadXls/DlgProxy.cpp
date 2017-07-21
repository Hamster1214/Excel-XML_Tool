
// DlgProxy.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ReadXls.h"
#include "DlgProxy.h"
#include "ReadXlsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CReadXlsDlgAutoProxy

IMPLEMENT_DYNCREATE(CReadXlsDlgAutoProxy, CCmdTarget)

CReadXlsDlgAutoProxy::CReadXlsDlgAutoProxy()
{
	EnableAutomation();
	
	// ΪʹӦ�ó������Զ��������ڻ״̬ʱһֱ���� 
	//	���У����캯������ AfxOleLockApp��
	AfxOleLockApp();

	// ͨ��Ӧ�ó����������ָ��
	//  �����ʶԻ���  ���ô�����ڲ�ָ��
	//  ָ��Ի��򣬲����öԻ���ĺ���ָ��ָ��
	//  �ô���
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CReadXlsDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CReadXlsDlg)))
		{
			m_pDialog = reinterpret_cast<CReadXlsDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CReadXlsDlgAutoProxy::~CReadXlsDlgAutoProxy()
{
	// Ϊ������ OLE �Զ����������ж������ֹӦ�ó���
	//	������������ AfxOleUnlockApp��
	//  ���������������⣬�⻹���������Ի���
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CReadXlsDlgAutoProxy::OnFinalRelease()
{
	// �ͷ��˶��Զ�����������һ�����ú󣬽�����
	// OnFinalRelease��  ���ཫ�Զ�
	// ɾ���ö���  �ڵ��øû���֮ǰ�����������
	// ��������ĸ���������롣

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CReadXlsDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CReadXlsDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// ע��: ��������˶� IID_IReadXls ��֧��
//  ��֧������ VBA �����Ͱ�ȫ�󶨡�  �� IID ����ͬ���ӵ� .IDL �ļ��е�
//  ���Ƚӿڵ� GUID ƥ�䡣

// {6B3EFF69-A4AD-4306-BD9C-D7863A4202B2}
static const IID IID_IReadXls =
{ 0x6B3EFF69, 0xA4AD, 0x4306, { 0xBD, 0x9C, 0xD7, 0x86, 0x3A, 0x42, 0x2, 0xB2 } };

BEGIN_INTERFACE_MAP(CReadXlsDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CReadXlsDlgAutoProxy, IID_IReadXls, Dispatch)
END_INTERFACE_MAP()

// IMPLEMENT_OLECREATE2 ���ڴ���Ŀ�� StdAfx.h �ж���
// {174D6E32-4900-420B-9006-1C6D299D4ABC}
IMPLEMENT_OLECREATE2(CReadXlsDlgAutoProxy, "ReadXls.Application", 0x174d6e32, 0x4900, 0x420b, 0x90, 0x6, 0x1c, 0x6d, 0x29, 0x9d, 0x4a, 0xbc)


// CReadXlsDlgAutoProxy ��Ϣ�������
