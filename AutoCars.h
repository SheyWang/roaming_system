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
		
		car->setPosition(car->getPosition()+m_speed); //更新汽车位置

		switch(m_axis)
		{
		case 'x': //如果汽车沿X轴前进
			if (car->getPosition()._v[0] > m_end._v[0] ||   
				car->getPosition()._v[0] < m_start._v[0]) //判断汽车是否越界
			{
//				m_rotatef += 180.0f;
//				car->setAttitude(osg::Quat(osg::DegreesToRadians(m_rotatef), osg::Z_AXIS));
//				m_speed = m_speed * -1;
//				car->setPosition(m_end);
				car->setPosition(m_start);
			}
			break;
		case 'y':  //如果汽车沿Y轴前进
			if (car->getPosition()._v[1] > m_end._v[1] ||
				car->getPosition()._v[1] < m_start._v[1]) //判断汽车是否越界
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
	osg::Vec3 m_start;  //汽车的起始位置
	osg::Vec3 m_end;    //汽车的终止位置
	osg::Vec3 m_speed;  //汽车的行驶速度
	char      m_axis;   //汽车沿某轴前进
	float     m_rotatef;//汽车绕Z轴的旋转角度
};