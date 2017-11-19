#pragma once

#include "VertexElement.h"
#include <list>
namespace SRE {

	class VertexDeclaration {
	public:
		typedef std::shared_ptr<VertexDeclaration> ptr;
		typedef std::list<VertexElement::ptr> VertexElementList;
		VertexDeclaration() = default;
		~VertexDeclaration()
		{
			_vertex_elements.clear();
		}

	public:
		static bool vertexElementLess(VertexElement::ptr e1, VertexElement::ptr e2);
		unsigned int getElementCount()const { return _vertex_elements.size(); }
		const VertexElementList& getElements() const { return _vertex_elements; }
		VertexElement::ptr getElement(unsigned int index);
		void sort();
		const VertexElement::ptr addElement(unsigned short source, unsigned int offset, Vertex_Element_Type type,
			Vertex_Element_Semantic semantic, unsigned short index = 0);
		const VertexElement::ptr insertElement(unsigned short atPosition,
			unsigned short source, unsigned int offset, Vertex_Element_Type theType,
			Vertex_Element_Semantic semantic, unsigned short index = 0);
		void removeElement(unsigned short elem_index);
		void removeElement(Vertex_Element_Semantic semantic, unsigned short index = 0);
		void removeAllElements(void);
		void modifyElement(unsigned short elem_index, unsigned short source, unsigned int offset, Vertex_Element_Type theType,
			Vertex_Element_Semantic semantic, unsigned short index = 0);
		const VertexElement::ptr findElementBySemantic(Vertex_Element_Semantic sem, unsigned short index = 0);
		VertexElementList findElementsBySource(unsigned short source);
		 unsigned int getVertexSize(unsigned short source);

	protected:

		VertexElementList _vertex_elements;
	};
}