
// DynamicCircleDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "DynamicCircle.h"
#include "DynamicCircleDlg.h"
#include "afxdialogex.h"

#include <thread>
#include <chrono>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CDynamicCircleDlg 대화 상자


void thRandCircle(CWnd* pParent)
{
	CDynamicCircleDlg* pWnd = (CDynamicCircleDlg*)pParent;
	pWnd->randCircle();
}

CDynamicCircleDlg::CDynamicCircleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DYNAMICCIRCLE_DIALOG, pParent)
	, m_nRadius(0), m_nBorderWidth(0), m_nCircleCnt(0), m_nDragNum(4)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDynamicCircleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDynamicCircleDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BTN_DRAW, &CDynamicCircleDlg::OnBnClickedBtnDraw)
	ON_BN_CLICKED(IDC_BTN_RESET, &CDynamicCircleDlg::OnBnClickedBtnReset)
	ON_BN_CLICKED(IDC_BTN_RAND, &CDynamicCircleDlg::OnBnClickedBtnRand)
END_MESSAGE_MAP()


// CDynamicCircleDlg 메시지 처리기

BOOL CDynamicCircleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	SetWindowText(L"Dynamin Circle");

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CDynamicCircleDlg::OnSysCommand(UINT nID, LPARAM lParam)
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


// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CDynamicCircleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDynamicCircleDlg::OnBnClickedBtnDraw()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CStringW wStrRadius, wStrBorderWidth;
	GetDlgItem(IDC_EDIT_RADIUS)->GetWindowTextW(wStrRadius);
	GetDlgItem(IDC_EDIT_BORDER_WIDTH)->GetWindowTextW(wStrBorderWidth);
	m_nRadius = _wtoi(wStrRadius);
	m_nBorderWidth = _wtoi(wStrBorderWidth);

	if (isInputSettingValue()) {
		GetDlgItem(IDC_EDIT_RADIUS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BORDER_WIDTH)->EnableWindow(FALSE);
		GetFocus()->EnableWindow(FALSE);

		if (m_image != NULL) {
			m_image.Destroy();
		}

		int nBpp = 8;
		CRect rcClient;
		GetClientRect(&rcClient);
		m_image.Create(rcClient.Width(), -rcClient.Height() + TOP_HEIGHT, nBpp);
		if (nBpp == 8) {
			static RGBQUAD rgb[256];
			for (int i = 0; i < 256; i++)
				rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
			m_image.SetColorTable(0, 256, rgb);
		}

		int nPitch = m_image.GetPitch();
		unsigned char* fm = (unsigned char*)m_image.GetBits();
		memset(fm, 0xff, m_image.GetWidth() * m_image.GetHeight());

		updateDisplay();
	}
	else {
		MessageBox(L"반지름 및 가장자리 두께를 0 이상 입력");
	}
}


void CDynamicCircleDlg::OnBnClickedBtnRand()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_image) {
		unsigned char* fm = (unsigned char*)m_image.GetBits();

		if (isInputSettingValue()) {
			std::thread thrRand([this]() {
				srand(time(NULL));
				for (int i = 0; i < RAND_NUM; i++) {
					randCircle();
					std::this_thread::sleep_for(std::chrono::milliseconds(RAND_TIME));
				}
				});
			thrRand.detach();
		}
		else {
			MessageBox(L"반지름 및 가장자리 두께를 0 이상 입력");
		}
	}
}


void CDynamicCircleDlg::OnBnClickedBtnReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nRadius = 0;
	m_nBorderWidth = 0;
	m_nCircleCnt = 0;
	m_nDragNum = 4;

	GetDlgItem(IDC_EDIT_RADIUS)->SetWindowTextW(L"");
	GetDlgItem(IDC_EDIT_BORDER_WIDTH)->SetWindowTextW(L"");
	GetDlgItem(IDC_EDIT_CIRCLE_POINT)->SetWindowTextW(L"");
	GetDlgItem(IDC_EDIT_RADIUS)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_BORDER_WIDTH)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_DRAW)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_RAND)->EnableWindow(FALSE);

	unsigned char* fm = (unsigned char*)m_image.GetBits();
	memset(fm, 0xff, m_image.GetWidth() * m_image.GetHeight());
	updateDisplay();
}


void CDynamicCircleDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	point.y -= TOP_HEIGHT;

	if (0 < point.y) {
		if (isInputSettingValue()) {
			if ((m_image) && (m_nCircleCnt < CIRCLE_NUM)) {
				m_posCircle[m_nCircleCnt++] = point;

				unsigned char* fm = (unsigned char*)m_image.GetBits();
				drawCircle(fm, point.x, point.y, m_nRadius, 0x00);
				updateDisplay();
				
				if (m_nCircleCnt == CIRCLE_NUM) {
					GetDlgItem(IDC_BTN_RAND)->EnableWindow(TRUE);
					CStringW wStrCirclePoint;
					wStrCirclePoint.Format(L"(%4d, %4d) (%4d, %4d) (%4d, %4d)"
						, m_posCircle[0].x, m_posCircle[0].y, m_posCircle[1].x, m_posCircle[1].y, m_posCircle[2].x, m_posCircle[2].y);
					GetDlgItem(IDC_EDIT_CIRCLE_POINT)->SetWindowTextW(wStrCirclePoint);
					calculateThreePointCircle(m_posCircle);
				}
			}
			else {
				for (int i = 0; i < CIRCLE_NUM; i++) {
					if (isBelongToCircle(point.x, point.y, m_posCircle[i].x, m_posCircle[i].y, m_nRadius)) {
						m_bDrag = TRUE;
						m_nDragNum = i;
						break;
					}
				}
			}
		}
		else {
			MessageBox(L"반지름 및 가장자리 두께를 0 이상 입력");
		}
	}	

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CDynamicCircleDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bDrag) {
		m_bDrag = FALSE;
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CDynamicCircleDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bDrag) {
		point.y -= TOP_HEIGHT;
		
		int nStartX = point.x - m_nRadius;
		int nStartY = point.y - m_nRadius;
		if ((0 <= nStartX) && (0 <= nStartY)
			&& (m_image.GetWidth() > nStartX) && (m_image.GetHeight() > nStartY)) {
			unsigned char* fm = (unsigned char*)m_image.GetBits();
			drawCircle(fm, m_posCircle[m_nDragNum].x, m_posCircle[m_nDragNum].y, m_nRadius, 0xff);
			drawCircle(fm, point.x, point.y, m_nRadius, 0x00);
			updateDisplay();
			
			if (m_nCircleCnt == CIRCLE_NUM) {
				CStringW wStrCirclePoint;
				wStrCirclePoint.Format(L"(%4d, %4d) (%4d, %4d) (%4d, %4d)"
					, m_posCircle[0].x, m_posCircle[0].y, m_posCircle[1].x, m_posCircle[1].y, m_posCircle[2].x, m_posCircle[2].y);
				GetDlgItem(IDC_EDIT_CIRCLE_POINT)->SetWindowTextW(wStrCirclePoint);
				calculateThreePointCircle(m_posCircle);
			}
			m_tx.Lock();
			m_posCircle[m_nDragNum] = point;
			m_tx.Unlock();
		}
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

BOOL CDynamicCircleDlg::isInputSettingValue()
{
	BOOL bRet = FALSE;
	if ((0 < m_nRadius) && (0 < m_nBorderWidth)) {
		bRet = TRUE;
	}
	return bRet;
}

void CDynamicCircleDlg::updateDisplay()
{
	CClientDC dc(this);
	m_image.Draw(dc, 0, TOP_HEIGHT);
}

void CDynamicCircleDlg::drawCircle(unsigned char* fm, int nX, int nY, int nRadius, int nGray)
{
	int nStartX = nX - nRadius;
	int nStartY = nY - nRadius;
	int nPitch = m_image.GetPitch();

	for (int i = nStartY; i < nY + nRadius; i++) {
		for (int j = nStartX; j < nX + nRadius; j++) {
			if ((isBelongToCircle(nX, nY, j, i, nRadius))
				&& ((i * nPitch + j) < m_image.GetWidth() * m_image.GetHeight())) {
				fm[i * nPitch + j] = nGray;
			}
		}
	}
}

void CDynamicCircleDlg::drawCircle(int nX, int nY, int nRadius)
{
	CClientDC dc(this);
	CPen pen;
	pen.CreatePen(PS_SOLID, m_nBorderWidth, RGB(0x00, 0x00, 0x00));
	CPen* pOldPen = dc.SelectObject(&pen);

	int nVertex = 365;
	double dZeroX = nRadius * cos(0.0) + nX;
	double dZeroY = nRadius * sin(0.0) + nY;
	dc.MoveTo(dZeroX, dZeroY);

	for (int i = 0; i <= nVertex; i++)
	{
		double radian = i * 2 * PI / nVertex;
		double dx = nRadius * cos(radian) + nX;
		double dy = nRadius * sin(radian) + nY;

		if (TOP_HEIGHT > dy) {
			dc.MoveTo(dx, TOP_HEIGHT);
			continue;
		}
		dc.LineTo(dx, dy);
	}
	dc.SelectObject(pOldPen);
}

BOOL CDynamicCircleDlg::isBelongToCircle(int nX, int nY, int nPointX, int nPointY, int nRadius)
{
	bool bRet = false;
	double dX = nPointX - nX;
	double dY = nPointY - nY;
	double dDist = dX * dX + dY * dY;

	if (dDist < nRadius * nRadius) {
		bRet = true;
	}

	return bRet;
}

void CDynamicCircleDlg::calculateThreePointCircle(CPoint* posCircle)
{
	double dy1 = 0.0, dy2 = 0.0, d = 0.0, d2 = 0.0, yi = 0.0;
	double cx = 0.0, cy = 0.0;
	CPoint p1((posCircle[0].x + posCircle[1].x) / 2, (posCircle[0].y + posCircle[1].y) / 2);
	CPoint p2((posCircle[0].x + posCircle[2].x) / 2, (posCircle[0].y + posCircle[2].y) / 2);
	dy1 = posCircle[0].y - posCircle[1].y;
	dy2 = posCircle[0].y - posCircle[2].y;
	int nRad = 0;
	if (dy1 != 0) {
		d = (posCircle[1].x - posCircle[0].x) / dy1;
		yi = p1.y - d * p1.x;
		if (dy2 != 0) {
			d2 = (posCircle[2].x - posCircle[0].x) / dy2;
			if (d != d2) cx = (yi - (p2.y - d2 * p2.x)) / (d2 - d);
		}
		else if (posCircle[2].x - posCircle[0].x == 0) {}
		else cx = p2.x;

		cy = d * cx + yi;
		nRad = sqrt((posCircle[0].x - cx) * (posCircle[0].x - cx) + (posCircle[0].y - cy) * (posCircle[0].y - cy));
	}
	else if (dy2 != 0 && posCircle[1].x - posCircle[0].x != 0) {
		d = (posCircle[2].x - posCircle[0].x) / dy2;
		yi = p2.y - d * p2.x;
		cx = p1.x;
		cy = d * cx + yi;
		nRad = sqrt((posCircle[0].x - cx) * (posCircle[0].x - cx) + (posCircle[0].y - cy) * (posCircle[0].y - cy));
	}
	
	if (0 != cy) {
		drawCircle(cx, cy + TOP_HEIGHT, nRad);
	}
}

void CDynamicCircleDlg::randCircle()
{
	unsigned char* fm = (unsigned char*)m_image.GetBits();
	memset(fm, 0xff, m_image.GetWidth() * m_image.GetHeight());

	CPoint posRandCircle[3];
	for (int i = 0; i < CIRCLE_NUM; i++) {
		int nX = rand() % (m_image.GetWidth() - m_nRadius + 1) + m_nRadius;
		int nY = rand() % (m_image.GetHeight() - m_nRadius + 1) + m_nRadius;
		posRandCircle[i].x = nX;
		posRandCircle[i].y = nY;
		drawCircle(fm, nX, nY, m_nRadius, 0x00);
		updateDisplay();
	}
	
	if (m_nCircleCnt == CIRCLE_NUM) {
		calculateThreePointCircle(posRandCircle);
	}
}
