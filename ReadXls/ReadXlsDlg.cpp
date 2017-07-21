
// ReadXlsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ReadXls.h"
#include "ReadXlsDlg.h"
#include "DlgProxy.h"
#include "afxdialogex.h"
#include "tinystr.h"
#include "tinyxml.h"
#include <windows.h>
#include "iconv.h"

#pragma comment(lib, "iconv")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define set_CRT_NON_CONFORMING_SWPRINTFS


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

int code_convert(char *from_charset,char *to_charset,const char *inbuf,unsigned int inlen,char *outbuf,unsigned int outlen)
{
	iconv_t cd;
	const char **pin = &inbuf;
	char **pout = &outbuf;
	cd = iconv_open(to_charset,from_charset);
	if (cd==0) return -1;
	memset(outbuf,0,outlen);
	if (iconv(cd,pin,&inlen,pout,&outlen)==-1) return -1;
	iconv_close(cd);
	return 0;
}



class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CReadXlsDlg �Ի���


IMPLEMENT_DYNAMIC(CReadXlsDlg, CDialogEx);

CReadXlsDlg::CReadXlsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CReadXlsDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;
}

CReadXlsDlg::~CReadXlsDlg()
{
	// ����öԻ������Զ���������
	//  ���˴���ָ��öԻ���ĺ���ָ������Ϊ NULL���Ա�
	//  �˴���֪���öԻ����ѱ�ɾ����
	if (m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;

}

void CReadXlsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Exit, m_Exit);
	DDX_Control(pDX, IDC_ToXml, m_btnToXml);
	DDX_Control(pDX, IDC_OpenFile, m_btnOpenFile);
	DDX_Control(pDX, IDC_SavePath, m_btnSavePath);
	DDX_Control(pDX, IDC_InputPath, m_editInputPath);
	DDX_Control(pDX, IDC_OutputPath, m_editOutputPath);
}

BEGIN_MESSAGE_MAP(CReadXlsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ToXml, &CReadXlsDlg::OnBnClickedToxml)
//	ON_EN_CHANGE(IDC_EDIT1, &CReadXlsDlg::OnEnChangeEdit1)
//ON_EN_CHANGE(IDC_EDIT1, &CReadXlsDlg::OnEnChangeEdit1)
ON_BN_CLICKED(IDC_Exit, &CReadXlsDlg::OnBnClickedExit)
ON_BN_CLICKED(IDC_OpenFile, &CReadXlsDlg::OnBnClickedOpenfile)
ON_BN_CLICKED(IDC_SavePath, &CReadXlsDlg::OnBnClickedSavepath)
ON_BN_CLICKED(IDC_ToXml2, &CReadXlsDlg::OnBnClickedToxml2)
ON_BN_CLICKED(IDC_BUTTON3, &CReadXlsDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CReadXlsDlg ��Ϣ�������

BOOL CReadXlsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_SHOW);


	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	if (!m_obExcel.initExcel())
	{
		return false;
	}
	m_oldLocale = _strdup(setlocale(LC_CTYPE, NULL));
	setlocale(LC_CTYPE, "chs");//�趨
	m_strInputName = "";

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CReadXlsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CReadXlsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CReadXlsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ���û��ر� UI ʱ������������Ա���������ĳ��
//  �������Զ�����������Ӧ�˳���  ��Щ
//  ��Ϣ�������ȷ����������: �����������ʹ�ã�
//  ������ UI�������ڹرնԻ���ʱ��
//  �Ի�����Ȼ�ᱣ�������

void CReadXlsDlg::OnClose()
{
	if (CanExit())
	{
		m_obExcel.release();
		//��ԭ�����趨
		setlocale(LC_CTYPE, m_oldLocale);
		free(m_oldLocale);
		CDialogEx::OnClose();
	}
}

void CReadXlsDlg::OnOK()
{
	if (CanExit())
		CDialogEx::OnOK();
}

void CReadXlsDlg::OnCancel()
{
	if (CanExit())
		CDialogEx::OnCancel();
}

BOOL CReadXlsDlg::CanExit()
{
	// �����������Ա�����������Զ���
	//  �������Իᱣ�ִ�Ӧ�ó���
	//  ʹ�Ի���������������� UI ����������
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}

void CReadXlsDlg::readExcelAsData(CString& strInputFile,CString& strOutputPath)
{
	m_editInputPath.GetWindowTextW(strInputFile);
	m_editOutputPath.GetWindowTextW(strOutputPath);
	m_mapSheetList.erase(m_mapSheetList.begin(), m_mapSheetList.end()); //ע��ʹ��mapǰ����������
	m_vecSheetName.clear();

	if (0 == strInputFile.GetLength())
	{
		MessageBox(_T("����ѡ����Ҫ��ȡ���ļ�"), _T("����"), MB_OK);
		return;
	}
	if (0 == strOutputPath.GetLength())
	{
		MessageBox(_T("����ѡ�����·��"), _T("����"), MB_OK);
		return;
	}
	
	if (!m_obExcel.open(__CString2Constchar(strInputFile)))
	{
		MessageBox(_T("�޷��򿪸��ļ�"), _T("����"), MB_OK);
		return;
	}
	int nSheetCount = m_obExcel.getSheetCount();



	//������excel����m_mapSheetList��
	for (int s = 1; s <= nSheetCount; ++s)
	{
		CString strSheetName = __CStringConvertAnsiToUtf8(m_obExcel.getSheetName(s));//��ȡsheet��
		m_vecSheetName.push_back(strSheetName);
		bool bLoad = m_obExcel.loadSheet(strSheetName);//װ��sheet  
		int nRow = m_obExcel.getRowCount();//��ȡsheet������
		int nCol = m_obExcel.getColumnCount();//��ȡsheet������  


		CString cell;
		vector<vector<CString>> vecSheet;
		for (int i = 1; i <= nRow; ++i)
		{
			vector<CString> vecRow;
			for (int j = 1; j <= nCol; ++j)
			{
				cell = __CStringConvertAnsiToUtf8(m_obExcel.getCellString(i, j));
				vecRow.push_back(cell);
			}
			vecSheet.push_back(vecRow);
		}
		m_mapSheetList[strSheetName] = vecSheet;
	}
}



void CReadXlsDlg::readExcelAsData3(CString& strInputFile,CString& strOutputPath)
{
	m_editInputPath.GetWindowTextW(strInputFile);
	m_editOutputPath.GetWindowTextW(strOutputPath);
	m_mapSheetList.erase(m_mapSheetList.begin(), m_mapSheetList.end()); 
	m_XlsToXml_mapSheetList.erase(m_XlsToXml_mapSheetList.begin(), m_XlsToXml_mapSheetList.end());
	m_vecSheetName.clear();
	//strInputFile = "C:\\Users\\linzhaolun.allen\\Desktop\\XMLtest\\maplist.xls";
	//strOutputPath = "C:\\Users\\linzhaolun.allen\\Desktop\\XMLtest\\maplist.xml";
	if (0 == strInputFile.GetLength())
	{
		MessageBox(_T("����ѡ����Ҫ��ȡ���ļ�"), _T("����"), MB_OK);
		return;
	}
	if (0 == strOutputPath.GetLength())
	{
		MessageBox(_T("����ѡ�����·��"), _T("����"), MB_OK);
		return;
	}
	
	if (!m_obExcel.open(__CString2Constchar(strInputFile)))
	{
		MessageBox(_T("�޷��򿪸��ļ�"), _T("����"), MB_OK);
		return;
	}

	int nSheetCount = m_obExcel.getSheetCount();

	CExcelSheet ASheet;


	for (int s = 1; s <= nSheetCount; ++s)
	{
		ASheet.strSheetName = __CStringConvertAnsiToUtf8(m_obExcel.getSheetName(s));
		m_vecSheetName.push_back(ASheet.strSheetName);
		bool bLoad = m_obExcel.loadSheet(ASheet.strSheetName);
		int nRow = m_obExcel.getRowCount();
		int nCol = m_obExcel.getColumnCount();


		CString cell;
	
		for (int i = 1; i <= nRow; ++i)
		{
			vector<CString> vecRow;
			for (int j = 1; j <= nCol; ++j)
			{
				cell = __CStringConvertAnsiToUtf8(m_obExcel.getCellString(i, j));
				vecRow.push_back(cell);
			}
			ASheet.vecSheet.push_back(vecRow);
		}
		m_XlsToXml_mapSheetList[ASheet.strSheetName] = ASheet;
		m_mapSheetList[ASheet.strSheetName] = ASheet.vecSheet;
		ASheet.vecSheet.clear();
	}
}

void CReadXlsDlg::readExcelAsData4(CString& strInputFile, CString& strOutputPath)
{
	m_editInputPath.GetWindowTextW(strInputFile);
	m_editOutputPath.GetWindowTextW(strOutputPath);
	m_mapSheetList.erase(m_mapSheetList.begin(), m_mapSheetList.end());
	m_XlsToXml_mapSheetList.erase(m_XlsToXml_mapSheetList.begin(), m_XlsToXml_mapSheetList.end());
	m_vecSheetName.clear();
	//strInputFile = "C:\\Users\\linzhaolun.allen\\Desktop\\XMLtest\\maplist.xls";
	//strOutputPath = "C:\\Users\\linzhaolun.allen\\Desktop\\XMLtest\\maplist.xml";
	if (0 == strInputFile.GetLength())
	{
		MessageBox(_T("����ѡ����Ҫ��ȡ���ļ�"), _T("����"), MB_OK);
		return;
	}
	if (0 == strOutputPath.GetLength())
	{
		MessageBox(_T("����ѡ�����·��"), _T("����"), MB_OK);
		return;
	}

	if (!m_obExcel.open(__CString2Constchar(strInputFile)))
	{
		MessageBox(_T("�޷��򿪸��ļ�"), _T("����"), MB_OK);
		return;
	}

	int nSheetCount = m_obExcel.getSheetCount();

	CCharSheet ASheet;


	for (int s = 1; s <= nSheetCount; ++s)
	{
		__CString2ConstcharWithAnsiToUtf8(m_obExcel.getSheetName(s), ASheet.strSheetName);
		m_vecCharSheetName.push_back(ASheet.strSheetName);
		bool bLoad = m_obExcel.loadSheet(__Constchar2CString(ASheet.strSheetName));
		int nRow = m_obExcel.getRowCount();
		int nCol = m_obExcel.getColumnCount();


		char* cell = new char[40];

		for (int i = 1; i <= nRow; ++i)
		{
			vector<char *> vecRow;
			for (int j = 1; j <= nCol; ++j)
			{
				__CString2ConstcharWithAnsiToUtf8(m_obExcel.getCellString(i, j), cell);
				
				vecRow.push_back(cell);
			}
			ASheet.vecSheet.push_back(vecRow);
		}
		m_XlsToXml_mapCharSheetList[ASheet.strSheetName] = ASheet;
		m_mapCharSheetList[ASheet.strSheetName] = ASheet.vecSheet;
		ASheet.vecSheet.clear();
	}
}
void CReadXlsDlg::OnBnClickedExit()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	SendMessage(WM_CLOSE, 0, 0);
}


void CReadXlsDlg::OnBnClickedOpenfile()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	// ���ù�����   
	TCHAR szFilter[] = _T("xml�ļ�(*.xml)|*.xml|�����ļ�(*.*)|*.*|Microsoft Excel 2007 ������(*.xlsx)|*.xlsx|�����ļ�(*.*)|*.*||");
	// ������ļ��Ի���   
	CFileDialog fileDlg(TRUE, /*_T("xls")*/NULL, NULL, 0, szFilter, this);
	CString strFilePath;

	// ��ʾ���ļ��Ի���   
	if (IDOK == fileDlg.DoModal())
	{
		// ���������ļ��Ի����ϵġ��򿪡���ť����ѡ����ļ�·����ʾ���༭����   
		strFilePath = fileDlg.GetPathName();
		SetDlgItemText(IDC_InputPath, strFilePath);

		m_strInputName = __GetFileName(strFilePath);
	}
}


void CReadXlsDlg::OnBnClickedSavepath()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	// ���ù�����   
	TCHAR szFilter[] = _T("xml�ļ�(*.xml)|*.xml|�����ļ�(*.*)|*.*|Microsoft Excel 2007 ������(*.xlsx)|*.xlsx|�����ļ�(*.*)|*.*||");
	//Ĭ������ļ���
	CString strDefName;
	if (0 == m_strInputName.GetLength())
	{
		strDefName = "my";
	}
	else
	{
		strDefName = m_strInputName;
	}

	// ���챣���ļ��Ի���   
	CFileDialog fileDlg(FALSE, _T("xml"), strDefName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString strFilePath;

	// ��ʾ�����ļ��Ի���   
	if (IDOK == fileDlg.DoModal())
	{
		// ���������ļ��Ի����ϵġ����桱��ť����ѡ����ļ�·����ʾ���༭����   
		strFilePath = fileDlg.GetPathName();
		SetDlgItemText(IDC_OutputPath, strFilePath);
	}
}




char* CReadXlsDlg::Unicode2Utf8(const char* unicode)
{
	int len;
	len = WideCharToMultiByte(CP_UTF8, 0, (const wchar_t*)unicode, -1, NULL, 0, NULL, NULL);
	char *szUtf8 = (char*)malloc(len + 1);
	memset(szUtf8, 0, len  + 1);
	WideCharToMultiByte(CP_UTF8, 0, (const wchar_t*)unicode, -1, szUtf8, len, NULL, NULL);
	return szUtf8;
}

char* CReadXlsDlg::Unicode2Ansi(const char* AnsiStr)
{
	int len;
	len = WideCharToMultiByte(CP_ACP, 0, (const wchar_t*)AnsiStr, -1, NULL, 0, NULL, NULL);
	char *szAnsi = (char*)malloc(len + 1);
	memset(szAnsi, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, (const wchar_t*)AnsiStr, -1, szAnsi, len, NULL, NULL);
	return szAnsi;
}

char* CReadXlsDlg::Ansi2Unicode(const char* str)
{
	int dwUnicodeLen = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	if (!dwUnicodeLen)
	{
		return _strdup(str);
	}
	size_t num = dwUnicodeLen*sizeof(wchar_t);
	wchar_t *pwText = (wchar_t*)malloc(num + 1);
	memset(pwText, 0, num + 1);
	//memset(pwText, 0, num*2 + 2);
	MultiByteToWideChar(CP_ACP, 0, str, -1, pwText, dwUnicodeLen);
	return (char*)pwText;
}

char* CReadXlsDlg::Utf82Unicode(const char* str)
{
	int dwUnicodeLen = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	if (!dwUnicodeLen)
	{
		return _strdup(str);
	}
	size_t num = dwUnicodeLen*sizeof(wchar_t);
	wchar_t *pwText = (wchar_t*)malloc(num +1);
	memset(pwText, 0, num +1);
	MultiByteToWideChar(CP_UTF8, 0, str, -1, pwText, dwUnicodeLen);
	return (char*)pwText;
}



char* CReadXlsDlg::ConvertAnsiToUtf8(const char* str)
{
	char* unicode = Ansi2Unicode(str);
    char* utf8 = Unicode2Utf8(unicode);
	free(unicode);
	return utf8;
	//return unicode;
}


char* CReadXlsDlg::ConvertUtf8ToAnsi(const char* str)
{
	char* unicode = Utf82Unicode(str);
	char* Ansi = Unicode2Ansi(unicode);
	free(unicode);
	return Ansi;
}



CString CReadXlsDlg::__CStringConvertAnsiToUtf8(CString& strSrc)
{

	CString tempStr;
	 char* tempchar = __CString2Constchar(strSrc);
	 size_t inlen=200;
	 char *outbuf=new char[200];
	 size_t outlen=200;
	 code_convert("GBK","utf-8",tempchar,inlen,outbuf,outlen);
	 tempStr = __Constchar2CString(outbuf);
	 return tempStr;
}

void CReadXlsDlg::__CString2ConstcharWithAnsiToUtf8(CString& strSrc, char * outbuf)
{

	CString tempStr;
	char* tempchar = __CString2Constchar(strSrc);
	size_t inlen = 300;
	size_t outlen = 300;
	code_convert("GBK", "utf-8", tempchar, inlen, outbuf, outlen);

}

char* CReadXlsDlg::__CString2Constchar(CString& strSrc)
{
	char* pszRet;
	const size_t strsize = (strSrc.GetLength() + 1) * 2;
	pszRet = new char[strsize];
	size_t sz = 0;
	wcstombs_s(&sz, pszRet, strsize, strSrc, _TRUNCATE);
	int n = atoi((const char*)pszRet);
	return pszRet;
}



CString  CReadXlsDlg::__Constchar2CString(const char* strSrc)
{
	CString  strDes;
	strDes = strSrc;
	return strDes;
}

void CReadXlsDlg::__CreateXLsFile(CString& strPath)
{



}



void CReadXlsDlg::__CreateXmlFile(CString& strPath)
{
	//http://www.jizhuomi.com/software/497.html
	CStdioFile obFile;
	if (obFile.Open(strPath, CStdioFile::modeCreate | CStdioFile::modeWrite))
	{
		CString strHead = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
		obFile.WriteString(strHead);

		CString strRoot = _T("<") + m_vecSheetName[0] + _T("List>\n");
		CString strFRoot = _T("</") + m_vecSheetName[0] + _T("List>\n");
		obFile.WriteString(strRoot);
		vector<vector<CString>> vecMark = m_mapSheetList[m_vecSheetName[0]];
		//size_t nMarkLen = vecMark.size();
		for (size_t i = 2; i < vecMark.size(); ++i)  //map���Ǹ�sheet��Ĵӵ����п�ʼ�ĵ�ͼ��Ϊroot
		{
			CString strMark = _T("\t<") + m_vecSheetName[0];
			for (size_t j = 0; j < vecMark[i].size(); ++j)
			{
				strMark += _T(" ") + vecMark[0][j] + _T("=\"") + vecMark[i][j] + _T("\"");
			}
			strMark += _T(">\n");
			obFile.WriteString(strMark);
			for (size_t childID = 1; childID < m_vecSheetName.size(); ++childID)
			{
				__WriteItem(obFile, childID, vecMark[i][0]);
			}
			obFile.WriteString(_T("\t</") + m_vecSheetName[0] + _T(">\n"));
		}

		obFile.WriteString(strFRoot);

		obFile.Close();
		MessageBox(_T("���ɳɹ�"), _T("�ɹ�"), MB_OK);
	}
	else
	{
		MessageBox(_T("����ʧ��"), _T("ʧ��"), MB_OK);
	}
}

bool CReadXlsDlg::isSummaryTableMode()
{
	
	for (int i = 1; i < m_vecSheetName.size(); i++)
	{
		CExcelSheet ASheet = m_XlsToXml_mapSheetList[m_vecSheetName[i]];
		if (ASheet.vecSheet[0][0] == m_XlsToXml_mapSheetList[m_vecSheetName[0]].vecSheet[0][0])
			return false;	
	}
	return true;
}


/*�ֱ�+�ϱ�*/
void CReadXlsDlg::__CreateXmlFile3(CString& strPath)
{
	//http://www.jizhuomi.com/software/497.html
	CStdioFile obFile;
	if (m_XlsToXml_mapSheetList.size() > 1 && !isSummaryTableMode())
	{

		CString strFirstTag = __CStringConvertAnsiToUtf8(__GetFileName(strPath));
		if (obFile.Open(strPath, CStdioFile::modeCreate | CStdioFile::modeWrite))
		{
			CString strExcelName = _T("<") + strFirstTag + _T(">\n");
			CString strExcelNameEnd = _T("</") + strFirstTag + _T(">\n");

			CString strHead = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
			
			obFile.WriteString(strHead);
			obFile.WriteString(strExcelName);
			CString strRoot = _T("<") + m_vecSheetName[0] + _T(">\n");
			CString strFRoot = _T("</") + m_vecSheetName[0] + _T(">\n");
			obFile.WriteString(strRoot);
			vector<vector<CString>> vecMark = m_XlsToXml_mapSheetList[m_vecSheetName[0]].vecSheet;
			//size_t nMarkLen = vecMark.size();
			for (size_t i = 2; i < vecMark.size(); ++i)  //map���Ǹ�sheet��Ĵӵ����п�ʼ�ĵ�ͼ��Ϊroot
			{
				CString itemName(m_vecSheetName[0]);
				itemName = itemName.Left(itemName.GetLength() - 4);
				CString strMark = _T("\t<") + itemName;
				for (size_t j = 0; j < vecMark[i].size(); ++j)
				{
					strMark += _T(" ") + vecMark[0][j] + _T("=\"") + vecMark[i][j] + _T("\"");
				}
				strMark += _T(">\n");
				obFile.WriteString(strMark);
				for (size_t childID = 1; childID < m_vecSheetName.size(); ++childID)
				{
					__WriteItem3(obFile, childID, vecMark[i][0], vecMark[0][0]);
				}
				obFile.WriteString(_T("\t</") + itemName + _T(">\n"));
			}

			for (size_t si = 1; si < m_vecSheetName.size(); ++si)
			{
				CExcelSheet ASheet = m_XlsToXml_mapSheetList[m_vecSheetName[si]];
				vector<vector<CString>> vecContentList = ASheet.vecSheet;
				if (!ASheet.BoIsSubTable)
				{
					CString strSheetRoot = _T("\t<") + m_vecSheetName[si] + _T(">\n");
					CString strSheetRootEnd = _T("\t</") + m_vecSheetName[si] + _T(">\n");
					if (vecContentList.size() > 3)
					{
						obFile.WriteString(strSheetRoot);
					}
					for (size_t i = 2; i < vecContentList.size(); ++i)
					{
						CString strItem;
						CString itemName(m_vecSheetName[si]);
						itemName = itemName.Left(itemName.GetLength() - 4);
						if (vecContentList.size() > 3)
						{

							strItem = _T("\t\t<") + itemName;
						}
						else
						{
							strItem = _T("\t<") + itemName;
						}
						for (size_t j = 0; j < vecContentList[i].size(); ++j)
						{
							strItem += _T(" ") + vecContentList[0][j] + _T("=\"") + vecContentList[i][j] + _T("\"");
						}
						strItem += _T("/>\n");
						obFile.WriteString(strItem);
					}
					if (vecContentList.size() > 3)
					{
						obFile.WriteString(strSheetRootEnd);
					}
				}
			}

			obFile.WriteString(strFRoot);
			obFile.WriteString(strExcelNameEnd);

			obFile.Close();
			MessageBox(_T("���ɳɹ�"), _T("�ɹ�"), MB_OK);
		}
		else
		{
			MessageBox(_T("����ʧ��"), _T("ʧ��"), MB_OK);
		}
	}
	else
	{
		//Excelת�ϱ�ģʽXML
		__CreateXmlFile2(strPath);	
	}
}

void CReadXlsDlg::__WriteItem(CStdioFile& obFile, size_t nID, CString strParentID)
{
	CString strChildHead = _T("\t\t");
	strChildHead += _T("<") + m_vecSheetName[nID] + _T("List>\n");
	obFile.WriteString(strChildHead);
	vector<vector<CString>> vecChild = m_mapSheetList[m_vecSheetName[nID]];
	
	for (size_t i = 2; i < vecChild.size(); ++i)
	{
		if (strParentID != vecChild[i][0])
		{
			continue;
		}
		CString strChild = _T("\t\t\t<") + m_vecSheetName[nID];
		for (size_t j = 0; j < vecChild[i].size(); ++j)
		{
			strChild += _T(" ") + vecChild[0][j] + _T("=\"") + vecChild[i][j] + _T("\"");
		}
		strChild += _T("/>\n");
		obFile.WriteString(strChild);
	}

	obFile.WriteString(_T("\t\t</") + m_vecSheetName[nID] + _T("List>\n"));
}

void CReadXlsDlg::__WriteItem3(CStdioFile& obFile, size_t nID, CString strParentID, CString strParentAttri)
{
	CString strChildHead = _T("\t\t");
	strChildHead += _T("<") + m_vecSheetName[nID] + _T(">\n");
	obFile.WriteString(strChildHead);
	CExcelSheet ASheet = m_XlsToXml_mapSheetList[m_vecSheetName[nID]];
	vector<vector<CString>> vecChild = ASheet.vecSheet;
	for (size_t i = 2; i < vecChild.size(); ++i)
	{
		if (strParentID != vecChild[i][0] || strParentAttri != vecChild[0][0])
		{
			continue;
		}
		CString itemName(m_vecSheetName[nID]);
		itemName = itemName.Left(itemName.GetLength() - 4);
		CString strChild = _T("\t\t\t<") + itemName;
		for (size_t j = 1; j < vecChild[i].size(); ++j)
		{
			strChild += _T(" ") + vecChild[0][j] + _T("=\"") + vecChild[i][j] + _T("\"");
			if (!ASheet.BoIsSubTable)
				ASheet.BoIsSubTable = true;
		}
		strChild += _T("/>\n");
		obFile.WriteString(strChild);
	}
	m_XlsToXml_mapSheetList[m_vecSheetName[nID]] = ASheet;

	obFile.WriteString(_T("\t\t</") + m_vecSheetName[nID] + _T(">\n"));

}
void CReadXlsDlg::__CreateXmlFile2(CString& strPath)
{
	CString strFirstTag = __CStringConvertAnsiToUtf8(__GetFileName(strPath));
	CStdioFile obFile;
	if (obFile.Open(strPath, CStdioFile::modeCreate | CStdioFile::modeWrite))
	{
		CString strHead = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
		obFile.WriteString(strHead);
		CString strRoot = _T("<") + strFirstTag + _T(">\n");
		CString strRootEnd = _T("</") + strFirstTag + _T(">\n");
		obFile.WriteString(strRoot);

		for (size_t si = 0; si < m_vecSheetName.size(); ++si)
		{
			int IsSpecialSheet = false;
			vector<vector<CString>> vecContentList = m_mapSheetList[m_vecSheetName[si]];
			CString SheetName = m_vecSheetName[si];
		
			if (SheetName.Find(_T("xxxxx")) != -1)
			{
				SheetName.Delete(0, 5);
				IsSpecialSheet = true;
			}

			CString strSheetRoot = _T("\t<") + SheetName + _T(">\n");
			CString strSheetRootEnd = _T("\t</") + SheetName + _T(">\n");
			if (!IsSpecialSheet)
			{
				obFile.WriteString(strSheetRoot);
			}
			for (size_t i = 2; i < vecContentList.size(); ++i)
			{
				CString strItem;
				CString itemName(SheetName);
				itemName = itemName.Left(itemName.GetLength() - 4);
				if (vecContentList.size() > 3)
				{
					strItem = _T("\t\t<") + itemName;
				}
				else
				{	
					strItem = _T("\t<") + itemName;
				}
				for (size_t j = 0; j < vecContentList[i].size(); ++j)
				{
					strItem += _T(" ") + vecContentList[0][j] + _T("=\"") + vecContentList[i][j] + _T("\"");
				}
				strItem += _T("/>\n");
				obFile.WriteString(strItem);
			}
			if (!IsSpecialSheet)
			{
				obFile.WriteString(strSheetRootEnd);
			}
			//obFile.WriteString(_T("\n"));
		}
		obFile.WriteString(strRootEnd);

		obFile.Close();
		MessageBox(_T("���ɳɹ�"), _T("�ɹ�"), MB_OK);
	}
	else
	{
		MessageBox(_T("����ʧ��"), _T("ʧ��"), MB_OK);
	}
}

CString CReadXlsDlg::__GetFileName(CString strPath)
{
	vector<CString> vecItem;
	int nCurPos = 0;
	while (1)
	{
		CString resToken = strPath.Tokenize(_T("\\"), nCurPos);
		if (resToken.IsEmpty()) break;
		vecItem.push_back(resToken);
	}
	CString strFileName = vecItem[vecItem.size() - 1];
	nCurPos = 0;
	strFileName = strFileName.Tokenize(_T("."), nCurPos);
	return strFileName;
}

/*�ֱ�ģʽ+�ϱ�ģʽ��ExcelתΪXML*/
 void CReadXlsDlg::OnBnClickedToxml()
{
	CString strInputFile;
	CString strOutputPath;
	//readExcelAsData(strInputFile, strOutputPath);
	//readExcelAsData3(strInputFile, strOutputPath);
	readExcelAsData4(strInputFile, strOutputPath);
	
	//��m_mapSheetList����д��xml�ļ�

	//�ֱ�ģʽ+�ϱ�ģʽ
	__CreateXmlFile3(strOutputPath);
}


 /*���´����Ѿ�����*/
void CReadXlsDlg::OnBnClickedToxml2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strInputFile;
	CString strOutputPath;
	readExcelAsData(strInputFile, strOutputPath);
	
	//Excelת�ϱ�ģʽXML
	__CreateXmlFile2(strOutputPath);
}

/*�ϱ�ģʽXMLתExcel*/
/*
void CReadXlsDlg::OnBnClickedButton2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strInputFile;
	CString strOutputPath;
	strInputFile.Empty();
	strOutputPath.Empty();
	m_editInputPath.GetWindowTextW(strInputFile);
	m_editOutputPath.GetWindowTextW(strOutputPath);

	//strInputFile = "C:\\Users\\linzhaolun.allen\\Desktop\\a.xml";
	//strOutputPath = "C:\\Users\\linzhaolun.allen\\Desktop\\b.xls";
	if (0 == strInputFile.GetLength())
	{
		MessageBox(_T("����ѡ����Ҫ��ȡ���ļ�"), _T("����"), MB_OK);
		return;
	}
	if (0 == strOutputPath.GetLength())
	{
		MessageBox(_T("����ѡ�����·��"), _T("����"), MB_OK);
		return;
	}

	vector<vector<CString>> vecSheet;
	CSheetSize SheetSize;
	TiXmlDocument *doc = new TiXmlDocument(__CString2Constchar(strInputFile));
	if (!doc->LoadFile(TIXML_ENCODING_UTF8))  //�ж�XML�ļ��Ƿ���سɹ�
	{
		MessageBox(_T("�޷��򿪸��ļ�"), _T("����"), MB_OK);
		return;
	}

    //doc->Parse(xmlParament,0,TIXML_ENCODING_UTF8);
    //Ŀǰ��֧�ֺϱ�ģʽxml�Ľ���
	int BoHaveTitle = false;
	TiXmlElement* RootLayersNestedElement = doc->RootElement();
	if (NULL == RootLayersNestedElement) //�ж��ļ��Ƿ�������
	{
		MessageBox(_T("û��root element"), _T("����"), MB_OK);
		return;
	}
	//cout << doc->Value() << endl;
	m_stroutputName = RootLayersNestedElement->Value();       //ÿ��xml��root����Ϊ�ļ���
	//cout << m_stroutputName << endl;
	m_mapSheetList.erase(m_mapSheetList.begin(), m_mapSheetList.end()); //ע��ʹ��mapǰ����������
	m_mapSheetSize.erase(m_mapSheetSize.begin(), m_mapSheetSize.end());
	m_vecSheetName.clear();
	TiXmlElement* TwoLayersNestedElement = RootLayersNestedElement->FirstChildElement();   //�ڶ���Ƕ��
	int nSheetCount =0;

	for (; TwoLayersNestedElement != NULL; TwoLayersNestedElement = TwoLayersNestedElement->NextSiblingElement()) 
	{  //ÿһ��ѭ������һ��sheet

		CString strSheetName;
		strSheetName = TwoLayersNestedElement->Value();//��ȡsheet��
		//cout << strSheetName << endl;
		TiXmlElement* ThreeLayersNestedElement = TwoLayersNestedElement->FirstChildElement();  //������Ƕ�� 
		for (; ThreeLayersNestedElement != NULL; ThreeLayersNestedElement = ThreeLayersNestedElement->NextSiblingElement())
		{
			//cout << ThreeLayersNestedElement->Value() << endl;
			TiXmlAttribute* attributeOfThreelayers = ThreeLayersNestedElement->FirstAttribute();  //�������  
			//cout << attributeOfThreelayers->Name() << ":" << attributeOfThreelayers->Value() << endl;
			vector<CString> vecRow, vecTitle;
			SheetSize.row = SheetSize.col = 0;

			for (; attributeOfThreelayers != NULL; attributeOfThreelayers = attributeOfThreelayers->Next())
			{
				//cout << attributeOfThreelayers->Name() << ":" << attributeOfThreelayers->Value() << endl;

				vecTitle.push_back(__Constchar2CString(ConvertUtf8ToAnsi(attributeOfThreelayers->Name())));
				vecRow.push_back(__Constchar2CString(ConvertUtf8ToAnsi(attributeOfThreelayers->Value())));
				SheetSize.col++;
			}
			if (!BoHaveTitle){
				BoHaveTitle = true;
				vecSheet.push_back(vecTitle);
				vecTitle.clear();
				vecSheet.push_back(vecTitle);
				SheetSize.row++;
			}
			vecSheet.push_back(vecRow);
			SheetSize.row++;
		}
		BoHaveTitle = false;
		m_mapSheetList[strSheetName] = vecSheet;
		m_mapSheetSize[strSheetName] = SheetSize;
		m_vecSheetName.push_back(strSheetName);
		nSheetCount++;
		
		vecSheet.clear();
		
	}
    
	//��m_mapSheetList����д��xls�ļ�
	LPDISPATCH lpDisp = NULL;
	if (!m_obExcel.open(__CString2Constchar(strOutputPath)))
	{
		MessageBox(_T("�޷��򿪸��ļ�"), _T("����"), MB_OK);
		return;
	}

	for (int i = m_vecSheetName.size()-1; i >=0 ; i--)
	{
		
		CString strSheetName = m_vecSheetName[i];
		if (!m_obExcel.addNewSheet(strSheetName))
		{
			MessageBox(_T("�޷��½�sheet"), _T("����"), MB_OK);
			return;
		}
		try{
			m_obExcel.useSheet(strSheetName);
		}
		catch(...)
		{
			MessageBox(_T("�޷�ʹ��ָ��sheet��")+strSheetName, _T("����"), MB_OK);
			return;
		}
		
		vector<vector<CString>> vecTempSheet;
		vecTempSheet = m_mapSheetList[strSheetName];


		for(int tempRow=0; tempRow < vecTempSheet.size(); tempRow++)
		{

			vector<CString> vecTempRow;
			vecTempRow = vecTempSheet[tempRow];
			for(int tempCol=0; tempCol < vecTempRow.size(); tempCol++)
			{
				CString tempCell;
				tempCell = vecTempRow[tempCol];
				try{
					m_obExcel.setCellString(tempRow + 1, tempCol + 1, tempCell);
				}
				catch(...)
				{
					MessageBox(_T("�޷�ʹ��д��cell���������������"), _T("����"), MB_OK);
					return;
				}
			}
		}
	}
    m_obExcel.saveAsXLSFile(strOutputPath);  //��ʱ���ɵ�xls��ΪAnsi��ʽ�ַ�
    m_obExcel.close();

}
*/


/*�ֱ���ܱ�ģʽXMLתExcel*/
void CReadXlsDlg::OnBnClickedButton3()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString strInputFile;
	CString strOutputPath;
	strInputFile.Empty();
	strOutputPath.Empty();
	m_editInputPath.GetWindowTextW(strInputFile);
	m_editOutputPath.GetWindowTextW(strOutputPath);

	//strInputFile = "C:\\Users\\linzhaolun.allen\\Desktop\\a.xml";
	//strOutputPath = "C:\\Users\\linzhaolun.allen\\Desktop\\b.xls";
	if (0 == strInputFile.GetLength())
	{
		MessageBox(_T("����ѡ����Ҫ��ȡ���ļ�"), _T("����"), MB_OK);
		return;
	}
	if (0 == strOutputPath.GetLength())
	{
		MessageBox(_T("����ѡ�����·��"), _T("����"), MB_OK);
		return;
	}

	vector<vector<CString>> vecSheet;
	CSheetSize SheetSize;
	TiXmlDocument *doc = new TiXmlDocument(__CString2Constchar(strInputFile));
	if (!doc->LoadFile(TIXML_ENCODING_UTF8))  //�ж�XML�ļ��Ƿ���سɹ�
	{
		MessageBox(_T("�޷��򿪸��ļ�"), _T("����"), MB_OK);
		return;
	}

	//doc->Parse(xmlParament,0,TIXML_ENCODING_UTF8);
    //Ŀǰ��֧�ֺϱ�ģʽxml�Ľ���
	int BoHaveTitle = false;
	TiXmlElement* RootLayersNestedElement = doc->RootElement();
	if (NULL == RootLayersNestedElement) //�ж��ļ��Ƿ�������
	{
		MessageBox(_T("û��root element"), _T("����"), MB_OK);
		return;
	}
	//cout << doc->Value() << endl;
	m_stroutputName = RootLayersNestedElement->Value();       //ÿ��xml��root����Ϊ�ļ���
	//cout << m_stroutputName << endl;
	m_mapSheetList.erase(m_mapSheetList.begin(), m_mapSheetList.end()); //ע��ʹ��mapǰ����������
	m_mapSheetSize.erase(m_mapSheetSize.begin(), m_mapSheetSize.end());
	m_vecSheetName.clear();
	TiXmlElement* TwoLayersNestedElement = RootLayersNestedElement->FirstChildElement();   //�ڶ���Ƕ��
	int nSheetCount =0;

	for (; TwoLayersNestedElement != NULL; TwoLayersNestedElement = TwoLayersNestedElement->NextSiblingElement()) 
	{  //ÿһ��ѭ������һ��sheet

		CString strSheetName;
		CString strSpecialSheetPrefix("xxxxx");
		CString strList("List");
		//CString strConnectSheetAndAtti("yyyyy");
		vector<CString> sub_vecSheetName;  
		vector<CString>::iterator it;
		map<CString, CExcelSheet> sub_mapSheetList;
		strSheetName = TwoLayersNestedElement->Value();//��ȡsheet��
		//cout << strSheetName << endl;
		TiXmlElement* ThreeLayersNestedElement = TwoLayersNestedElement->FirstChildElement();  //������Ƕ�� 
		if (TwoLayersNestedElement->FirstAttribute())
		{
			strSheetName = strSpecialSheetPrefix + strSheetName + strList;
			ThreeLayersNestedElement = TwoLayersNestedElement;
		}
		if (TwoLayersNestedElement->Value() != ThreeLayersNestedElement->Value())
		{
			//CString tempStr(ThreeLayersNestedElement->Value());
			//strSheetName = strSheetName + strConnectSheetAndAtti + tempStr;
			//MessageBox(_T("item��list֮�������淶����list��=item��+��list"), _T("����"), MB_OK);
			//return;
		}
		for (; ThreeLayersNestedElement != NULL; ThreeLayersNestedElement = ThreeLayersNestedElement->NextSiblingElement())
		{	
			if (ThreeLayersNestedElement->FirstChildElement() && ThreeLayersNestedElement->FirstChildElement()->FirstChildElement())
			{
				TiXmlElement* FourLayersNestedElement = ThreeLayersNestedElement->FirstChildElement();  //���Ĳ�Ƕ�ף��൱�ڷֱ��Excel�ܱ��չ����ÿһ��Ϊһ���µ�sheet�����sheet�ĵ�һ�У��������ǰ����������ID����Ϊ����
				  //�����Ƕ�ף��µ�sheet�����ÿһ��

				for (; FourLayersNestedElement != NULL; FourLayersNestedElement = FourLayersNestedElement->NextSiblingElement())//ÿһ��ѭ����ζ�Ŵ���һ���µķֱ�
				{
					CExcelSheet asheet;
					asheet.BoHaveTitle = false;
					asheet.strSheetName = FourLayersNestedElement->Value();//��ȡsheet��
					/*��vec���Ѿ����ڸ�sheet����Ҳ����֮ǰ�Ѿ��зֱ���ֹ��ˣ���ʱҪ�����ǰѻ�õ�����push��֮ǰ�ı���*/
					it = find(sub_vecSheetName.begin(), sub_vecSheetName.end(), asheet.strSheetName);
					if (it != sub_vecSheetName.end())
					{
						asheet = sub_mapSheetList[asheet.strSheetName];
					}
					vector<CString> vecSubRow, vecSubTitle;	
					vecSubTitle.push_back(__Constchar2CString(ConvertUtf8ToAnsi(ThreeLayersNestedElement->FirstAttribute()->Name())));
					vecSubRow.push_back(__Constchar2CString(ConvertUtf8ToAnsi(ThreeLayersNestedElement->FirstAttribute()->Value())));
					TiXmlElement* FiveLayersNestedElement = FourLayersNestedElement->FirstChildElement();
					for (; FiveLayersNestedElement != NULL; FiveLayersNestedElement = FiveLayersNestedElement->NextSiblingElement())
					{
						TiXmlAttribute* attributeOfFivelayers = FiveLayersNestedElement->FirstAttribute();  //������� 
						for (; attributeOfFivelayers != NULL; attributeOfFivelayers = attributeOfFivelayers->Next())
						{
							vecSubTitle.push_back(__Constchar2CString(ConvertUtf8ToAnsi(attributeOfFivelayers->Name())));
							vecSubRow.push_back(__Constchar2CString(ConvertUtf8ToAnsi(attributeOfFivelayers->Value())));
						}
						if (!asheet.BoHaveTitle){
							asheet.BoHaveTitle = true;
							asheet.vecSheet.push_back(vecSubTitle);
							vecSubTitle.clear();
							asheet.vecSheet.push_back(vecSubTitle);
							//SheetSize.row++;
						}
						asheet.vecSheet.push_back(vecSubRow);
						vecSubRow.clear();
						vecSubRow.push_back(__Constchar2CString(ConvertUtf8ToAnsi(ThreeLayersNestedElement->FirstAttribute()->Value())));
						it = find(sub_vecSheetName.begin(), sub_vecSheetName.end(), asheet.strSheetName);
						if (it == sub_vecSheetName.end())
						{
							sub_vecSheetName.push_back(asheet.strSheetName);
						}
						sub_mapSheetList[asheet.strSheetName] = asheet;			
					}						
				}
			}

			TiXmlAttribute* attributeOfThreelayers = ThreeLayersNestedElement->FirstAttribute();  //�������  
		
			vector<CString> vecRow, vecTitle;
			//SheetSize.row = SheetSize.col = 0;

			for (; attributeOfThreelayers != NULL; attributeOfThreelayers = attributeOfThreelayers->Next())
			{
				//cout << attributeOfThreelayers->Name() << ":" << attributeOfThreelayers->Value() << endl;

				vecTitle.push_back(__Constchar2CString(ConvertUtf8ToAnsi(attributeOfThreelayers->Name())));
				vecRow.push_back(__Constchar2CString(ConvertUtf8ToAnsi(attributeOfThreelayers->Value())));
				//SheetSize.col++;
			}
			if (!BoHaveTitle){
				BoHaveTitle = true;
				vecSheet.push_back(vecTitle);
				vecTitle.clear();
				vecSheet.push_back(vecTitle);
				//SheetSize.row++;
			}
			vecSheet.push_back(vecRow);
			//SheetSize.row++;
			
		}
		BoHaveTitle = false;
		m_mapSheetList[strSheetName] = vecSheet;
		//m_mapSheetSize[strSheetName] = SheetSize;
		m_vecSheetName.push_back(strSheetName);
		for (int i = 0; i < sub_vecSheetName.size(); i++)
		{
			m_vecSheetName.push_back(sub_vecSheetName[i]);
			m_mapSheetList[sub_vecSheetName[i]] = sub_mapSheetList[sub_vecSheetName[i]].vecSheet;
		}

		nSheetCount++;
		vecSheet.clear();
		sub_mapSheetList.erase(sub_mapSheetList.begin(), sub_mapSheetList.end()); //ע��ʹ��mapǰ����������
		sub_vecSheetName.clear();
		
	}
    
	//��m_mapSheetList����д��xls�ļ�
	LPDISPATCH lpDisp = NULL;
	if (!m_obExcel.open(__CString2Constchar(strOutputPath)))
	{
		MessageBox(_T("�޷��򿪸��ļ�"), _T("����"), MB_OK);
		return;
	}

	for (int i = m_vecSheetName.size()-1; i >=0 ; i--)
	{
		
		CString strSheetName = m_vecSheetName[i];
		if (!m_obExcel.addNewSheet(strSheetName))
		{
			MessageBox(_T("�޷��½�sheet"), _T("����"), MB_OK);
			return;
		}
		try{
			m_obExcel.useSheet(strSheetName);
		}
		catch(...)
		{
			MessageBox(_T("�޷�ʹ��ָ��sheet��")+strSheetName, _T("����"), MB_OK);
			return;
		}
		
		vector<vector<CString>> vecTempSheet;
		vecTempSheet = m_mapSheetList[strSheetName];


		for(int tempRow=0; tempRow < vecTempSheet.size(); tempRow++)
		{

			vector<CString> vecTempRow;
			vecTempRow = vecTempSheet[tempRow];
			for(int tempCol=0; tempCol < vecTempRow.size(); tempCol++)
			{
				CString tempCell;
				tempCell = vecTempRow[tempCol];
				try{
					m_obExcel.setCellString(tempRow + 1, tempCol + 1, tempCell);
				}
				catch(...)
				{
					MessageBox(_T("�޷�ʹ��д��cell���������������"), _T("����"), MB_OK);
					return;
				}
			}
		}
	}
	m_obExcel.deleteSheet(__Constchar2CString("Sheet1"));
	m_obExcel.deleteSheet(__Constchar2CString("Sheet2"));
	m_obExcel.deleteSheet(__Constchar2CString("Sheet3"));
	m_obExcel.saveAsXLSFile(strOutputPath);  //��ʱ���ɵ�xls��ΪAnsi��ʽ�ַ�,xls�ٴ�ת��XMLʱ�ᱻת���ΪUTF-8
    m_obExcel.close();


	
}
