
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

#include "DrawToolsDlg.h"
#include "json.hpp"

using json = nlohmann::json;
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
	ON_WM_MBUTTONDOWN()
	// 打开绘图工具对话框
	ON_COMMAND(1, &CMFCSTARTView::OpenDTDlgCmd)
	ON_COMMAND(2, &CMFCSTARTView::OnFileSaveAs)
	ON_COMMAND(3, &CMFCSTARTView::OnFileOpen)
END_MESSAGE_MAP()

// CMFCSTARTView 构造/析构

CMFCSTARTView::CMFCSTARTView() noexcept
{
	// TODO: 在此处添加构造代码
	// 以默认值初始化一系列成员
	_mouseDown = false;
	_mouseJustUp = false;
	_drawingPoly = false;
	_endingPoly = false;
	_oldPen = nullptr;
	_toolDlg = nullptr;
	DrawTask = DRAW_DOT;
	LineWidth = 1;
	PenStyle = PS_SOLID;
	R = 0;
	G = 0;
	B = 0;
}


CMFCSTARTView::~CMFCSTARTView()
{
	if (_toolDlg != nullptr)
		delete _toolDlg;
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
	if (_mouseDown || _mouseJustUp || _drawingPoly || _endingPoly)
	{
		switch (DrawTask)
		{
		case DRAW_DOT:
			DrawDot();
			break;
			// 画线、多边形、曲线都是用DrawLine。
		case DRAW_LINE:
		case DRAW_POLY:
		case DRAW_CURVE:
			DrawLine(!(_mouseJustUp || DrawTask == DRAW_CURVE));
			break;
		case DRAW_REC:
			DrawRec(!_mouseJustUp);
			break;
		case DRAW_CIRCLE:
			DrawCircle(!_mouseJustUp);
			break;
		}
		if (_endingPoly)
			_endingPoly = false;
		if (_mouseJustUp)
			_mouseJustUp = false;
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

	// 绘制曲线时，由于采用的是用直线首尾相连的方式，所以绘制曲线时，更新方式不同。
	if (DrawTask == DRAW_CURVE) {
		_startPoint = _oldPoint;
		_curPoint = point;
	}
	else
		_curPoint = point;

	Invalidate(0);

	CView::OnMouseMove(nFlags, point);
}


void CMFCSTARTView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	_mouseDown = true;
	if (!_drawingPoly) {
		// 如果是画多边形以外的东西，那么只需修改点。
		_startPoint = point;
		_oldPoint = point;
	}
	if (DrawTask == DRAW_POLY && !_drawingPoly) {
		// 如果是刚开始画多边形，那么记录下多边形的起点，以及更改绘画状态。
		_drawingPoly = true;
		_polyStartPoint = point;
	}

	CView::OnLButtonDown(nFlags, point);
}


void CMFCSTARTView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	_mouseDown = false;
	_mouseJustUp = true;
	Invalidate(0);
	CView::OnLButtonUp(nFlags, point);
}

void CMFCSTARTView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 按下鼠标中件时，如果正在画多边形，那么就闭合这个多边形，完成这次绘画。
	if (_drawingPoly) {
		_drawingPoly = false;
		// 更改绘制点，让图形首尾闭合
		_curPoint = _polyStartPoint;
		_endingPoly = true;
		Invalidate(0);
	}
	CView::OnMButtonDown(nFlags, point);
}

// 打开绘画工具对话框
void CMFCSTARTView::OpenDTDlgCmd()
{
	if (_toolDlg == nullptr)
	{
		_toolDlg = new DrawToolsDlg(this);
		_toolDlg->Create(IDD_DRAWTOOLSDLG, this);
	}
	_toolDlg->ShowWindow(SW_SHOW);
}

void CMFCSTARTView::DrawDot()
{
	auto pDC = GetDC();
	pDC->SetPixel(_curPoint, RGB(R, G, B));
}

void CMFCSTARTView::DrawLine(bool xorMode)
{
	auto pDC = GetDC();
	// 切换画笔
	CPen pen;
	COLORREF color;
	color = RGB(255 - R, 255 - G, 255 - B);
	pen.CreatePen(PenStyle, LineWidth, color);
	_oldPen = pDC->SelectObject(&pen);
	pDC->SetROP2(R2_XORPEN);// 设置绘画模式为异或
	// 覆盖旧位置
	pDC->MoveTo(_startPoint);
	pDC->LineTo(_oldPoint);
	// 如果是闭合一个多边形，那么这个情况比较特殊
	if (!xorMode || _endingPoly) {
		pDC->SetROP2(R2_NOTCOPYPEN);
	}
	// 画新位置
	pDC->MoveTo(_startPoint);
	pDC->LineTo(_curPoint);
	_oldPoint = _curPoint; // 更新旧点
	if (_drawingPoly && _mouseJustUp)
		_startPoint = _curPoint;
	pDC->SelectObject(_oldPen);// 恢复画笔
}

void CMFCSTARTView::DrawRec(bool xorMode)
{
	CDC* pDC = GetDC();
	// 切换画笔
	CPen pen;
	COLORREF color;
	color = RGB(255 - R, 255 - G, 255 - B);
	pen.CreatePen(PenStyle, LineWidth, color);
	_oldPen = pDC->SelectObject(&pen);
	pDC->SetROP2(R2_XORPEN);// 设置绘画模式为异或
	pDC->SelectStockObject(NULL_BRUSH);// 设置透明刷子
	pDC->Rectangle(_startPoint.x, _startPoint.y, _oldPoint.x, _oldPoint.y);// 覆盖旧位置
	if (!xorMode) {
		pDC->SetROP2(R2_NOTCOPYPEN);
	}
	pDC->Rectangle(_startPoint.x, _startPoint.y, _curPoint.x, _curPoint.y);// 画新位置
	_oldPoint = _curPoint;// 更新旧点
	// 恢复画笔
	pDC->SelectObject(_oldPen);
}

void CMFCSTARTView::DrawCircle(bool xorMode)
{
	CDC* pDC = GetDC();
	CPen pen;
	COLORREF color;
	color = RGB(255 - R, 255 - G, 255 - B);
	pen.CreatePen(PenStyle, LineWidth, color);
	_oldPen = pDC->SelectObject(&pen);
	pDC->SetROP2(R2_XORPEN);// 设置绘画模式为异或
	pDC->SelectStockObject(NULL_BRUSH);// 设置透明刷子
	pDC->Ellipse(_startPoint.x, _startPoint.y, _oldPoint.x, _oldPoint.y);// 覆盖旧位置
	if (!xorMode) {
		pDC->SetROP2(R2_NOTCOPYPEN);
	}
	pDC->Ellipse(_startPoint.x, _startPoint.y, _curPoint.x, _curPoint.y);// 画新位置
	_oldPoint = _curPoint;// 更新旧点
	// 恢复画笔
	pDC->SelectObject(_oldPen);
}


// 保存成位图，网上找的方法。
bool CMFCSTARTView::SaveBmp(HBITMAP hBitmap, CString fileName)
{
	//设备描述表
	HDC hDC;
	//当前分辨率下每象素所占字节数
	int iBits;
	//位图中每象素所占字节数
	WORD wBitCount;
	//定义调色板大小， 位图中像素字节大小 ，位图文件大小 ， 写入文件字节数
	DWORD dwPaletteSize = 0, dwBmBitsSize = 0, dwDIBSize = 0, dwWritten = 0;
	//位图属性结构
	BITMAP Bitmap;
	//位图文件头结构
	BITMAPFILEHEADER bmfHdr;
	//位图信息头结构
	BITMAPINFOHEADER bi;
	//指向位图信息头结构  
	LPBITMAPINFOHEADER lpbi;
	//定义文件，分配内存句柄，调色板句柄
	HANDLE fh, hDib, hPal, hOldPal = NULL;

	//计算位图文件每个像素所占字节数
	hDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if (iBits <= 1) wBitCount = 1;
	else if (iBits <= 4) wBitCount = 4;
	else if (iBits <= 8) wBitCount = 8;
	else wBitCount = 24;

	GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrImportant = 0;
	bi.biClrUsed = 0;

	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;

	//为位图内容分配内存
	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;

	// 处理调色板  
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = ::GetDC(NULL);
		hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}

	// 获取该调色板下新的像素值
	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)
		+ dwPaletteSize, (BITMAPINFO*)lpbi, DIB_RGB_COLORS);

	//恢复调色板  
	if (hOldPal)
	{
		::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}

	//创建位图文件  
	fh = CreateFile(fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if (fh == INVALID_HANDLE_VALUE)   return FALSE;

	// 设置位图文件头
	bmfHdr.bfType = 0x4D42; // "BM"
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;
	// 写入位图文件头
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	// 写入位图文件其余内容
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
	//清除  
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);

	return TRUE;
}


void CMFCSTARTView::OnFileSaveAs()
{
	// TODO: 在此添加命令处理程序代码

	CString filePath; // 文件保存路径
	// 初始化CFileDialog
	TCHAR szFilter[] = _T("位图文件(*.bmp)||自定义文件(*.json)||");
	CFileDialog fileDlg(FALSE, _T("bmp"), nullptr, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);

	if (IDOK == fileDlg.DoModal())// 如果对话框打开成功
	{
		filePath = fileDlg.GetPathName();
		HDC hDC = GetDC()->m_hDC;//获取DC
		RECT rect;
		GetClientRect(&rect); //获取客户端大小
		HDC hDCMem = CreateCompatibleDC(hDC);//创建兼容DC
		HBITMAP hBitMap = CreateCompatibleBitmap(hDC, abs(rect.right - rect.left), abs(rect.bottom - rect.top));//创建兼容位图
		HBITMAP hOldMap = (HBITMAP)::SelectObject(hDCMem, hBitMap);//将位图选入DC,并保存返回值
		BitBlt(hDCMem, 0, 0, abs(rect.right - rect.left), abs(rect.bottom - rect.top), hDC, 0, 0, SRCCOPY);//将屏幕DC的图象复制到内存DC中
		if (SaveBmp(hBitMap, filePath.GetBuffer())) {// 保存成功
			MessageBox(_T("保存成功"));
		}
		else // 保存失败
		{
			MessageBox(_T("保存失败"));
		}
	}
}


void CMFCSTARTView::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	CString filePath; // 文件保存路径
	// 初始化CFileDialog
	TCHAR szFilter[] = _T("位图文件(*.bmp)||自定义文件(*.json)||");
	CFileDialog fileDlg(true, _T("bmp"), nullptr, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);

	if (IDOK == fileDlg.DoModal())// 如果对话框打开成功
	{
		CBitmap mybitmap;
		filePath = fileDlg.GetPathName();
		HBITMAP bitmap = (HBITMAP)LoadImage(NULL, filePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
		mybitmap.Attach(bitmap);

		CDC* pdc = GetDC();
		CDC bmp;
		bmp.CreateCompatibleDC(pdc); //创建一个兼容pdc的设备上下文
		bmp.SelectObject(&mybitmap); //替换设备环境位图

		RECT rect;
		GetClientRect(&rect); //获取客户端大小
		pdc->BitBlt(0, 0, abs(rect.right - rect.left), abs(rect.bottom - rect.top), &bmp, 0, 0, SRCCOPY); //复制位图至pdc 也就是主窗口
		mybitmap.DeleteObject();//释放掉对象
		ReleaseDC(pdc); //释放掉设备上下文
	}
}
