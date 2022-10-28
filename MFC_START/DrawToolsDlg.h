#pragma once
#include "afxdialogex.h"


// DrawToolsDlg 对话框

class DrawToolsDlg : public CDialog
{
	DECLARE_DYNAMIC(DrawToolsDlg)

public:
	DrawToolsDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~DrawToolsDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DRAWTOOLSDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeCombo1();
	virtual BOOL OnInitDialog();
	// 选择绘画工具
	CComboBox _toolsCombo;
	afx_msg void OnCbnSelchangeToolscombo();
};
