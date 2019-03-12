#include "NodeVisitor.h"


CMyNodeVisitor::CMyNodeVisitor():osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
{
	m_NameToFind = "";
}

CMyNodeVisitor::~CMyNodeVisitor()
{

}

void CMyNodeVisitor::setNameToFind(const string& str)
{
	m_NameToFind = str;
}

osg::Node* CMyNodeVisitor::getFindNode()
{
	return m_NodeToFind;
}

void CMyNodeVisitor::apply(osg::Node& searchNode)
{
	if (searchNode.getName() == m_NameToFind)
	{
		m_NodeToFind = &searchNode;
		return ;
	}
	traverse(searchNode);
}


void CMyNodeVisitor::apply(osg::Geode& geode)
{
	apply((osg::Node&)geode);
	traverse(geode);
}

void CMyNodeVisitor::apply(osg::PositionAttitudeTransform& searchNode)
{
	apply((osg::Node&)searchNode);
	traverse((osg::Node&)searchNode);
}