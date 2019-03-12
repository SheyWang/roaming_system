#pragma once

#include "Include.h"



//�Զ���ڵ������
class CMyNodeVisitor : public osg::NodeVisitor
{
private:
	string m_NameToFind;     //��Ҫ���ҵĽڵ������
	osg::Node* m_NodeToFind; //���ҵõ��Ľڵ�

public:
	CMyNodeVisitor();
	~CMyNodeVisitor();

	void setNameToFind(const string& str);            //����Ҫ���ҽڵ������
	osg::Node* getFindNode();                        //���ز��ҵõ��Ľڵ�



                void apply(osg::Node&  searchNode);

	//�����������������Բ���Ҫ..
	virtual void apply(osg::Geode& geode);
	virtual void apply(osg::PositionAttitudeTransform& searchNode);


};