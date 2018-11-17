
// OpenglTestDlg.h : 头文件
//

#pragma once
#include "DlgOpengl.h"

// COpenglTestDlg 对话框
class COpenglTestDlg : public CDialogEx
{
public:
	HGLRC m_hRC;					//着色描述表
	CDC *m_pDC;						//设备描述表
	HWND m_hGLWnd;
	//BOOL InitOpengl();				//初始化opengl
	//BOOL SetupPixelFormat();		//设置图片控件的像素格式
	//void RenderScene();				//绘制图像
	CDlgOpengl* m_pDlg = nullptr;

// 构造
public:
	COpenglTestDlg(CWnd* pParent = NULL);	// 标准构造函数
	~COpenglTestDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPENGLTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnOpengl();
};
