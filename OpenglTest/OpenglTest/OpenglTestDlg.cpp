
// OpenglTestDlg.cpp : 实现文件
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

//打开文件
FILE* pfile = fopen("C:\\Users\\wangf.5FUN\\Desktop\\OpenglTest\\OpenglTest\\calib_front_1.bmp", "rb");
if (pfile == 0) exit(0);

//读取图像大小
fseek(pfile, 0x0012, SEEK_SET);
fread(&imagewidth, sizeof(imagewidth), 1, pfile);
fread(&imageheight, sizeof(imageheight), 1, pfile);

//计算像素数据长度
pixellength = imagewidth * 3;
while (pixellength % 4 != 0)pixellength++;
pixellength *= imageheight;

//读取像素数据
pixeldata = (GLubyte*)malloc(pixellength);
if (pixeldata == 0) exit(0);

fseek(pfile, 54, SEEK_SET);
fread(pixeldata, pixellength, 1, pfile);

//关闭文件
fclose(pfile);


}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// COpenglTestDlg 对话框



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


// COpenglTestDlg 消息处理程序

BOOL COpenglTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void COpenglTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		g_glesRender.render(true);
		g_glesRender.swapBuffer();

		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR COpenglTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void COpenglTestDlg::OnBnClickedBtnOpengl()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDlgOpengl dlg;
	//dlg.DoModal();
	CRect rect;
	m_pDlg->GetWindowRect(&rect);
	m_pDlg->MoveWindow(0, 0, rect.Width(), rect.Height());//靠右显示
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