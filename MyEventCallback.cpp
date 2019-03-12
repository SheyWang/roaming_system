#include "MyEventCallback.h"


extern bool gSnow;   //定义在MyLabelMenu.cpp文件
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

//初始化类中成员变量
void CMyEventCallback::init(osg::Node* node, osg::NodeVisitor* nv)
{
	//得到整个场景的根节点
	m_rootNode = dynamic_cast<osg::Group*>(node);  

	//得到整个场景的摄像机节点
	osg::ref_ptr<osgGA::EventVisitor> ev = dynamic_cast<osgGA::EventVisitor*>(nv);
	osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(ev->getActionAdapter());
	m_camera = dynamic_cast<CMyCamera*>(viewer->getCameraManipulator()); 

	//得到窗口信息
	vector<osgViewer::GraphicsWindow*> vgw;
	viewer->getWindows(vgw);
	m_gw = vgw[0];

	//得到屏幕分辨率
	osg::GraphicsContext::WindowingSystemInterface *wsi = osg::GraphicsContext::getWindowingSystemInterface();
	wsi->getScreenResolution(*(m_gw->getTraits()), m_screenWidth, m_screenHeight);

	//初始化窗口模式时 窗口的位置和宽、高
	m_screenWindowWidth = m_screenWidth*0.75;
	m_screenWindowHeight = m_screenHeight*0.75;
	m_screenX = (m_screenWidth - m_screenWindowWidth)/2;
	m_screenY = (m_screenHeight - m_screenWindowHeight)/2;

	//设置屏幕窗口大小
	m_gw->setWindowRectangle(m_screenX, m_screenY, m_screenWindowWidth, m_screenWindowHeight);

	//节点访问器
	CMyNodeVisitor nodeVisitor; 

	//查找并获取汽车节点
	nodeVisitor.setNameToFind(gPlayCarName);
	m_rootNode->accept(nodeVisitor);           
	m_playCar = dynamic_cast<osg::PositionAttitudeTransform*>(nodeVisitor.getFindNode());

	//查找并获取雨、雪节点
	nodeVisitor.setNameToFind(gPrecipitationEffectName);
	m_rootNode->accept(nodeVisitor); 
	m_precipitationEffect = dynamic_cast<osgParticle::PrecipitationEffect*>(nodeVisitor.getFindNode());

	//查找并获取全局光源节点
	nodeVisitor.setNameToFind(gSceneLight);
	m_rootNode->accept(nodeVisitor);   
	m_sceneLight = dynamic_cast<osg::LightSource*>(nodeVisitor.getFindNode())->getLight();

	//查找并获取汽车车灯
	nodeVisitor.setNameToFind(gLeftSpotLampName);
	m_rootNode->accept(nodeVisitor);  
	m_SpotLamp = dynamic_cast<osg::PositionAttitudeTransform*>(nodeVisitor.getFindNode());

	//查找并获取方向显示器节点
	nodeVisitor.setNameToFind(gCompassName);
	m_rootNode->accept(nodeVisitor);    
	m_compass = dynamic_cast<osg::PositionAttitudeTransform*>(nodeVisitor.getFindNode());

	//查找并获取方向图 节点
	nodeVisitor.setNameToFind(gCompassNodeName);
	m_rootNode->accept(nodeVisitor);   
	m_compassNode = dynamic_cast<osg::PositionAttitudeTransform*>(nodeVisitor.getFindNode());

	//查找并获取驾驶速度文本节点
	nodeVisitor.setNameToFind(gSpeedTextName);
	m_rootNode->accept(nodeVisitor);
	m_speedText = dynamic_cast<osgText::Text*>(dynamic_cast<osg::Geode*>(nodeVisitor.getFindNode())->getDrawable(0));
	//查找并获取显示驾驶速度的位置节点
	nodeVisitor.setNameToFind(gSpeedNodeName);
	m_rootNode->accept(nodeVisitor);  
	m_speedNode = dynamic_cast<osg::PositionAttitudeTransform*>(nodeVisitor.getFindNode());

    //查找并获取驾驶地图节点
	nodeVisitor.setNameToFind(gMapName);
	m_rootNode->accept(nodeVisitor);  
	m_map = dynamic_cast<osg::PositionAttitudeTransform*>(nodeVisitor.getFindNode());

	//查找并获取显示帮助信息的节点
	nodeVisitor.setNameToFind(gHelpNodeName);
	m_rootNode->accept(nodeVisitor);
	m_helpNode = dynamic_cast<osg::PositionAttitudeTransform*>(nodeVisitor.getFindNode());

	//得到雾效状态
	m_fog = dynamic_cast<osg::Fog*>(m_rootNode->getOrCreateStateSet()->getAttribute(osg::StateAttribute::FOG));

}




void CMyEventCallback::operator ()(osg::Node* node, osg::NodeVisitor* nv)
{
	static bool isInit = false;
	if (!isInit)                 //在程序最开始运行的时候初始化类中成员变量
	{
		init(node, nv);
		isInit = true;
	}

	if (osg::NodeVisitor::EVENT_VISITOR == nv->getVisitorType()) //判断访问器类型
	{
		//创建事件访问器并初始化
		osg::ref_ptr<osgGA::EventVisitor> ev = dynamic_cast<osgGA::EventVisitor*>(nv);
		if (ev)
		{
			osgGA::GUIActionAdapter* aa = ev->getActionAdapter();  //得到执行动作
			osgGA::EventQueue::Events& events = ev->getEvents();   //得到事件队列
			//遍历整个事件队列并处理每一个事件
			for (osgGA::EventQueue::Events::iterator itr = events.begin(); itr != events.end(); ++itr)
			{
				osgGA::GUIEventAdapter* ea = (*itr)->asGUIEventAdapter();
				handle(*(ea), *(aa));
			}
		}
	}

	updateScene(); //每帧都需要更新场景数据
}


//处理每一个事件
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


//更新场景数据
void CMyEventCallback::updateScene()
{
	processMenu();

	//更新汽车位置
	m_position = m_playCar->getPosition();
	changePosition(osg::Vec3d(-m_speed*sin(m_rotatef),  m_speed*cos(m_rotatef), 0.0));
	m_playCar->setPosition(m_position); 

	//更新车灯位置
	m_SpotLamp->setPosition(m_position);

	//更新摄像机位置
	if (m_theThreeView) //判断是否处于第三人称视角
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

	//更新速度显示器位置
	m_speedNode->setPosition(m_position);

	//更新方向显示器位置
	m_compassNode->setPosition(m_position);

	//更新帮助节点的位置
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
		m_helpNode->setPosition(osg::Vec3(0.0f, 0.0f, -1000.0f)); //此时把该节点的位置设置到视线外
	}

}


//碰撞检测，判断汽车是否能够移动
void CMyEventCallback::changePosition(osg::Vec3d delta)
{
	if (m_speed > -0.05f && m_speed < 0.05f)  
		return ;

	if (m_speed < 0.0f)  //倒车时的碰撞检测没有实现
	{
		m_position += delta;
		return ;
	}

	
	osg::BoundingSphere bs = m_playCar->getBound();
	float radius = bs.radius()/10;//此处包围盒的半径并不会随着pat节点的放大、缩小而变化，是固定的
	float downRotatef = m_rotatef+osg::PI_4/3;
	float upRotatef = m_rotatef-osg::PI_4/3;

	osg::Vec3 midStart;   //中间的直线段的起点
	osg::Vec3 downStart;  //下面的直线段的起点
	osg::Vec3 upStart;    //上面的直线段的起点

	//	if (m_speed > 0.0f)
	//	{
	midStart = osg::Vec3(bs.center().x()-radius*sin(m_rotatef),  
		bs.center().y()+radius*cos(m_rotatef), bs.center().z());

	downStart = osg::Vec3(bs.center().x()-radius*sin(downRotatef),  
		bs.center().y()+radius*cos(downRotatef), bs.center().z());

	upStart = osg::Vec3(bs.center().x()-radius*sin(upRotatef),  
		bs.center().y()+radius*cos(upRotatef), bs.center().z());
	//}
	//else  //倒车时候的碰撞检测，被注释了
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
		//只要有任何一条线段发生了碰撞，即判定汽车与场景碰撞，此时汽车无法前行
	}
	else
	{
		m_position += delta;
	}
	
}

//处理雾效
bool CMyEventCallback::processFog()
{
	switch(m_key)
	{
	case '1':
		m_fog->setColor(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f)); //雾的颜色设置为纯白色
		return true;

	case '2':
		m_fog->setColor(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f)); //雾的颜色设置为纯黑色
		return true;

	case '3':
		m_fog->setColor(osg::Vec4(0.5f, 0.5f, 0.5f, 1.0f));  //雾的颜色设置为灰色
		return true;

	case '0':
		if (m_fogFlag = !m_fogFlag)  //开启/关闭雾效
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

//处理整个驾驶地图
bool CMyEventCallback::processMap()
{
	static bool isFullScreen = false;

	switch(m_key)
	{
	case osgGA::GUIEventAdapter::KEY_F1:
		if(m_DisplayHelp = !m_DisplayHelp)
		{
			gHelp = true;  
			m_theThreeView = true;  // 当需要显示帮助时，驾驶视角将处于第三人称角度
		}
		else
		{
			gHelp = false;
		}
		return true;

	case osgGA::GUIEventAdapter::KEY_F3:   //全屏/窗口 模式切换
		if (isFullScreen = !isFullScreen)
		{
			m_gw->setWindowDecoration(false); //隐藏标题栏
			m_gw->setWindowRectangle(0, 0, m_screenWidth, m_screenHeight);
		}
		else
		{
			m_gw->setWindowDecoration(true);  //显示标题栏
			m_gw->setWindowRectangle(m_screenX, m_screenY, m_screenWindowWidth, m_screenWindowHeight);
		}
		return true;

	case osgGA::GUIEventAdapter::KEY_F4:
		//m_playCar->setPosition(osg::Vec3(-60.0, -800.0, 0.0)); //汽车的位置重置到原点
		m_playCar->setPosition(osg::Vec3(0.0, 0.0, 0.0)); //汽车的位置重置到原点
		return true;

	default:
		return false;
	}

	return false;
}


//控制汽车移动
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

	 //如果汽车的旋转角度发生了变化，更新汽车、车灯、速度显示器、方向显示器、摄像机的旋转角度
	if (changeRotatef) 
	{
		osg::Quat quat = osg::Quat(m_rotatef - osg::PI_2, osg::Z_AXIS);
		m_playCar->setAttitude(quat);  
		m_SpotLamp->setAttitude(quat);

		m_speedNode->setAttitude(osg::Quat(m_rotatef, osg::Z_AXIS));
		m_compass->setAttitude(osg::Quat(m_rotatef, osg::Z_AXIS));
		m_compassNode->setAttitude(osg::Quat(m_rotatef, osg::Z_AXIS));

		m_camera->setRotatefZ(m_rotatef);

		//设置汽车风向与行驶方向相等
		m_precipitationEffect->setWind(osg::Vec3d(-m_speed*sin(m_rotatef),  m_speed*cos(m_rotatef), 0.0));
	}

	//如果汽车的速度发生了变化，更新速度显示器的内容
	if (changeSpeed)
	{
		float speed = ((int)(m_speed*3600*30/1000))/20 ;
		char text[12];
		sprintf_s(text, "%.1f", speed);

		string label(text);
		label += " km/h";

		m_speedText->setText(label);

		//设置粒子风向与汽车行驶方向相等
		m_precipitationEffect->setWind(osg::Vec3d(-m_speed*sin(m_rotatef),  m_speed*cos(m_rotatef), 0.0));
	}
	return false;
}


//控制雨雪效果
bool CMyEventCallback::processPrecipitationEffect()
{
	switch(m_key)
	{
	case '4'://雨天
		m_precipitationEffect->rain(0.8f);
		return true;

	case '5'://不下雨了
		m_precipitationEffect->rain(0.0f);
		return true;

	case '6'://雪天
		m_precipitationEffect->snow(0.8f);
		m_precipitationEffect->setCellSize(osg::Vec3(2.0f, 2.0f, 2.0f));
		return true;

	case '7'://不下雪了
		m_precipitationEffect->snow(0.0f);
		return true;

	default:
		return false;
	}

	return false;
}


//控制车灯
bool CMyEventCallback::processSpotLamp()
{
	static bool isOpen = true;

	switch(m_key)
	{
	case osgGA::GUIEventAdapter::KEY_F2:
		if (isOpen = !isOpen) //车灯是否开启/关闭
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
		if (isNight = !isNight) //控制场景为白天/黑夜
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

	if (gSnow && gRain)  //雪
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

	if (gFog) //雾
	{
		m_rootNode->getOrCreateStateSet()->setMode(GL_FOG, osg::StateAttribute::ON);
	}
	else
	{
		m_rootNode->getOrCreateStateSet()->setMode(GL_FOG, osg::StateAttribute::OFF);
	}

	if (gNight) //夜晚
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

	if (gHelp)  //帮助
	{
		m_DisplayHelp = true;
		m_theThreeView = true;
	}
	else
	{
		m_DisplayHelp = false;
	}
	
	if (gFullScreen) //是否全屏
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