/*********************************************************

	 main.cpp
 
*********************************************************/

#include "Include.h"
#include "MyCamera.h"
#include "MyLabelMenu.h"
#include "AutoCars.h"
#include "MyEventCallback.h"



const char *titleString = "操作说明";
const char *textString = 
{
"a: 向左看             i: 汽车加速\n"
"d: 向右看             j: 汽车左转\n"
"w: 抬头看             k: 汽车减速\n"
"s: 低头看             l: 汽车右转\n"
"空格:视角变换         o: 汽车停止\n"
"F1: 显示/隐藏帮助     F3: 全屏/窗口切换\n"
"F2: 打开/关闭车灯     F4：初始化汽车位置\n"
};

using namespace std;

void installMemoryLeakDetector()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_crtBreakAlloc = 10923;
#endif
}

//创建覆盖整个场景的光源信息
osg::ref_ptr<osg::Node> createLights()
{
	osg::ref_ptr<osg::Light> myLight1 = new osg::Light;
	myLight1->setLightNum(0);
	myLight1->setPosition(osg::Vec4(0.0f, 0.0f, 4800.0f, 1.0f));	
	myLight1->setAmbient(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	myLight1->setDiffuse(osg::Vec4(0.25f, 0.25f, 0.25f, 0.25f));
	myLight1->setSpecular(osg::Vec4(0.0f, 0.0f, 0.0f, 0.25f));
	myLight1->setConstantAttenuation(1.0f); //设置恒衰减指数
	myLight1->setLinearAttenuation(0.0f);   //设置线性衰减指数
	myLight1->setQuadraticAttenuation(0.0f); //设置二次方衰减指数
	myLight1->setDirection(osg::Vec3(0.0, 0.0, -2000.0f)); 

	osg::ref_ptr<osg::LightSource> lightS1 = new osg::LightSource;
	lightS1->setLight(myLight1);
	lightS1->setName(gSceneLight);
	lightS1->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);

	osg::ref_ptr<osg::Group> group = new osg::Group;
	group->addChild(lightS1);

	return group.get();
}

//创建车灯
osg::ref_ptr<osg::Node> createSpotLamp(osg::ref_ptr<osg::Node> car)
{
	osg::BoundingSphere bs = car->getBound();
	osg::BoundingBox    bb;
	bb.expandBy(bs);

	float modelSize = bb.radius()/10;

	osg::Vec4 pos(bb.center().x(),bb.center().y(),bb.center().z(),1.0f);

	osg::Vec3 direction(bb.center().x() - modelSize, bb.center().y(), -modelSize/4.0f);

	osg::ref_ptr<osg::Light> light1 = new osg::Light;
	light1->setLightNum(1);
	light1->setDiffuse(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	light1->setAmbient(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f ));  
	light1->setSpecular(osg::Vec4(0.0f, 0.0f, 0.0f, 0.f));
	light1->setPosition(pos);
	light1->setDirection(direction);
	light1->setSpotCutoff(20.0f);
	light1->setSpotExponent(10.0f);

	
	osg::ref_ptr<osg::LightSource> lightSource1 = new osg::LightSource;
	lightSource1->setLight(light1 );
	lightSource1->setLocalStateSetModes(osg::StateAttribute::ON); 
	lightSource1->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);

	osg::ref_ptr<osg::PositionAttitudeTransform> pat1 = new osg::PositionAttitudeTransform;
	pat1->addChild(lightSource1.get());
	pat1->setName(gLeftSpotLampName);

	osg::ref_ptr<osg::Group> group = new osg::Group;
	group->addChild(pat1.get());

	return group.get();
}

//驾驶员控制的车辆
osg::ref_ptr<osg::Node> createPlayCar(osg::Vec3 vPos, osg::Vec3 vScale, osg::Node* model)
{
	osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform;
	pat->addChild(model);
	pat->setPosition(vPos);
	osg::Quat quat(osg::DegreesToRadians(-90.0), osg::Z_AXIS);
	pat->setAttitude(quat);
	pat->setScale(vScale);
	pat->setName(gPlayCarName);

	return pat.get(); 
}


//创建雾效
osg::ref_ptr<osg::Fog> createFog()
{
	osg::ref_ptr<osg::Fog> fog = new osg::Fog();
	fog->setColor(osg::Vec4(0.5f, 0.5f, 0.5f, 0.0f));
	fog->setDensity(0.05f);
	fog->setMode(osg::Fog::LINEAR);
	fog->setStart(0.0f);
	fog->setEnd(400.0f);

	return fog.get();
}

//创建驾驶场景
osg::ref_ptr<osg::Node> createScene(osg::Node* model)
{
	osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform;
	pat->addChild(model);
	pat->setScale(osg::Vec3(100.0f, 100.0f, 100.0f));

	

	osg::BoundingSphere bs = pat->getBound();
	pat->setPosition(osg::Vec3(0-bs.center().x(), 0-bs.center().y(), 0-bs.center().z()));
	pat->setName(gMapName);

	return pat.get();
}

//创建雨、雪效果
osg::ref_ptr<osgParticle::PrecipitationEffect> createPrecipitationEffect()
{
	osg::ref_ptr<osgParticle::PrecipitationEffect> precipitationEffect = new osgParticle::PrecipitationEffect;
	precipitationEffect->rain(0.0); //雨
	precipitationEffect->snow(0.0); //雪
	precipitationEffect->setCellSize(osg::Vec3(2.0f, 2.0f, 2.0f));
	precipitationEffect->setParticleSize(0.8f);
	precipitationEffect->setParticleColor(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	precipitationEffect->setWind(osg::Vec3(2, 0, 0));
	precipitationEffect->setUseFarLineSegments(true);
	precipitationEffect->setName(gPrecipitationEffectName);

	return precipitationEffect.get();
}


//创建一个图元碎片（给一个四边形贴上纹理）
osg::ref_ptr<osg::Geometry> createSegment(float x, float y, float z, osg::ref_ptr<osg::Image> image,\
										  bool cullFlag, float alpha)
{
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;

	//定义四个点
	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
	v->push_back(osg::Vec3( 0.0f, y,    z)); //逆时针
	v->push_back(osg::Vec3( 0.0f, 0.0f, z));
	v->push_back(osg::Vec3( x,    0.0f, z));
	v->push_back(osg::Vec3( x,    y,    z));
	
	geom->setVertexArray(v.get());

	//定义法线
	osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array;
	osg::Vec3f upVec(0.0f, 0.0f, 1.0f);
	n->push_back(upVec);
	geom->setNormalArray(n.get());
	geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

	//设置纹理坐标
	osg::ref_ptr<osg::Vec2Array> geomTexcoords = new osg::Vec2Array(4);
	(*geomTexcoords)[0].set(0.0f, 1.0f);
	(*geomTexcoords)[1].set(0.0f, 0.0f);
	(*geomTexcoords)[2].set(1.0f, 0.0f);
	(*geomTexcoords)[3].set(1.0f, 1.0f);
	
	geom->setTexCoordArray(0, geomTexcoords.get());

	osg::Vec4Array* colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(0.5f, 0.5f, 0.5f, alpha));
	geom->setColorArray(colors);
	geom->setColorBinding(osg::Geometry::BIND_OVERALL);

	//设置顶点关联方式
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4)); 

	osg::StateSet* geomState = geom->getOrCreateStateSet();
	geomState->setAttributeAndModes( new osg::BlendFunc, osg::StateAttribute::ON );

	if (cullFlag) //背面剔除
	{
		osg::ref_ptr<osg::CullFace> cullface = new osg::CullFace(osg::CullFace::BACK);
		geomState->setAttribute(cullface.get());
		geomState->setMode(GL_CULL_FACE, osg::StateAttribute::ON);
	}

	if (image) //如果要添加纹理
	{
		osg::ref_ptr<osg::Texture2D> tex  =  new osg::Texture2D;
		tex->setImage(image.get()); 
		geomState->setTextureAttributeAndModes(0, tex.get(),osg::StateAttribute::ON);
	}

	return geom.get();
}


//创建速度显示节点
osg::ref_ptr<osg::Node> createSpeedNode()
{
	osg::ref_ptr<osgText::Text> text = new osgText::Text();
	text->setFont("fonts/simhei.ttf");
	text->setCharacterSize(4.0f);
	text->setColor(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	text->setAlignment(osgText::Text::CENTER_CENTER);
	text->setAxisAlignment(osgText::Text::XY_PLANE);
	text->setText("0.0 km/h");

	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	geode->addDrawable(text.get()); //绘制速度文本
	geode->setName(gSpeedTextName); //给该速度节点设置名字
	geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);  //关闭光照

	osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform;
	pat->addChild(geode.get());
	pat->setPosition(osg::Vec3(-1.0f, 8.724f, 15.60f)); //设置文本的初始化位置（即相对于汽车的位置）
	pat->setScale(osg::Vec3(0.07f, 0.07f, 0.07f));
	pat->setAttitude(osg::Quat(osg::DegreesToRadians(90.0f), osg::X_AXIS)); //旋转

	osg::ref_ptr<osg::PositionAttitudeTransform> pat1 = new osg::PositionAttitudeTransform;
	pat1->addChild(pat.get());     //在程序的运行过程中，每帧都会更新该节点的位置信息
	pat1->setName(gSpeedNodeName); //速度节点的最终位置等于他所有父节点的位置矩阵相乘

	return pat1.get();
}


//创建方向指示器
osg::ref_ptr<osg::Node> createCompassNode()
{
	//
	osg::ref_ptr<osg::Image> compass = osgDB::readImageFile("compass.png");
	osg::ref_ptr<osg::Geometry> compassGeom =  createSegment(1.0f, 1.0f, 0.0f, compass, true, 1.0f);
	osg::ref_ptr<osg::Geode> compassGeode = new osg::Geode;
	compassGeode->addDrawable(compassGeom.get());

	osg::ref_ptr<osg::PositionAttitudeTransform> compassPat = new osg::PositionAttitudeTransform;
	compassPat->addChild(compassGeode.get());
	compassPat->setPivotPoint(osg::Vec3(0.5f, 0.5f, 0.0f));
	compassPat->setPosition(osg::Vec3(0.5f, 0.5f, 0.0f));
	compassPat->setName(gCompassName); //在程序的运行过程中，会根据汽车的旋转状态更新此方向盘的旋转状态


	//指针
	osg::ref_ptr<osg::Image> point   = osgDB::readImageFile("pointer.bmp");
	osg::ref_ptr<osg::Geometry> pointGeom = createSegment(0.8f, 0.1f, 0.001f, point, true, 1.0);
	osg::ref_ptr<osg::Geode> pointGeode = new osg::Geode;
	pointGeode->addDrawable(pointGeom.get());

	osg::ref_ptr<osg::PositionAttitudeTransform> pointPat = new osg::PositionAttitudeTransform;
	pointPat->addChild(pointGeode.get());
	pointPat->setAttitude(osg::Quat(osg::DegreesToRadians(-90.0f), osg::Z_AXIS));
	pointPat->setScale(osg::Vec3(0.4f, 0.5f, 0.5f));
	pointPat->setPosition(osg::Vec3(0.5f, 0.83f, 0.0f));

	osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform;
	pat->addChild(compassPat.get());
	pat->addChild(pointPat.get());
	pat->setPosition(osg::Vec3(-1.8, 6.455, 15.2));
	pat->setAttitude(osg::Quat(osg::DegreesToRadians(90.0), osg::X_AXIS));

	osg::ref_ptr<osg::PositionAttitudeTransform> pat1 = new osg::PositionAttitudeTransform;
	pat1->setName(gCompassNodeName); //每帧都会根据汽车当前的位置更新pat1的位置
	pat1->addChild(pat.get());

	pat1->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	pat1->getOrCreateStateSet()->setMode(GL_LIGHT0, osg::StateAttribute::OFF);
	pat1->getOrCreateStateSet()->setMode(GL_LIGHT1, osg::StateAttribute::OFF);

	return pat1.get();

}


//创建自动行驶的汽车
osg::ref_ptr<osg::Node> createAutoCar(float rotatef, osg::Vec3 start, osg::Vec3 end, osg::Vec3 speed,
									  osg::Vec3 scale, char axis, osg::Node *model)
{
	osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform;
	pat->addChild(model);
	pat->setScale(scale);
	pat->setAttitude(osg::Quat(osg::DegreesToRadians(rotatef), osg::Z_AXIS));
	pat->setPosition(start);
	pat->addUpdateCallback(new CAutoCar(start,end,speed,axis,rotatef));

	return pat.get();
}

//给屏幕添加多辆自动驾驶的汽车
osg::ref_ptr<osg::Node> createAutoCars()
{
	osg::ref_ptr<osg::Group> group = new osg::Group;
	osg::ref_ptr<osg::Node> publicCar = osgDB::readNodeFile("publicCar.ive"); //公交车
	osg::ref_ptr<osg::Node> playCar  = osgDB::readNodeFile("playerCar.ive"); //小轿车 

	/*group->addChild(createAutoCar(180.0f,osg::Vec3(-2000.0f,20.0f,0.0f),osg::Vec3(2000.0f, 20.0f, 0.0f),
	osg::Vec3(3.0f, 0.0f, 0.0f), osg::Vec3(4.5f, 4.5f, 4.5f), 'x', publicCar.get()).get());*/

	/*group->addChild(createAutoCar(90.0f,osg::Vec3(-50.0f,-450.0f,0.0f),osg::Vec3(-50.0f, 2000.0f, 0.0f),
	osg::Vec3(0.0f, 4.0f, 0.0f), osg::Vec3(4.5f, 4.5f, 4.5f), 'y', publicCar.get()).get());*/

	//可以在此添加更多的汽车

	return group.get();
}


//给场景添加雾效
void addStateSet(osg::ref_ptr<osg::Group> root)
{
	osg::StateSet *stateset = root->getOrCreateStateSet();
	stateset->setAttributeAndModes(createFog(), osg::StateAttribute::OFF);
	stateset->setMode(GL_LIGHTING, osg::StateAttribute::ON);
	stateset->setMode(GL_LIGHT0, osg::StateAttribute::ON);
	stateset->setMode(GL_LIGHT1, osg::StateAttribute::OFF);

	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());
}

void setupProperties(osgText::Text& textObject, osgText::Font *font, float size, const osg::Vec3 &pos)
{
	textObject.setFont(font); //设置文本的各种信息
	textObject.setCharacterSize(size);
	textObject.setPosition(pos);
	textObject.setColor(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0));
	textObject.setAlignment(osgText::Text::LEFT_TOP); //左上角对齐
	textObject.setAxisAlignment(osgText::Text::XZ_PLANE);//在XZ平面上
}

void createContent(osgText::Text &textObject, const char *string)
{
	int requiredSize = mbstowcs(NULL, string, 0); //文本字体格式转换，使其支持中文
	wchar_t *wtext = new wchar_t[requiredSize+1];
	mbstowcs(wtext, string, requiredSize+1);
	textObject.setText(wtext);
	delete wtext;
	wtext = NULL;
}

//帮助窗口
osg::ref_ptr<osg::Node> createHelpNode()
{
	setlocale(LC_ALL, ".936");

	osgText::Font* fontHei = osgText::readFontFile("Fonts/simhei.ttf");

	osg::ref_ptr<osgText::Text> title = new osgText::Text;
	setupProperties(*title, fontHei, 1.5f, osg::Vec3(0.0f, 0.0f, 0.0f));
	createContent(*title, titleString);

	osg::ref_ptr<osgText::Text> text = new osgText::Text;
	setupProperties(*text, fontHei, 1.0f,  osg::Vec3(0.0f, 0.0f, 0.0f));
	createContent(*text,textString);

	//
	osg::ref_ptr<osg::Geode> geode1 = new osg::Geode;
	geode1->addDrawable(createSegment(30.0f, 14.0f, 0.0f, NULL, true, 0.4f));

	osg::ref_ptr<osg::PositionAttitudeTransform> pat1 = new osg::PositionAttitudeTransform;
	pat1->addChild(geode1.get());
	pat1->setAttitude(osg::Quat(osg::DegreesToRadians(90.0f), osg::X_AXIS));
	pat1->setPosition(osg::Vec3(0.0f, 0.05f, 0.0f));

	//
	osg::ref_ptr<osg::Geode> geode2 = new osg::Geode;
	geode2->addDrawable(title.get());

	osg::ref_ptr<osg::PositionAttitudeTransform> pat2 = new osg::PositionAttitudeTransform;
	pat2->addChild(geode2.get());
	pat2->setPosition(osg::Vec3(12.5f, 0.0f, 12.5f));

	//
	osg::ref_ptr<osg::Geode> geode3 = new osg::Geode;
	geode3->addDrawable(text.get());

	osg::ref_ptr<osg::PositionAttitudeTransform> pat3 = new osg::PositionAttitudeTransform;
	pat3->addChild(geode3.get());
	pat3->setPosition(osg::Vec3(6.0f, 0.0f, 10.0f));

	//
	osg::ref_ptr<osg::PositionAttitudeTransform> pat4 = new osg::PositionAttitudeTransform;
	pat4->addChild(pat1.get());
	pat4->addChild(pat2.get());
	pat4->addChild(pat3.get());
	pat4->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	pat4->getOrCreateStateSet()->setMode(GL_LIGHT0, osg::StateAttribute::OFF);
	pat4->getOrCreateStateSet()->setMode(GL_LIGHT1, osg::StateAttribute::OFF);
	
	osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform;
	pat->addChild(pat4.get());
	pat->setName(gHelpNodeName);

	return pat.get();
}

int main(int argc, char **argv)
{
	installMemoryLeakDetector();
	setlocale(LC_ALL, "chs");

	cout << "请稍等，正在加载程序运行资源..." ;

	osgViewer::Viewer viewer;

	osgWidget::WindowManager* wm = new osgWidget::WindowManager(&viewer, WINDOW_WIDTH, WINDOW_HEIGHT, MASK_3D);

	osgWidget::Window *menu = new osgWidget::Box("menu", osgWidget::Box::HORIZONTAL);
	menu->addWidget(new CMyLabelMenu("菜单")); //添加菜单按钮
	menu->getBackground()->setColor(1.0f, 1.0f, 1.0f, 1.0f);
	menu->setPosition(15.0f, 15.0f, 0.0f);

	wm->addChild(menu);

	osg::ref_ptr<osg::Group> root = new osg::Group();

	osg::ref_ptr<osg::Node> car = osgDB::readNodeFile("MyCar.ive");
	

	//root->addChild(osgDB::readNodeFile("SkyBox.ive")); //场景添加天空盒子

	//root->addChild(createPlayCar(osg::Vec3(-60.0, -800.0, 0.0), osg::Vec3(0.1, 0.1, 0.1), car.get()));  //汽车
	root->addChild(createPlayCar(osg::Vec3(0.0, 0.0, 0.0), osg::Vec3(0.1, 0.1, 0.1), car.get()));  //汽车

	//root->addChild(createScene(osgDB::readNodeFile("MyScene.ive")).get());  //整个驾驶场景
	root->addChild(createScene(osgDB::readNodeFile("dixing.ive")).get());  //整个驾驶场景

	//root->addChild(createAutoCars().get()); //给场景添加自动行驶的汽车

	root->addChild(createPrecipitationEffect().get());  //添加粒子效果，如“雪天、雾天”


	addStateSet(root);

	root->addChild(createHelpNode().get());      // 添加帮助窗口
	root->addChild(createLights().get());        // 添加全局光照
	root->addChild(createSpotLamp(car).get());   // 添加车灯
	root->addChild(createSpeedNode().get());     // 添加速度计 
	root->addChild(createCompassNode().get());   // 添加方向显示器

	root->setEventCallback(new CMyEventCallback); // 设置事件监听回调函数


	osg::ref_ptr<CMyCamera> camera = new CMyCamera();
	//camera->setPosition(osg::Vec3f(-2.9797, 7.17492, 30));  //设置摄像机的初始位置
	camera->setPosition(osg::Vec3f(-2.9797, 7.17492, 30));  //设置摄像机的初始位置
	

	viewer.setCameraManipulator(camera);
	viewer.addEventHandler(new CMyViewerHandler); 

	return osgWidget::createExample(viewer, wm, root);
}

