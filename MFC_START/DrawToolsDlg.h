#pragma once
#include <vector>

#include "afxdialogex.h"


// DrawToolsDlg 对话框

class DrawToolsDlg : public CDialog
{
	DECLARE_DYNAMIC(DrawToolsDlg)

public:
	DrawToolsDlg(CWnd* pParent = nullptr); // 标准构造函数
	virtual ~DrawToolsDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DRAWTOOLSDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	// 选择绘画工具
	CComboBox _toolsCombo;
	afx_msg void OnCbnSelchangeToolscombo();
	afx_msg void OnEnChangeLinewidthedit();
	CEdit _lineWidthEdit;

	afx_msg void OnCbnSelchangePenstylecombo();
	CComboBox _penStyleCombo;
};
