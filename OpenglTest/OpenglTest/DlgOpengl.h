#pragma once


// CDlgOpengl 对话框

class CDlgOpengl : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgOpengl)

public:
	CDlgOpengl(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgOpengl();
	BOOL Create();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
