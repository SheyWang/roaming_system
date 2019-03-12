/*********************************************************

MyCamera.cpp

*********************************************************/

#include "MyCamera.h"

CMyCamera::CMyCamera(void)
{
	m_fMoveSpeed	   = 3.0f;
	m_fAngle		   = 2.5f; //������ת�Ƕ�
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
//function description: �õ���ͼ����
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
//function description:�õ���ǰ��ͼ���������� 
//
osg::Matrixd CMyCamera::getInverseMatrix(void) const
{
	return osg::Matrixd::inverse(getMatrix()); //�ȵõ���ǰ����ͼ����Ȼ�󷵻��������
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
//function description: �¼�������
//
bool CMyCamera::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
{
	switch (ea.getEventType())  
	{
	case (osgGA::GUIEventAdapter::KEYDOWN): //����Ǽ��̰����¼�
		{
			if (ea.getKey() == 'a') //����
			{
				m_fMyRotateZ += osg::DegreesToRadians(m_fAngle);
				resetRotateZ();
			}

			if (ea.getKey() == 'd')  //���ҿ�
			{
				m_fMyRotateZ -= osg::DegreesToRadians(m_fAngle);
				resetRotateZ();
			}

			if (ea.getKey() == 'w')  //̧ͷ��
			{
				m_vRotation._v[0] += 0.015f;
				return true;
			}

			if (ea.getKey() == 's') //��ͷ��
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
//function description: �����ӿ�λ��
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
//function description:�����ӿڵ�λ��
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
//function description:�õ��ӿ����ڵ�λ��
//
osg::Vec3f CMyCamera::GetPosition()
{
	return m_vPosition;
}
