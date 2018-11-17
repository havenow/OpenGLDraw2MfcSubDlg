
// OpenglTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OpenglTest.h"
#include "OpenglTestDlg.h"
#include "afxdialogex.h"
#include "DlgOpengl.h"
#include "glesRender.hpp"
CglesRender g_glesRender;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static inline void rgb888to565(int* src, short* dest, int width, int height)
{
	for (int i = 0; i < width * height; i++) {
		dest[i] = (((src[i] >> 19) & 0x1F) << 11) |
			(((src[i] >> 10) & 0x3F) << 5) |
			((src[i] >> 3) & 0x1F);
	}
}

static GLint imagewidth;
static GLint imageheight;
static GLint pixellength;
static GLubyte* pixeldata;
void readBmp()
{

//���ļ�
FILE* pfile = fopen("C:\\Users\\wangf.5FUN\\Desktop\\OpenglTest\\OpenglTest\\calib_front_1.bmp", "rb");
if (pfile == 0) exit(0);

//��ȡͼ���С
fseek(pfile, 0x0012, SEEK_SET);
fread(&imagewidth, sizeof(imagewidth), 1, pfile);
fread(&imageheight, sizeof(imageheight), 1, pfile);

//�����������ݳ���
pixellength = imagewidth * 3;
while (pixellength % 4 != 0)pixellength++;
pixellength *= imageheight;

//��ȡ��������
pixeldata = (GLubyte*)malloc(pixellength);
if (pixeldata == 0) exit(0);

fseek(pfile, 54, SEEK_SET);
fread(pixeldata, pixellength, 1, pfile);

//�ر��ļ�
fclose(pfile);


}

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

END_MESSAGE_MAP()


// COpenglTestDlg �Ի���



COpenglTestDlg::COpenglTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_OPENGLTEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COpenglTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COpenglTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPENGL, &COpenglTestDlg::OnBnClickedBtnOpengl)
END_MESSAGE_MAP()


// COpenglTestDlg ��Ϣ�������

BOOL COpenglTestDlg::OnInitDialog()
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

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_pDlg = new CDlgOpengl(this);
	m_pDlg->Create();

//#define MAIN_DLG
#ifdef MAIN_DLG
	m_hGLWnd = GetDlgItem(IDC_PIC)->GetSafeHwnd();
#else
	m_hGLWnd = m_pDlg->GetDlgItem(IDC_PIC_OPENGL)->GetSafeHwnd();
#endif
	g_glesRender.setHwnd(m_hGLWnd);
	CRect rect;
#ifdef MAIN_DLG
	GetDlgItem(IDC_PIC)->GetClientRect(rect);
#else
	m_pDlg->GetDlgItem(IDC_PIC_OPENGL)->GetClientRect(rect);
#endif
	int nWidth = rect.right - rect.left;
	int nHeight = rect.bottom - rect.top;
	g_glesRender.setWH(nWidth, nHeight);
	g_glesRender.initOpenGLES20();
	g_glesRender.initShader();
	g_glesRender.initVideoTexture();

	/*
	char*   data = new char[128 * 128 * 4];
	for (int i = 0; i < 128 * 128 * 4; ++i)
	{
		data[i] = rand() % 255;
	}
	g_glesRender.loopVideo(128, 128, (BYTE *)data);
	delete [] data;
	*/

	/*
	readBmp();
	g_glesRender.loopVideo(imagewidth, imageheight, (BYTE *)pixeldata);
	*/

	g_glesRender.initBMP("C:\\Users\\wangf.5FUN\\Desktop\\OpenglTest\\OpenglTest\\calib_front_1.bmp");
	g_glesRender.loopVideo(g_glesRender.m_bmpBk.nWidth, g_glesRender.m_bmpBk.nHeight, (BYTE *)g_glesRender.m_bmpBk.pData);


	g_glesRender.initTipsBuffer("C:\\Users\\wangf.5FUN\\Desktop\\OpenglTest\\OpenglTest\\tips_fullscreen.png");
	//g_glesRender.setFullscreenTips(true);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void COpenglTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void COpenglTestDlg::OnPaint()
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
		g_glesRender.render(true);
		g_glesRender.swapBuffer();

		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR COpenglTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void COpenglTestDlg::OnBnClickedBtnOpengl()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDlgOpengl dlg;
	//dlg.DoModal();
	CRect rect;
	m_pDlg->GetWindowRect(&rect);
	m_pDlg->MoveWindow(0, 0, rect.Width(), rect.Height());//������ʾ
	m_pDlg->ShowWindow(SW_SHOW);
	
	Invalidate();
}



COpenglTestDlg::~COpenglTestDlg()
{
	free(pixeldata);
	if (m_pDlg)
	{
		delete m_pDlg;
	}
	g_glesRender.destroyOpenGLES20();
}