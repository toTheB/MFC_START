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
	CComboBox _toolsCombo; // 绘画工具
	CEdit _lineWidthEdit; // 线宽
	CComboBox _penStyleCombo; // 线型
	CEdit _colorEdit; // 颜色

	virtual BOOL OnInitDialog();

	// 消息响应函数
	afx_msg void OnCbnSelchangeToolscombo();
	afx_msg void OnEnChangeLinewidthedit();
	afx_msg void OnCbnSelchangePenstylecombo();
	afx_msg void OnEnChangeColoredit();
};
