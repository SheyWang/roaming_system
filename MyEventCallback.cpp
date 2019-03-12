#include "MyEventCallback.h"


extern bool gSnow;   //������MyLabelMenu.cpp�ļ�
extern bool gFog;
extern bool gHelp;
extern bool gRain;
extern bool gNight;
extern bool gFullScreen;
extern bool gMousePush;


CMyEventCallback::CMyEventCallback()
{
	m_speed        = 0.0f;
	m_rotatef      = 0.0f;
	m_fogFlag      = false;
	m_theThreeView = false;
	m_DisplayHelp  = false;
}

CMyEventCallback::~CMyEventCallback()
{

}

//��ʼ�����г�Ա����
void CMyEventCallback::init(osg::Node* node, osg::NodeVisitor* nv)
{
	//�õ����������ĸ��ڵ�
	m_rootNode = dynamic_cast<osg::Group*>(node);  

	//�õ�����������������ڵ�
	osg::ref_ptr<osgGA::EventVisitor> ev = dynamic_cast<osgGA::EventVisitor*>(nv);
	osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(ev->getActionAdapter());
	m_camera = dynamic_cast<CMyCamera*>(viewer->getCameraManipulator()); 

	//�õ�������Ϣ
	vector<osgViewer::GraphicsWindow*> vgw;
	viewer->getWindows(vgw);
	m_gw = vgw[0];

	//�õ���Ļ�ֱ���
	osg::GraphicsContext::WindowingSystemInterface *wsi = osg::GraphicsContext::getWindowingSystemInterface();
	wsi->getScreenResolution(*(m_gw->getTraits()), m_screenWidth, m_screenHeight);

	//��ʼ������ģʽʱ ���ڵ�λ�úͿ���
	m_screenWindowWidth = m_screenWidth*0.75;
	m_screenWindowHeight = m_screenHeight*0.75;
	m_screenX = (m_screenWidth - m_screenWindowWidth)/2;
	m_screenY = (m_screenHeight - m_screenWindowHeight)/2;

	//������Ļ���ڴ�С
	m_gw->setWindowRectangle(m_screenX, m_screenY, m_screenWindowWidth, m_screenWindowHeight);

	//�ڵ������
	CMyNodeVisitor nodeVisitor; 

	//���Ҳ���ȡ�����ڵ�
	nodeVisitor.setNameToFind(gPlayCarName);
	m_rootNode->accept(nodeVisitor);           
	m_playCar = dynamic_cast<osg::PositionAttitudeTransform*>(nodeVisitor.getFindNode());

	//���Ҳ���ȡ�ꡢѩ�ڵ�
	nodeVisitor.setNameToFind(gPrecipitationEffectName);
	m_rootNode->accept(nodeVisitor); 
	m_precipitationEffect = dynamic_cast<osgParticle::PrecipitationEffect*>(nodeVisitor.getFindNode());

	//���Ҳ���ȡȫ�ֹ�Դ�ڵ�
	nodeVisitor.setNameToFind(gSceneLight);
	m_rootNode->accept(nodeVisitor);   
	m_sceneLight = dynamic_cast<osg::LightSource*>(nodeVisitor.getFindNode())->getLight();

	//���Ҳ���ȡ��������
	nodeVisitor.setNameToFind(gLeftSpotLampName);
	m_rootNode->accept(nodeVisitor);  
	m_SpotLamp = dynamic_cast<osg::PositionAttitudeTransform*>(nodeVisitor.getFindNode());

	//���Ҳ���ȡ������ʾ���ڵ�
	nodeVisitor.setNameToFind(gCompassName);
	m_rootNode->accept(nodeVisitor);    
	m_compass = dynamic_cast<osg::PositionAttitudeTransform*>(nodeVisitor.getFindNode());

	//���Ҳ���ȡ����ͼ �ڵ�
	nodeVisitor.setNameToFind(gCompassNodeName);
	m_rootNode->accept(nodeVisitor);   
	m_compassNode = dynamic_cast<osg::PositionAttitudeTransform*>(nodeVisitor.getFindNode());

	//���Ҳ���ȡ��ʻ�ٶ��ı��ڵ�
	nodeVisitor.setNameToFind(gSpeedTextName);
	m_rootNode->accept(nodeVisitor);
	m_speedText = dynamic_cast<osgText::Text*>(dynamic_cast<osg::Geode*>(nodeVisitor.getFindNode())->getDrawable(0));
	//���Ҳ���ȡ��ʾ��ʻ�ٶȵ�λ�ýڵ�
	nodeVisitor.setNameToFind(gSpeedNodeName);
	m_rootNode->accept(nodeVisitor);  
	m_speedNode = dynamic_cast<osg::PositionAttitudeTransform*>(nodeVisitor.getFindNode());

    //���Ҳ���ȡ��ʻ��ͼ�ڵ�
	nodeVisitor.setNameToFind(gMapName);
	m_rootNode->accept(nodeVisitor);  
	m_map = dynamic_cast<osg::PositionAttitudeTransform*>(nodeVisitor.getFindNode());

	//���Ҳ���ȡ��ʾ������Ϣ�Ľڵ�
	nodeVisitor.setNameToFind(gHelpNodeName);
	m_rootNode->accept(nodeVisitor);
	m_helpNode = dynamic_cast<osg::PositionAttitudeTransform*>(nodeVisitor.getFindNode());

	//�õ���Ч״̬
	m_fog = dynamic_cast<osg::Fog*>(m_rootNode->getOrCreateStateSet()->getAttribute(osg::StateAttribute::FOG));

}




void CMyEventCallback::operator ()(osg::Node* node, osg::NodeVisitor* nv)
{
	static bool isInit = false;
	if (!isInit)                 //�ڳ����ʼ���е�ʱ���ʼ�����г�Ա����
	{
		init(node, nv);
		isInit = true;
	}

	if (osg::NodeVisitor::EVENT_VISITOR == nv->getVisitorType()) //�жϷ���������
	{
		//�����¼�����������ʼ��
		osg::ref_ptr<osgGA::EventVisitor> ev = dynamic_cast<osgGA::EventVisitor*>(nv);
		if (ev)
		{
			osgGA::GUIActionAdapter* aa = ev->getActionAdapter();  //�õ�ִ�ж���
			osgGA::EventQueue::Events& events = ev->getEvents();   //�õ��¼�����
			//���������¼����в�����ÿһ���¼�
			for (osgGA::EventQueue::Events::iterator itr = events.begin(); itr != events.end(); ++itr)
			{
				osgGA::GUIEventAdapter* ea = (*itr)->asGUIEventAdapter();
				handle(*(ea), *(aa));
			}
		}
	}

	updateScene(); //ÿ֡����Ҫ���³�������
}


//����ÿһ���¼�
bool CMyEventCallback::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	switch(ea.getEventType())
	{
		case osgGA::GUIEventAdapter::KEYDOWN:
			{
				m_key = ea.getKey();

				if (processFog() 
					|| processPrecipitationEffect()
					|| processSceneLight()
					|| processSpotLamp()
					|| processMap())
				{
					break;
				}
				else if (processPlayCar())
				{
					break;
				}
				else
				{
					break;
				}

				m_key = -10000;
			}
			break;

		default:
			break;
	}

	return true;
}


//���³�������
void CMyEventCallback::updateScene()
{
	processMenu();

	//��������λ��
	m_position = m_playCar->getPosition();
	changePosition(osg::Vec3d(-m_speed*sin(m_rotatef),  m_speed*cos(m_rotatef), 0.0));
	m_playCar->setPosition(m_position); 

	//���³���λ��
	m_SpotLamp->setPosition(m_position);

	//���������λ��
	if (m_theThreeView) //�ж��Ƿ��ڵ����˳��ӽ�
	{
		//m_cameraPos = m_position + osg::Vec3f(0, -100, 35)*osg::Matrixd(osg::Quat(m_rotatef,osg::Z_AXIS)); 
		m_cameraPos = m_position + osg::Vec3f(0, -100, 35)*osg::Matrixd(osg::Quat(m_rotatef,osg::Z_AXIS)); 
	}
	else
	{
		//m_cameraPos = m_position + osg::Vec3f(-5, -2.2, 19)*osg::Matrixd(osg::Quat(m_rotatef,osg::Z_AXIS)); 
		m_cameraPos = m_position + osg::Vec3f(-5, -2.2, 19)*osg::Matrixd(osg::Quat(m_rotatef,osg::Z_AXIS)); 
	}
	m_camera->setPosition(m_cameraPos);

	//�����ٶ���ʾ��λ��
	m_speedNode->setPosition(m_position);

	//���·�����ʾ��λ��
	m_compassNode->setPosition(m_position);

	//���°����ڵ��λ��
	if (m_DisplayHelp)
	{
		osg::Quat quat(m_camera->getRotatefX() - osg::PI_2, osg::Vec3(1.0f, 0.0f, 0.0f), 
			m_camera->getRotatefY(), osg::Vec3(0.0f, 1.0f, 0.0f),
			m_camera->getRotatefZ(), osg::Vec3(0.0f, 0.0f, 1.0f));

		m_helpNode->setAttitude(quat);
		m_helpNode->setPosition(m_cameraPos + osg::Vec3(-15.0f, 40.0f, -7.0f)*osg::Matrixd(quat));
	}
	else
	{
		m_helpNode->setPosition(osg::Vec3(0.0f, 0.0f, -1000.0f)); //��ʱ�Ѹýڵ��λ�����õ�������
	}

}


//��ײ��⣬�ж������Ƿ��ܹ��ƶ�
void CMyEventCallback::changePosition(osg::Vec3d delta)
{
	if (m_speed > -0.05f && m_speed < 0.05f)  
		return ;

	if (m_speed < 0.0f)  //����ʱ����ײ���û��ʵ��
	{
		m_position += delta;
		return ;
	}

	
	osg::BoundingSphere bs = m_playCar->getBound();
	float radius = bs.radius()/10;//�˴���Χ�еİ뾶����������pat�ڵ�ķŴ���С���仯���ǹ̶���
	float downRotatef = m_rotatef+osg::PI_4/3;
	float upRotatef = m_rotatef-osg::PI_4/3;

	osg::Vec3 midStart;   //�м��ֱ�߶ε����
	osg::Vec3 downStart;  //�����ֱ�߶ε����
	osg::Vec3 upStart;    //�����ֱ�߶ε����

	//	if (m_speed > 0.0f)
	//	{
	midStart = osg::Vec3(bs.center().x()-radius*sin(m_rotatef),  
		bs.center().y()+radius*cos(m_rotatef), bs.center().z());

	downStart = osg::Vec3(bs.center().x()-radius*sin(downRotatef),  
		bs.center().y()+radius*cos(downRotatef), bs.center().z());

	upStart = osg::Vec3(bs.center().x()-radius*sin(upRotatef),  
		bs.center().y()+radius*cos(upRotatef), bs.center().z());
	//}
	//else  //����ʱ�����ײ��⣬��ע����
	//{
	//	midStart = osg::Vec3(bs.center().x()+radius*sin(m_rotatef),   
	//		bs.center().y()-radius*cos(m_rotatef), bs.center().z());

	//	downStart = osg::Vec3(bs.center().x()+radius*sin(downRotatef),  
	//		bs.center().y()-radius*cos(downRotatef), bs.center().z());

	//	upStart = osg::Vec3(bs.center().x()+radius*sin(upRotatef),  
	//		bs.center().y()-radius*cos(upRotatef), bs.center().z());
	//}


	osg::ref_ptr<osgUtil::LineSegmentIntersector> midIntersector = new osgUtil::LineSegmentIntersector
		(midStart, midStart + delta);
	osg::ref_ptr<osgUtil::LineSegmentIntersector> downIntersector = new osgUtil::LineSegmentIntersector
		(downStart, downStart + delta);
	osg::ref_ptr<osgUtil::LineSegmentIntersector> upIntersector = new osgUtil::LineSegmentIntersector
		(upStart, upStart + delta);

	osgUtil::IntersectionVisitor mindIntersectVisitor(midIntersector.get());
	osgUtil::IntersectionVisitor downIntersectVisitor(downIntersector.get());
	osgUtil::IntersectionVisitor upIntersectVisitor(upIntersector.get());

	m_rootNode->accept(mindIntersectVisitor);
	m_rootNode->accept(downIntersectVisitor);
	m_rootNode->accept(upIntersectVisitor);


	if (midIntersector->containsIntersections() || 
		downIntersector->containsIntersections() ||
		upIntersector->containsIntersections())
	{
		//ֻҪ���κ�һ���߶η�������ײ�����ж������볡����ײ����ʱ�����޷�ǰ��
	}
	else
	{
		m_position += delta;
	}
	
}

//������Ч
bool CMyEventCallback::processFog()
{
	switch(m_key)
	{
	case '1':
		m_fog->setColor(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f)); //�����ɫ����Ϊ����ɫ
		return true;

	case '2':
		m_fog->setColor(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f)); //�����ɫ����Ϊ����ɫ
		return true;

	case '3':
		m_fog->setColor(osg::Vec4(0.5f, 0.5f, 0.5f, 1.0f));  //�����ɫ����Ϊ��ɫ
		return true;

	case '0':
		if (m_fogFlag = !m_fogFlag)  //����/�ر���Ч
		{
			m_rootNode->getOrCreateStateSet()->setMode(GL_FOG, osg::StateAttribute::OFF);
		}
		else
		{
			m_rootNode->getOrCreateStateSet()->setMode(GL_FOG, osg::StateAttribute::ON);
		}
		return true;
	default:
		return false;
	}

	return false;
}

//����������ʻ��ͼ
bool CMyEventCallback::processMap()
{
	static bool isFullScreen = false;

	switch(m_key)
	{
	case osgGA::GUIEventAdapter::KEY_F1:
		if(m_DisplayHelp = !m_DisplayHelp)
		{
			gHelp = true;  
			m_theThreeView = true;  // ����Ҫ��ʾ����ʱ����ʻ�ӽǽ����ڵ����˳ƽǶ�
		}
		else
		{
			gHelp = false;
		}
		return true;

	case osgGA::GUIEventAdapter::KEY_F3:   //ȫ��/���� ģʽ�л�
		if (isFullScreen = !isFullScreen)
		{
			m_gw->setWindowDecoration(false); //���ر�����
			m_gw->setWindowRectangle(0, 0, m_screenWidth, m_screenHeight);
		}
		else
		{
			m_gw->setWindowDecoration(true);  //��ʾ������
			m_gw->setWindowRectangle(m_screenX, m_screenY, m_screenWindowWidth, m_screenWindowHeight);
		}
		return true;

	case osgGA::GUIEventAdapter::KEY_F4:
		//m_playCar->setPosition(osg::Vec3(-60.0, -800.0, 0.0)); //������λ�����õ�ԭ��
		m_playCar->setPosition(osg::Vec3(0.0, 0.0, 0.0)); //������λ�����õ�ԭ��
		return true;

	default:
		return false;
	}

	return false;
}


//���������ƶ�
bool CMyEventCallback::processPlayCar()
{
	bool changeSpeed = false;
	bool changeRotatef = false;

	unsigned int curtime = GetTickCount();

	switch(m_key)
	{
	case 'i':
		changeSpeed = true;
		m_speed += 0.2f;
		break;
	case 'k':
		changeSpeed = true;
		m_speed -= 0.2f;				
		break;
	case 'j':
		changeRotatef = true;
		m_rotatef += 0.02f;
		break;
	case 'l':
		changeRotatef = true;
		m_rotatef -= 0.02f;
		break;
	case 'o':
		changeSpeed = true;
		m_speed = 0.0f;
		break;
	case osgGA::GUIEventAdapter::KEY_Space:
		if (!(m_theThreeView = !m_theThreeView))
		{
			m_DisplayHelp = false;
		}
		break;

	default:
		break;
	}

	 //�����������ת�Ƕȷ����˱仯���������������ơ��ٶ���ʾ����������ʾ�������������ת�Ƕ�
	if (changeRotatef) 
	{
		osg::Quat quat = osg::Quat(m_rotatef - osg::PI_2, osg::Z_AXIS);
		m_playCar->setAttitude(quat);  
		m_SpotLamp->setAttitude(quat);

		m_speedNode->setAttitude(osg::Quat(m_rotatef, osg::Z_AXIS));
		m_compass->setAttitude(osg::Quat(m_rotatef, osg::Z_AXIS));
		m_compassNode->setAttitude(osg::Quat(m_rotatef, osg::Z_AXIS));

		m_camera->setRotatefZ(m_rotatef);

		//����������������ʻ�������
		m_precipitationEffect->setWind(osg::Vec3d(-m_speed*sin(m_rotatef),  m_speed*cos(m_rotatef), 0.0));
	}

	//����������ٶȷ����˱仯�������ٶ���ʾ��������
	if (changeSpeed)
	{
		float speed = ((int)(m_speed*3600*30/1000))/20 ;
		char text[12];
		sprintf_s(text, "%.1f", speed);

		string label(text);
		label += " km/h";

		m_speedText->setText(label);

		//�������ӷ�����������ʻ�������
		m_precipitationEffect->setWind(osg::Vec3d(-m_speed*sin(m_rotatef),  m_speed*cos(m_rotatef), 0.0));
	}
	return false;
}


//������ѩЧ��
bool CMyEventCallback::processPrecipitationEffect()
{
	switch(m_key)
	{
	case '4'://����
		m_precipitationEffect->rain(0.8f);
		return true;

	case '5'://��������
		m_precipitationEffect->rain(0.0f);
		return true;

	case '6'://ѩ��
		m_precipitationEffect->snow(0.8f);
		m_precipitationEffect->setCellSize(osg::Vec3(2.0f, 2.0f, 2.0f));
		return true;

	case '7'://����ѩ��
		m_precipitationEffect->snow(0.0f);
		return true;

	default:
		return false;
	}

	return false;
}


//���Ƴ���
bool CMyEventCallback::processSpotLamp()
{
	static bool isOpen = true;

	switch(m_key)
	{
	case osgGA::GUIEventAdapter::KEY_F2:
		if (isOpen = !isOpen) //�����Ƿ���/�ر�
		{
			m_rootNode->getOrCreateStateSet()->setMode(GL_LIGHT1, osg::StateAttribute::OFF);
		}
		else
		{
			m_rootNode->getOrCreateStateSet()->setMode(GL_LIGHT1, osg::StateAttribute::ON);
		}
		return true;

	default:
		return false;
	}
	return false;
}


bool CMyEventCallback::processSceneLight()
{
	static bool isNight = false;

	switch(m_key)
	{
	case '8':
		if (isNight = !isNight) //���Ƴ���Ϊ����/��ҹ
		{
			m_sceneLight->setAmbient(osg::Vec4(0.0f, 0.0f, 0.0f, 0.0f)); 
			m_sceneLight->setDiffuse(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
			m_sceneLight->setSpecular(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
		}
		else
		{
			m_sceneLight->setAmbient(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			m_sceneLight->setDiffuse(osg::Vec4(0.25f, 0.25f, 0.25f, 1.0f));
			m_sceneLight->setSpecular(osg::Vec4(0.1f, 0.1f, 0.1f, 1.0f));
		}
		return true;

	default:
		return false;
	}

	return false;
}


//
void CMyEventCallback::processMenu()
{
	if (!gMousePush)
		return ;
	gMousePush = false;

	if (gSnow && gRain)  //ѩ
	{
		m_precipitationEffect->snow(0.5f);
		m_precipitationEffect->rain(0.5f);
	}
	else if (gSnow && !gRain)
	{
		m_precipitationEffect->rain(0.001f);
		m_precipitationEffect->snow(0.5f);
	}
	else if (!gSnow && gRain)
	{
		m_precipitationEffect->snow(0.001f);
		m_precipitationEffect->rain(0.5f);
	}
	else 
	{
		m_precipitationEffect->rain(0.0f);
		m_precipitationEffect->snow(0.0f);
	}

	if (gFog) //��
	{
		m_rootNode->getOrCreateStateSet()->setMode(GL_FOG, osg::StateAttribute::ON);
	}
	else
	{
		m_rootNode->getOrCreateStateSet()->setMode(GL_FOG, osg::StateAttribute::OFF);
	}

	if (gNight) //ҹ��
	{
		m_sceneLight->setAmbient(osg::Vec4(0.0f, 0.0f, 0.0f, 0.0f)); 
		m_sceneLight->setDiffuse(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
		m_sceneLight->setSpecular(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
	}
	else
	{
		m_sceneLight->setAmbient(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		m_sceneLight->setDiffuse(osg::Vec4(0.25f, 0.25f, 0.25f, 1.0f));
	}

	if (gHelp)  //����
	{
		m_DisplayHelp = true;
		m_theThreeView = true;
	}
	else
	{
		m_DisplayHelp = false;
	}
	
	if (gFullScreen) //�Ƿ�ȫ��
	{
		m_gw->setWindowDecoration(false);
		m_gw->setWindowRectangle(0, 0, m_screenWidth, m_screenHeight);
	}
	else
	{
		m_gw->setWindowDecoration(true);
		m_gw->setWindowRectangle(m_screenX, m_screenY, m_screenWindowWidth, m_screenWindowHeight);
	}
}