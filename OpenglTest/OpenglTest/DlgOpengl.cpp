// DlgOpengl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OpenglTest.h"
#include "DlgOpengl.h"
#include "afxdialogex.h"


// CDlgOpengl �Ի���

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


// CDlgOpengl ��Ϣ�������

BOOL CDlgOpengl::Create()
{
	return CDialog::Create(IDD_DIALOG1);
}
