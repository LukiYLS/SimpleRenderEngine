#include "VertexBufferBinding.h"


namespace SRE {



	void VertexBufferBinding::setBinding(unsigned short index, const HardwareVertexBuffer::ptr buffer)
	{
		_binding_map[index] = buffer;
	}
	void VertexBufferBinding::unsetBinding(unsigned short index)
	{
		VertexBufferBindingMap::iterator iter = _binding_map.find(index);
		if (iter == _binding_map.end())
		{
			//没找到
		}
		_binding_map.erase(iter);
	}
	void VertexBufferBinding::unsetAllBindings(void)
	{
		_binding_map.clear();
		_high_index = 0;
	}
	const VertexBufferBindingMap& VertexBufferBinding::getBindings(void) const
	{
		return _binding_map;
	}
	const HardwareVertexBuffer::ptr& VertexBufferBinding::getBuffer(unsigned short index) const
	{
		VertexBufferBindingMap::const_iterator iter = _binding_map.find(index);
		if (iter == _binding_map.end())
		{
			//没找到
		}
		return iter->second;

	}
	bool VertexBufferBinding::isBufferBound(unsigned short index) const
	{
		return _binding_map.find(index) != _binding_map.end();
	}
	unsigned int VertexBufferBinding::getBufferCount(void) const
	{
		return _binding_map.size();
	}
	unsigned short VertexBufferBinding::getNextIndex(void) const
	{
		return _high_index++;
	}
	unsigned short VertexBufferBinding::getLastBoundIndex(void) const
	{
		return _binding_map.empty() ? 0 : _binding_map.rbegin()->first + 1;
	}
}