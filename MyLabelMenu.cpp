#include "MyLabelMenu.h"

bool gSnow = false;
bool gFog  = false;
bool gHelp = false;
bool gRain = false;
bool gNight = false;
bool gFullScreen = false;
bool gMousePush = false;

//////////////////////////////////////////////////////////////////////////
//
//                   class CMyLabel
//
//////////////////////////////////////////////////////////////////////////



CMyLabel::CMyLabel(const char *label):osgWidget::Label(label, "")
{
	setlocale(LC_ALL, ".936");
	setFont("fonts/simhei.ttf");
	setFontSize(14);
	setFontColor(1.0f, 1.0f, 1.0f, 1.0f);
	setColor(0.3f, 0.3f, 0.3f, 1.0f);
	setPadding(2.0f);
	setCanFill(true);

	addSize(150.0f, 25.0f);

	int requiredSize = mbstowcs(NULL, label, 0);
	wchar_t *wtext = new wchar_t[requiredSize+1];
	mbstowcs(wtext, label, requiredSize+1);
	setLabel(wtext);
	delete wtext;

	setEventMask(osgWidget::EVENT_MOUSE_PUSH | osgWidget::EVENT_MASK_MOUSE_MOVE);
}

CMyLabel::~CMyLabel()
{

}

bool CMyLabel::mousePush(double, double, const osgWidget::WindowManager*)
{
	osgWidget::Table *p = dynamic_cast<osgWidget::Table*>(_parent);
	if (!p)
	{
		return false;
	}

	const std::string& name = getName();

	gMousePush = true;

	if (!name.compare("ȫ��/����"))
	{
		gFullScreen = !gFullScreen;
	}
	else if (!name.compare("����"))
	{
		gHelp = !gHelp;
	}
	else if (!name.compare("����"))
	{
		gSnow = false;
		gRain = !gRain;
	}
	else if (!name.compare("����"))
	{
		gFog = !gFog;
	}
	else if (!name.compare("ҹ��"))
	{
		gNight = !gNight;
	}
	else if (!name.compare("ѩ��"))
	{
		gRain = false;
		gSnow = !gSnow;
	}
	else if (!name.compare("����ģ��"))
	{
		
	}
	else if (!name.compare("��Ч"))
	{

	}
	else if (!name.compare("������Ⱦ"))
	{

	}
	else if (!name.compare("�����"))
	{

	}
	else if (!name.compare("����ϵͳ"))
	{

	}
	else if (!name.compare("��Ч"))
	{

	}
	else
	{
		return true;
	}


	return true;
}

bool CMyLabel::mouseEnter(double, double, const osgWidget::WindowManager*)
{
	setColor(0.9f, 0.6f, 0.1f, 1.0f);
	return true;
}

bool CMyLabel::mouseLeave(double, double, const osgWidget::WindowManager*)
{
	setColor(0.3f, 0.3f, 0.3f, 1.0f);
	return true;
}





//////////////////////////////////////////////////////////////////////////
//
//                   class CMyLabelMenu
//
//////////////////////////////////////////////////////////////////////////

CMyLabelMenu::CMyLabelMenu(const char *label) : CMyLabel(label)
{
	m_window = new osgWidget::Table(std::string("Menu_")+label, 12, 1);

	m_window->addWidget(new CMyLabel("ȫ��/����"), 0, 0);
	m_window->addWidget(new CMyLabel("����"), 1, 0);
	m_window->addWidget(new CMyLabel("����"), 2, 0);
	m_window->addWidget(new CMyLabel("����"), 3, 0);
	m_window->addWidget(new CMyLabel("ҹ��"), 4, 0);
	m_window->addWidget(new CMyLabel("ѩ��"), 5, 0);
	m_window->addWidget(new CMyLabel("����ģ��"), 6, 0);
	m_window->addWidget(new CMyLabel("��Ч"), 7, 0);
	m_window->addWidget(new CMyLabel("������Ⱦ"), 8, 0);
	m_window->addWidget(new CMyLabel("�����"), 9, 0);
	m_window->addWidget(new CMyLabel("����ϵͳ"), 10, 0);
	m_window->addWidget(new CMyLabel("��Ч"), 11, 0);

	m_window->resize();
}

CMyLabelMenu::~CMyLabelMenu()
{

}


void CMyLabelMenu::managed(osgWidget::WindowManager *wm)
{
	osgWidget::Label::managed(wm);

	wm->addChild(m_window.get());

	m_window->hide();
}

void CMyLabelMenu::positioned()
{
	osgWidget::Label::positioned();

	m_window->setOrigin(_parent->getX(), _parent->getY() + _parent->getHeight()); //��������Щ�ط�����ȫ�����
}

bool CMyLabelMenu::mousePush(double, double, const osgWidget::WindowManager*)
{
	if (!m_window->isVisible())
	{
		m_window->show();
	}
	else
	{
		m_window->hide();
	}
	
	return true;
}

