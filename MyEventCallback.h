#include "Include.h"
#include "NodeVisitor.h"
#include "MyCamera.h"


class CMyEventCallback : public osg::NodeCallback
{
public:
	CMyEventCallback();
	~CMyEventCallback();

	void init(osg::Node* node, osg::NodeVisitor* nv);    //初始化类中成员变量

                void operator()(osg::Node* node, osg::NodeVisitor* nv);
    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

	void updateScene();                //更新场景数据

	bool processFog();                 //控制雾效
	bool processPlayCar();             //控制汽车行驶
	bool processPrecipitationEffect(); //控制雨雪效果
	bool processSceneLight();          //控制全局光源的各项参数
	bool processSpotLamp();            //控制车灯参数
	bool processMap();                 //控制地图  

	void processMenu();                //处理鼠标点击菜单

	void changePosition(osg::Vec3d delta); //碰撞检测，判断汽车是否能够前进

private:
	//场景的根节点
	osg::Group          *m_rootNode; 

	//当前的键盘按键的键值
	int                  m_key;

	//用于控制整个窗口
	osgViewer::GraphicsWindow *m_gw;

    //全屏的大小
	unsigned int        m_screenWidth;
	unsigned int        m_screenHeight;

	//非全屏时，程序窗口的起始位置和宽、高
    unsigned int        m_screenX;
	unsigned int        m_screenY;
	unsigned int        m_screenWindowWidth;
	unsigned int        m_screenWindowHeight;

	//场景中的摄像机
	CMyCamera           *m_camera;   

	//雾效开启/关闭标志 以及 雾效状态
	bool                 m_fogFlag;  
	osg::Fog            *m_fog;      

	//雨雪节点
	osgParticle::PrecipitationEffect *m_precipitationEffect; 

	//整个场景的全局光源
	osg::Light          *m_sceneLight; 

	//显示汽车行驶方向的 方向显示器节点
	osg::PositionAttitudeTransform  *m_compass; 
	osg::PositionAttitudeTransform  *m_compassNode;

	//显示汽车行驶速度的 文本节点
	osgText::Text                   *m_speedText;
	osg::PositionAttitudeTransform  *m_speedNode;

	//车灯
	osg::PositionAttitudeTransform  *m_SpotLamp;

	//整个驾驶地图
	osg::PositionAttitudeTransform  *m_map;

	//当前汽车在场景的位置 以及 摄像机的位置
	osg::Vec3                        m_position;
	osg::Vec3                        m_cameraPos;

	//汽车的行驶速度 以及 汽车绕整个场景Z轴的旋转角度
	float                            m_speed;
	float                            m_rotatef;

	//是否处于第三人称视角驾驶汽车
	bool                             m_theThreeView;

	//用于控制汽车行驶的节点
	osg::PositionAttitudeTransform  *m_playCar;

	//用于显示帮助菜单
	osg::PositionAttitudeTransform  *m_helpNode;
	bool                             m_DisplayHelp;

	//用来记录按下转弯键时的时间
	deque<unsigned int>              m_rotatefTime;

};


class CMyViewerHandler : public osgGA::GUIEventHandler
{
public:
	CMyViewerHandler(){};
	~CMyViewerHandler(){};


	//重载此函数，目的是为了覆盖OSG本身自带的某些键盘交互键
                bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) 
	{
		return true;
	};

private:
};