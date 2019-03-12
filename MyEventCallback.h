#include "Include.h"
#include "NodeVisitor.h"
#include "MyCamera.h"


class CMyEventCallback : public osg::NodeCallback
{
public:
	CMyEventCallback();
	~CMyEventCallback();

	void init(osg::Node* node, osg::NodeVisitor* nv);    //��ʼ�����г�Ա����

                void operator()(osg::Node* node, osg::NodeVisitor* nv);
    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

	void updateScene();                //���³�������

	bool processFog();                 //������Ч
	bool processPlayCar();             //����������ʻ
	bool processPrecipitationEffect(); //������ѩЧ��
	bool processSceneLight();          //����ȫ�ֹ�Դ�ĸ������
	bool processSpotLamp();            //���Ƴ��Ʋ���
	bool processMap();                 //���Ƶ�ͼ  

	void processMenu();                //����������˵�

	void changePosition(osg::Vec3d delta); //��ײ��⣬�ж������Ƿ��ܹ�ǰ��

private:
	//�����ĸ��ڵ�
	osg::Group          *m_rootNode; 

	//��ǰ�ļ��̰����ļ�ֵ
	int                  m_key;

	//���ڿ�����������
	osgViewer::GraphicsWindow *m_gw;

    //ȫ���Ĵ�С
	unsigned int        m_screenWidth;
	unsigned int        m_screenHeight;

	//��ȫ��ʱ�����򴰿ڵ���ʼλ�úͿ���
    unsigned int        m_screenX;
	unsigned int        m_screenY;
	unsigned int        m_screenWindowWidth;
	unsigned int        m_screenWindowHeight;

	//�����е������
	CMyCamera           *m_camera;   

	//��Ч����/�رձ�־ �Լ� ��Ч״̬
	bool                 m_fogFlag;  
	osg::Fog            *m_fog;      

	//��ѩ�ڵ�
	osgParticle::PrecipitationEffect *m_precipitationEffect; 

	//����������ȫ�ֹ�Դ
	osg::Light          *m_sceneLight; 

	//��ʾ������ʻ����� ������ʾ���ڵ�
	osg::PositionAttitudeTransform  *m_compass; 
	osg::PositionAttitudeTransform  *m_compassNode;

	//��ʾ������ʻ�ٶȵ� �ı��ڵ�
	osgText::Text                   *m_speedText;
	osg::PositionAttitudeTransform  *m_speedNode;

	//����
	osg::PositionAttitudeTransform  *m_SpotLamp;

	//������ʻ��ͼ
	osg::PositionAttitudeTransform  *m_map;

	//��ǰ�����ڳ�����λ�� �Լ� �������λ��
	osg::Vec3                        m_position;
	osg::Vec3                        m_cameraPos;

	//��������ʻ�ٶ� �Լ� ��������������Z�����ת�Ƕ�
	float                            m_speed;
	float                            m_rotatef;

	//�Ƿ��ڵ����˳��ӽǼ�ʻ����
	bool                             m_theThreeView;

	//���ڿ���������ʻ�Ľڵ�
	osg::PositionAttitudeTransform  *m_playCar;

	//������ʾ�����˵�
	osg::PositionAttitudeTransform  *m_helpNode;
	bool                             m_DisplayHelp;

	//������¼����ת���ʱ��ʱ��
	deque<unsigned int>              m_rotatefTime;

};


class CMyViewerHandler : public osgGA::GUIEventHandler
{
public:
	CMyViewerHandler(){};
	~CMyViewerHandler(){};


	//���ش˺�����Ŀ����Ϊ�˸���OSG�����Դ���ĳЩ���̽�����
                bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) 
	{
		return true;
	};

private:
};