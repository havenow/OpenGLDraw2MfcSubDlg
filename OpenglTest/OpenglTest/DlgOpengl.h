#pragma once


// CDlgOpengl �Ի���

class CDlgOpengl : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgOpengl)

public:
	CDlgOpengl(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgOpengl();
	BOOL Create();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
