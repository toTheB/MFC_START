// MFC_STARTView.h: CMFCSTARTView 类的接口
//

#pragma once
#include "DrawToolsDlg.h"
#include "MFC_STARTDoc.h"


class CMFCSTARTView : public CView
{
protected: // 仅从序列化创建
	CMFCSTARTView() noexcept;
	DECLARE_DYNCREATE(CMFCSTARTView)

	// 特性
public:
	CMFCSTARTDoc* GetDocument() const;

	// 操作
public:
	// 重写
public:
	virtual void OnDraw(CDC* pDC); // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// 实现
public:
	virtual ~CMFCSTARTView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

	// 消息处理
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnFileSaveAs();
	void OpenDTDlgCmd();

	// 私有成员变量
protected:
	CPen* _oldPen; // 旧笔
	CPoint _polyStartPoint; // 多边形起点
	CPoint _startPoint; // 绘图起点
	CPoint _oldPoint; // 旧的鼠标位置（XOR覆盖用）
	CPoint _curPoint; // 当前鼠标位置（绘图用）
	bool _mouseDown; // 鼠标左键是否按下
	bool _drawingPoly; // 记录是否正在绘制多边形的过程中，因为多边形绘制时鼠标的操作与其他模式不同。
	bool _endingPoly; // 记录是否闭合一个多边形，OnDraw中判断用
	DrawToolsDlg* _toolDlg; // 绘图工具子对话框
	CString _savePath; // 文件保存路径

	// 共有成员变量
public:
	int DrawTask; // 绘画模式
	int LineWidth; // 线宽
	int PenStyle; // 线型
	COLORREF PenColor; // 颜色

	// 成员函数
protected:
	void DrawDot(); // 点
	void DrawLine(); // 线
	void DrawRec(); // 矩形
	void DrawCircle(); // 圆
	bool SaveBmp(HBITMAP hBitmap, CString fileName);

	//常量
public:
	static const int DRAW_DOT = 0; // 点
	static const int DRAW_LINE = 1; // 线
	static const int DRAW_REC = 2; // 矩形
	static const int DRAW_CIRCLE = 3; // 圆
	static const int DRAW_POLY = 4;
	static const int DRAW_CURVE = 5;

};

#ifndef _DEBUG  // MFC_STARTView.cpp 中的调试版本
inline CMFCSTARTDoc* CMFCSTARTView::GetDocument() const
{
	return reinterpret_cast<CMFCSTARTDoc*>(m_pDocument);
}
#endif
