
// DynamicCircleDlg.h: 헤더 파일
//

#pragma once

#define TOP_HEIGHT 50
#define CIRCLE_NUM 3
#define RAND_NUM 10
#define PI 3.1415926535897932384626433832795
#define RAND_TIME 500

// CDynamicCircleDlg 대화 상자
class CDynamicCircleDlg : public CDialogEx
{
// 생성입니다.
public:
	CDynamicCircleDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	void randCircle();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DYNAMICCIRCLE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

	afx_msg void OnBnClickedBtnDraw();
	afx_msg void OnBnClickedBtnReset();
	afx_msg void OnBnClickedBtnRand();

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

private:
	CMutex m_tx;
	CImage m_image;
	CPoint m_posCircle[CIRCLE_NUM];
	int m_nRadius, m_nBorderWidth, m_nCircleCnt, m_nDragNum;
	BOOL m_bDrag;

	BOOL isInputSettingValue();
	void updateDisplay();
	void drawCircle(unsigned char* fm, int nX, int nY, int nRadius, int nGray);
	void drawCircle(int nX, int nY, int nRadius);
	BOOL isBelongToCircle(int nX, int nY, int nPointX, int nPointY, int nRadius);
	void calculateThreePointCircle(CPoint* posCircle);
};
