
// MFC_STARTView.cpp: CMFCSTARTView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFC_START.h"
#endif

#include "MFC_STARTDoc.h"
#include "MFC_STARTView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCSTARTView

IMPLEMENT_DYNCREATE(CMFCSTARTView, CView)

BEGIN_MESSAGE_MAP(CMFCSTARTView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_MOUSEACTIVATE()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CMFCSTARTView 构造/析构

CMFCSTARTView::CMFCSTARTView() noexcept
{
	// TODO: 在此处添加构造代码
	_oldPen = nullptr;
	_mouseDown = false;
}

CMFCSTARTView::~CMFCSTARTView()
{
}

BOOL CMFCSTARTView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMFCSTARTView 绘图

void CMFCSTARTView::OnDraw(CDC* pDC)
{
	CMFCSTARTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	if (_mouseDown)
	{
		DrawCircle();
	}

}


// CMFCSTARTView 打印

BOOL CMFCSTARTView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMFCSTARTView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMFCSTARTView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CMFCSTARTView 诊断

#ifdef _DEBUG
void CMFCSTARTView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCSTARTView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCSTARTDoc* CMFCSTARTView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCSTARTDoc)));
	return (CMFCSTARTDoc*)m_pDocument;
}
#endif //_DEBUG




// CMFCSTARTView 消息处理程序

void CMFCSTARTView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	_curPoint = point;
	Invalidate(0);

	CView::OnMouseMove(nFlags, point);
}


void CMFCSTARTView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	_mouseDown = true;
	_startPoint = point;
	_oldPoint = point;

	CView::OnLButtonDown(nFlags, point);
}


void CMFCSTARTView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	_mouseDown = false;

	CView::OnLButtonUp(nFlags, point);
}

void CMFCSTARTView::DrawDot()
{
	auto pDC = GetDC();
	pDC->SetPixel(_curPoint, AFX_IDC_COLOR_DARKBLUE);
}

void CMFCSTARTView::DrawLine()
{
	auto pDC = GetDC();
	// 切换画笔
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, AFX_IDC_COLOR_DARKBLUE);
	_oldPen = pDC->SelectObject(&pen);
	pDC->SetROP2(R2_XORPEN);// 设置绘画模式为异或
	// 覆盖旧位置
	pDC->MoveTo(_startPoint);
	pDC->LineTo(_oldPoint);
	// 画新位置
	pDC->MoveTo(_startPoint);
	pDC->LineTo(_curPoint);
	_oldPoint = _curPoint; // 更新旧点
	pDC->SelectObject(_oldPen);// 恢复画笔
}

void CMFCSTARTView::DrawRec()
{
	CDC* pDC = GetDC();
	// 切换画笔
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, AFX_IDC_COLOR_DARKBLUE);
	_oldPen = pDC->SelectObject(&pen);
	pDC->SetROP2(R2_XORPEN);// 设置绘画模式为异或
	pDC->SelectStockObject(NULL_BRUSH);// 设置透明刷子
	pDC->Rectangle(_startPoint.x, _startPoint.y, _oldPoint.x, _oldPoint.y);// 覆盖旧位置
	pDC->Rectangle(_startPoint.x, _startPoint.y, _curPoint.x, _curPoint.y);// 画新位置
	_oldPoint = _curPoint;// 更新旧点
	// 恢复画笔
	pDC->SelectObject(_oldPen);
}

void CMFCSTARTView::DrawCircle()
{
	CDC* pDC = GetDC();
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, AFX_IDC_COLOR_DARKBLUE);
	_oldPen = pDC->SelectObject(&pen);
	pDC->SetROP2(R2_XORPEN);// 设置绘画模式为异或
	pDC->SelectStockObject(NULL_BRUSH);// 设置透明刷子
	pDC->Ellipse(_startPoint.x, _startPoint.y, _oldPoint.x, _oldPoint.y);// 覆盖旧位置
	pDC->Ellipse(_startPoint.x, _startPoint.y, _curPoint.x, _curPoint.y);// 画新位置
	_oldPoint = _curPoint;// 更新旧点
	// 恢复画笔
	pDC->SelectObject(_oldPen);
}


