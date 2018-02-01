#pragma once

#include "HttpService.h"

namespace SRE {

	class RequestDomOrDem {

	public:

		char* getDomDataFromService(int x, int y, int level);

	private:

		std::string _baseUrl;
		std::string _styleName;
		std::string _driverName;

		std::string _formatName;
		std::string _layerName;
		std::string _tileMatrixSet;
		std::string _tileMatirx;
	};
}