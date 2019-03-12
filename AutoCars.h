#include "Include.h"

class CAutoCar : public osg::NodeCallback
{
public:
	CAutoCar(osg::Vec3 start, osg::Vec3 end, osg::Vec3 speed, char axis, float rotatef)
	{
		m_start = start;
		m_end   = end;
		m_speed = speed;
		m_axis  = axis;
		m_rotatef = rotatef;
	}

	~CAutoCar(){};

	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		osg::PositionAttitudeTransform *car = dynamic_cast<osg::PositionAttitudeTransform*>(node);
		
		car->setPosition(car->getPosition()+m_speed); //��������λ��

		switch(m_axis)
		{
		case 'x': //���������X��ǰ��
			if (car->getPosition()._v[0] > m_end._v[0] ||   
				car->getPosition()._v[0] < m_start._v[0]) //�ж������Ƿ�Խ��
			{
//				m_rotatef += 180.0f;
//				car->setAttitude(osg::Quat(osg::DegreesToRadians(m_rotatef), osg::Z_AXIS));
//				m_speed = m_speed * -1;
//				car->setPosition(m_end);
				car->setPosition(m_start);
			}
			break;
		case 'y':  //���������Y��ǰ��
			if (car->getPosition()._v[1] > m_end._v[1] ||
				car->getPosition()._v[1] < m_start._v[1]) //�ж������Ƿ�Խ��
			{
				m_rotatef += 180.0f;
				car->setAttitude(osg::Quat(osg::DegreesToRadians(m_rotatef), osg::Z_AXIS));
				m_start = m_speed * -1;
				car->setPosition(m_end);
			}
			break;
		default:
			break;
		}
	}

private:
	osg::Vec3 m_start;  //��������ʼλ��
	osg::Vec3 m_end;    //��������ֹλ��
	osg::Vec3 m_speed;  //��������ʻ�ٶ�
	char      m_axis;   //������ĳ��ǰ��
	float     m_rotatef;//������Z�����ת�Ƕ�
};