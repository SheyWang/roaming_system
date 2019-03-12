/*********************************************************

MyCamera.h 

操作器管理类，用来管理摄像机

*********************************************************/

#pragma once

#include "Include.h"

class CMyCamera : public osgGA::CameraManipulator
{
public:
	CMyCamera(void);
	~CMyCamera(void);

	void  changePosition(const osg::Vec3f& delta);	//位置变换函数
	float getSpeed();								//得到当前速度
	void  setSpeed(float speed);					//设置当前速度
	void  setPosition(const osg::Vec3f& position);  //设置视点位置
	void  setPosition(double*);

	//void  setRotatefZ(float rotatefZ);            //汽车绕Z轴旋转 带动 摄像机绕Z轴旋转
	float getRotatefZ(); //
	float getRotatefX();
	float getRotatefY();
	void resetRotateZ();

	osg::Vec3f GetPosition();						//得到当前视点位置

	virtual void setByMatrix(const osg::Matrixd& matrix);
	virtual void setByInverseMatrix(const osg::Matrixd& matrix);
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);//主要事件控制器
	
	//virtual osg::Matrixd getMatrix(void) const;
	virtual osg::Matrixd getInverseMatrix(void) const;		//得到逆矩阵

	
private:
	unsigned int				m_nID;			    //相机操作器
	float						m_fMoveSpeed;	    //移动速度
	float						m_fAngle;			//屏幕角度
	osg::Vec3f					m_vPosition;        //位置
	osg::Vec3f					m_vRotation;	    //旋转角度
	float                       m_fCarRotateZ;       //汽车转弯带动的摄像机旋转  
	float                       m_fMyRotateZ;        //玩家控制摄像机绕Z轴旋转的角度

};