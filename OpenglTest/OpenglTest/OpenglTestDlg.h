
// OpenglTestDlg.h : ͷ�ļ�
//

#pragma once
#include "DlgOpengl.h"

// COpenglTestDlg �Ի���
class COpenglTestDlg : public CDialogEx
{
public:
	HGLRC m_hRC;					//��ɫ������
	CDC *m_pDC;						//�豸������
	HWND m_hGLWnd;
	//BOOL InitOpengl();				//��ʼ��opengl
	//BOOL SetupPixelFormat();		//����ͼƬ�ؼ������ظ�ʽ
	//void RenderScene();				//����ͼ��
	CDlgOpengl* m_pDlg = nullptr;

// ����
public:
	COpenglTestDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~COpenglTestDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPENGLTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnOpengl();
};
