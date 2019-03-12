#pragma once

#include "Include.h"



class CMyLabel : public osgWidget::Label
{
public:
	CMyLabel(const char *label);
	~CMyLabel();

	bool mousePush(double, double, const osgWidget::WindowManager*);
	bool mouseEnter(double, double, const osgWidget::WindowManager*);
	bool mouseLeave(double, double,const osgWidget::WindowManager*);

private:

};



class CMyLabelMenu : public CMyLabel
{
public:
	CMyLabelMenu(const char *label);
	~CMyLabelMenu();

	void managed(osgWidget::WindowManager* wm);

	void positioned();

	bool mousePush(double, double, const osgWidget::WindowManager*);

private:

	osg::ref_ptr<osgWidget::Table> m_window;

};