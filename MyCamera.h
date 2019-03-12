/*********************************************************

MyCamera.h 

�����������࣬�������������

*********************************************************/

#pragma once

#include "Include.h"

class CMyCamera : public osgGA::CameraManipulator
{
public:
	CMyCamera(void);
	~CMyCamera(void);

	void  changePosition(const osg::Vec3f& delta);	//λ�ñ任����
	float getSpeed();								//�õ���ǰ�ٶ�
	void  setSpeed(float speed);					//���õ�ǰ�ٶ�
	void  setPosition(const osg::Vec3f& position);  //�����ӵ�λ��
	void  setPosition(double*);

	//void  setRotatefZ(float rotatefZ);            //������Z����ת ���� �������Z����ת
	float getRotatefZ(); //
	float getRotatefX();
	float getRotatefY();
	void resetRotateZ();

	osg::Vec3f GetPosition();						//�õ���ǰ�ӵ�λ��

	virtual void setByMatrix(const osg::Matrixd& matrix);
	virtual void setByInverseMatrix(const osg::Matrixd& matrix);
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);//��Ҫ�¼�������
	
	//virtual osg::Matrixd getMatrix(void) const;
	virtual osg::Matrixd getInverseMatrix(void) const;		//�õ������

	
private:
	unsigned int				m_nID;			    //���������
	float						m_fMoveSpeed;	    //�ƶ��ٶ�
	float						m_fAngle;			//��Ļ�Ƕ�
	osg::Vec3f					m_vPosition;        //λ��
	osg::Vec3f					m_vRotation;	    //��ת�Ƕ�
	float                       m_fCarRotateZ;       //����ת��������������ת  
	float                       m_fMyRotateZ;        //��ҿ����������Z����ת�ĽǶ�

};