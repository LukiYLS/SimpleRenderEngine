#pragma once
#include <memory>
#include <string>
#include <map>
#include "utils\AnyValue.h"
using namespace Utils;
using namespace std;
namespace Core {

	class Parameter {

	public:
		typedef shared_ptr<Parameter> ptr;
		void addParameter(const string& name, AnyValue value) { _paras.insert(make_pair(name, value)); }
		AnyValue getValue(const string& name) {
			if (_paras.find(name) != _paras.end())
				return _paras[name];
			else
				return NULL;
		}
	private:
		map<string, AnyValue> _paras;
	};
}