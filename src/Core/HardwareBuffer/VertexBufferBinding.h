#pragma once
#include "HardwareVertexBuffer.h"
#include <map>
namespace SRE {

	class VertexBufferBinding
	{
	public:
		typedef std::shared_ptr<VertexBufferBinding> ptr;
		typedef std::map<unsigned short, HardwareVertexBuffer::ptr> VertexBufferBindingMap;
	public:
		VertexBufferBinding()
			:_high_index(0)
		{}
		~VertexBufferBinding() { _binding_map.clear(); }
	public:
		void setBinding(unsigned short index, const HardwareVertexBuffer::ptr buffer);
		void unsetBinding(unsigned short index);
		void unsetAllBindings(void);
		const VertexBufferBindingMap& getBindings(void) const;
		const HardwareVertexBuffer::ptr& getBuffer(unsigned short index) const;
		bool isBufferBound(unsigned short index) const;
		unsigned int getBufferCount(void) const;
		unsigned short getNextIndex(void) const;
		unsigned short getLastBoundIndex(void) const;
	private:
		VertexBufferBindingMap _binding_map;
		mutable unsigned short _high_index;;
	};
}