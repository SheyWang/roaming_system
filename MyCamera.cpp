/*********************************************************

MyCamera.cpp

*********************************************************/

#include "MyCamera.h"

CMyCamera::CMyCamera(void)
{
	m_fMoveSpeed	   = 3.0f;
	m_fAngle		   = 2.5f; //左右旋转角度
	m_vPosition		   = osg::Vec3f(0.0, 0.0f, 5.0f);
	m_vRotation		   = osg::Vec3f(osg::PI_2-0.165f, 0.0f, 0.0f);
	m_fCarRotateZ      = 0.0f;
	m_fMyRotateZ       = 0.0f;
}

CMyCamera::~CMyCamera(void)
{
}

float CMyCamera::getRotatefX()
{
	return m_vRotation._v[0];
}

float CMyCamera::getRotatefY()
{
	return m_vRotation._v[1];
}

void CMyCamera::setByMatrix(const osg::Matrixd& matrix)
{

}

void CMyCamera::setByInverseMatrix(const osg::Matrixd& matrix)
{

}


//******************************************************************
//function description: 得到视图矩阵
//
osg::Matrixd CMyCamera::getMatrix(void) const
{
	osg::Matrixd mat;
	mat.makeRotate(m_vRotation._v[0], osg::Vec3(1.0f, 0.0f, 0.0f), 
			   m_vRotation._v[1], osg::Vec3(0.0f, 1.0f, 0.0f),
			   m_vRotation._v[2], osg::Vec3(0.0f, 0.0f, 1.0f));

	return mat * osg::Matrixd::translate(m_vPosition);

}


//****************************************************************** 
//function description:得到当前视图矩阵的逆矩阵 
//
osg::Matrixd CMyCamera::getInverseMatrix(void) const
{
	return osg::Matrixd::inverse(getMatrix()); //先得到当前的视图矩阵，然后返回其逆矩阵
}

void  CMyCamera::setRotatefZ(float rotatefZ)
{
	m_fCarRotateZ = rotatefZ;
	resetRotateZ();
}

float CMyCamera::getRotatefZ()
{
	return m_vRotation._v[2];
}


//****************************************************************** 
//function description: 事件控制器
//
bool CMyCamera::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
	switch (ea.getEventType())  
	{
	case (osgGA::GUIEventAdapter::KEYDOWN): //如果是键盘按下事件
		{
			if (ea.getKey() == 'a') //向左看
			{
				m_fMyRotateZ += osg::DegreesToRadians(m_fAngle);
				resetRotateZ();
			}

			if (ea.getKey() == 'd')  //向右看
			{
				m_fMyRotateZ -= osg::DegreesToRadians(m_fAngle);
				resetRotateZ();
			}

			if (ea.getKey() == 'w')  //抬头看
			{
				m_vRotation._v[0] += 0.015f;
				return true;
			}

			if (ea.getKey() == 's') //低头看
			{
				m_vRotation._v[0] -= 0.015f;
				return true;
			}

		}
	}

	return false;
}


void CMyCamera::resetRotateZ()
{
	m_vRotation._v[2] = m_fMyRotateZ + m_fCarRotateZ;
}

//****************************************************************** 
//function description: 更新视口位置
//
void CMyCamera::changePosition(const osg::Vec3f &delta)
{
	m_vPosition += delta;
}


void CMyCamera::setSpeed(float speed)
{
	m_fMoveSpeed = speed;
}


//****************************************************************** 
//function description:设置视口的位置
//
void CMyCamera::setPosition(const osg::Vec3f &position)
{
	m_vPosition = position;
}

void CMyCamera::setPosition(double *position)
{
	m_vPosition._v[0] = position[0];
	m_vPosition._v[1] = position[1];
	m_vPosition._v[2] = position[2];
}

//****************************************************************** 
//function description:得到视口所在的位置
//
osg::Vec3f CMyCamera::GetPosition()
{
	return m_vPosition;
}
