#include "VertexDeclaration.h"


namespace SRE {

	const VertexElement::ptr VertexDeclaration::addElement(unsigned short source, unsigned int offset, Vertex_Element_Type type,
		Vertex_Element_Semantic semantic, unsigned short index)
	{
		VertexElement *vert_ele = new VertexElement(source, offset, type, semantic, index);
		VertexElement::ptr vert_ele_ptr(vert_ele);
		_vertex_elements.push_back(vert_ele_ptr);
		return _vertex_elements.back();
	}

	VertexElement::ptr VertexDeclaration::getElement(unsigned int index)
	{
		VertexElementList::iterator iter = _vertex_elements.begin();
		for (unsigned int i = 0; i < index; i++)
			iter++;
		return *iter;
	}

	const VertexElement::ptr VertexDeclaration::insertElement(unsigned short atPosition,
		unsigned short source, unsigned int offset, Vertex_Element_Type type,
		Vertex_Element_Semantic semantic, unsigned short index)
	{
		if (atPosition >= _vertex_elements.size())
			return addElement(source, offset, type, semantic, index);
		VertexElementList::iterator iter = _vertex_elements.begin();
		for (unsigned int i = 0; i < atPosition; i++)
			iter++;
		VertexElement *vert_ele = new VertexElement(source, offset, type, semantic, index);
		VertexElement::ptr vert_ele_ptr(vert_ele);
		iter = _vertex_elements.insert(iter, vert_ele_ptr);
		return *iter;
	}

	void VertexDeclaration::removeElement(unsigned short elem_index)
	{
		if (elem_index < 0 || elem_index  > _vertex_elements.size())
		{
			//assert
		}
		VertexElementList::iterator iter = _vertex_elements.begin();
		for (unsigned int i = 0; i < elem_index; i++)
			iter++;
		_vertex_elements.erase(iter);
	}

	void VertexDeclaration::removeElement(Vertex_Element_Semantic semantic, unsigned short index)
	{
		VertexElementList::iterator ei, eiend;
		eiend = _vertex_elements.end();
		for (ei = _vertex_elements.begin(); ei != eiend; ++ei)
		{
			if ((*ei)->getSemantic() == semantic && (*ei)->getIndex() == index)
			{
				_vertex_elements.erase(ei);
				break;
			}
		}
	}

	void VertexDeclaration::removeAllElements(void)
	{
		_vertex_elements.clear();
	}

	void VertexDeclaration::modifyElement(unsigned short elem_index, unsigned short source, unsigned int offset, Vertex_Element_Type type,
		Vertex_Element_Semantic semantic, unsigned short index)
	{
		if (elem_index < 0 || elem_index > _vertex_elements.size())
		{
			//assert
		}
		VertexElementList::iterator iter = _vertex_elements.begin();
		std::advance(iter, elem_index);
		VertexElement *vert_ele = new VertexElement(source, offset, type, semantic, index);
		VertexElement::ptr vert_ele_ptr(vert_ele);
		(*iter) = vert_ele_ptr;
	}

	const VertexElement::ptr VertexDeclaration::findElementBySemantic(Vertex_Element_Semantic sem, unsigned short index)
	{
		VertexElementList::iterator  ei, eiend;
		eiend = _vertex_elements.end();
		for (ei = _vertex_elements.begin(); ei != eiend; ++ei)
		{
			if ((*ei)->getSemantic() == sem && (*ei)->get_index() == index)
			{
				return (*ei);
			}
		}
		return NULL;
	}
	VertexElementList VertexDeclaration::findElementsBySource(unsigned short source)
	{
		VertexElementList retList;
		VertexElementList::iterator ei, eiend;
		eiend = _vertex_elements.end();
		for (ei = _vertex_elements.begin(); ei != eiend; ++ei)
		{
			if ((*ei)->getSource() == source)
			{
				retList.push_back(*ei);
			}
		}
		return retList;
	}

	unsigned int VertexDeclaration::getVertexSize(unsigned short source)
	{
		VertexElementList::iterator i, iend;
		iend = _vertex_elements.end();
		size_t sz = 0;

		for (i = _vertex_elements.begin(); i != iend; ++i)
		{
			if ((*i)->getSource() == source)
			{
				sz += (*i)->getSize();
			}
		}
		return sz;
	}

	bool VertexDeclaration::vertexElementLess(VertexElement::ptr e1, VertexElement::ptr e2)
	{
		if (e1->getSource() < e2->getSource())
		{
			return true;
		}
		else if (e1->getSource() == e2->getSource())
		{
			// Use ordering of semantics to sort
			if (e1->getSemantic() < e2->getSemantic())
			{
				return true;
			}
			else if (e1->getSemantic() == e2->getSemantic())
			{
				// Use index to sort
				if (e1->getIndex() < e2->getIndex())
				{
					return true;
				}
			}
		}
		return false;
	}
	void VertexDeclaration::sort(void)
	{
		_vertex_elements.sort(VertexDeclaration::vertexElementLess);
	}

}