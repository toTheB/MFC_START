// DrawToolsDlg.cpp: 实现文件
//

#include "pch.h"
#include "MFC_START.h"
#include "afxdialogex.h"
#include "DrawToolsDlg.h"
#include "MFC_STARTView.h"

// DrawToolsDlg 对话框

IMPLEMENT_DYNAMIC(DrawToolsDlg, CDialog)

DrawToolsDlg::DrawToolsDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DRAWTOOLSDLG, pParent)
{

}

DrawToolsDlg::~DrawToolsDlg()
{
}

void DrawToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TOOLSCOMBO, _toolsCombo);
}


BEGIN_MESSAGE_MAP(DrawToolsDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_TOOLSCOMBO, &DrawToolsDlg::OnCbnSelchangeToolscombo)
END_MESSAGE_MAP()


// DrawToolsDlg 消息处理程序


void DrawToolsDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
}


BOOL DrawToolsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	_toolsCombo.AddString(_T("点"));
	_toolsCombo.AddString(_T("直线"));
	_toolsCombo.AddString(_T("矩形"));
	_toolsCombo.AddString(_T("圆"));
	_toolsCombo.AddString(_T("多边形"));
	_toolsCombo.AddString(_T("曲线"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void DrawToolsDlg::OnCbnSelchangeToolscombo()
{
	// TODO: 在此添加控件通知处理程序代码
	CMFCSTARTView* parentWnd = dynamic_cast<CMFCSTARTView*>(m_pParentWnd);
	parentWnd->DrawTask = _toolsCombo.GetCurSel();
}
