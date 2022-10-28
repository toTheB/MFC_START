// DrawToolsDlg.cpp: 实现文件
//

#include "pch.h"
#include "MFC_START.h"
#include "afxdialogex.h"
#include "DrawToolsDlg.h"

#include <string>

#include "MFC_STARTView.h"
using namespace std;

// DrawToolsDlg 对话框

IMPLEMENT_DYNAMIC(DrawToolsDlg, CDialog)

DrawToolsDlg::DrawToolsDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DRAWTOOLSDLG, pParent)
{
	m_pParentWnd = pParent;
}

DrawToolsDlg::~DrawToolsDlg()
{
}

void DrawToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TOOLSCOMBO, _toolsCombo);
	DDX_Control(pDX, IDC_LineWidthEdit, _lineWidthEdit);
	DDX_Control(pDX, IDC_PenStyleCombo, _penStyleCombo);
}


BEGIN_MESSAGE_MAP(DrawToolsDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_TOOLSCOMBO, &DrawToolsDlg::OnCbnSelchangeToolscombo)
	ON_EN_CHANGE(IDC_LineWidthEdit, &DrawToolsDlg::OnEnChangeLinewidthedit)
	ON_CBN_SELCHANGE(IDC_PenStyleCombo, &DrawToolsDlg::OnCbnSelchangePenstylecombo)
END_MESSAGE_MAP()


// DrawToolsDlg 消息处理程序

BOOL DrawToolsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化、

	// 给绘图模式Combo添加选项
	_toolsCombo.AddString(_T("点"));
	_toolsCombo.AddString(_T("直线"));
	_toolsCombo.AddString(_T("矩形"));
	_toolsCombo.AddString(_T("圆"));
	_toolsCombo.AddString(_T("多边形"));
	_toolsCombo.AddString(_T("曲线"));

	// 给线型Combo添加选项
	_penStyleCombo.AddString(_T("SOLID"));
	_penStyleCombo.AddString(_T("DASH"));
	_penStyleCombo.AddString(_T("DOT"));
	_penStyleCombo.AddString(_T("DASHDOT"));
	_penStyleCombo.AddString(_T("DASHDOTDOT"));
	_penStyleCombo.AddString(_T("NULL"));

	return TRUE; // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


// 设置绘图模式的 combo box 响应函数
void DrawToolsDlg::OnCbnSelchangeToolscombo()
{
	// TODO: 在此添加控件通知处理程序代码

	// 通过响应函数改变绘图模式
	auto parentWnd = dynamic_cast<CMFCSTARTView*>(m_pParentWnd);
	parentWnd->DrawTask = _toolsCombo.GetCurSel();
}

// 线型选择 combo box 响应函数
void DrawToolsDlg::OnCbnSelchangePenstylecombo()
{
	// TODO: 在此添加控件通知处理程序代码

	// 通过响应函数改变线型
	auto parentWnd = dynamic_cast<CMFCSTARTView*>(m_pParentWnd);
	parentWnd->PenStyle = _penStyleCombo.GetCurSel();
}

// 线宽 Edit 响应函数
void DrawToolsDlg::OnEnChangeLinewidthedit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	// 获取Edit中的字符串
	CString str;
	_lineWidthEdit.GetWindowTextW(str);
	auto parentWnd = dynamic_cast<CMFCSTARTView*>(m_pParentWnd);
	// 将其赋值给线宽
	parentWnd->LineWidth = std::stoi(str.GetBuffer(0));
}
