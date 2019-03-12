#pragma once

#include "Include.h"



//自定义节点访问器
class CMyNodeVisitor : public osg::NodeVisitor
{
private:
	string m_NameToFind;     //需要查找的节点的名字
	osg::Node* m_NodeToFind; //查找得到的节点

public:
	CMyNodeVisitor();
	~CMyNodeVisitor();

	void setNameToFind(const string& str);            //设置要查找节点的名字
	osg::Node* getFindNode();                        //返回查找得到的节点



                void apply(osg::Node&  searchNode);

	//以下这两个方法可以不需要..
	virtual void apply(osg::Geode& geode);
	virtual void apply(osg::PositionAttitudeTransform& searchNode);


};