
// DlgProxy.h: ͷ�ļ�
//

#pragma once

class CReadXlsDlg;


// CReadXlsDlgAutoProxy ����Ŀ��

class CReadXlsDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CReadXlsDlgAutoProxy)

	CReadXlsDlgAutoProxy();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��

// ����
public:
	CReadXlsDlg* m_pDialog;

// ����
public:

// ��д
	public:
	virtual void OnFinalRelease();

// ʵ��
protected:
	virtual ~CReadXlsDlgAutoProxy();

	// ���ɵ���Ϣӳ�亯��

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CReadXlsDlgAutoProxy)

	// ���ɵ� OLE ����ӳ�亯��

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

