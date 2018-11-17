// DlgOpengl.cpp : 实现文件
//

#include "stdafx.h"
#include "OpenglTest.h"
#include "DlgOpengl.h"
#include "afxdialogex.h"


// CDlgOpengl 对话框

IMPLEMENT_DYNAMIC(CDlgOpengl, CDialogEx)

CDlgOpengl::CDlgOpengl(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
	
}

CDlgOpengl::~CDlgOpengl()
{
}

void CDlgOpengl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgOpengl, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlgOpengl 消息处理程序

BOOL CDlgOpengl::Create()
{
	return CDialog::Create(IDD_DIALOG1);
}
