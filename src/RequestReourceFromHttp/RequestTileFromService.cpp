#include "RequestTileFromService.h"
#include <sstream>
namespace SRE {

	char* RequestDomOrDem::getDomDataFromService(int x, int y, int level)
	{
		char* img_data = NULL;
		std::stringstream buf;
		char sep = _baseUrl.find_first_of('?') == std::string::npos ? '?' : '&';

		if (strcmp(_driverName.c_str(), "google") == 0)  // for googleEarth service
		{
			buf << _baseUrl << "&z=%d" << "&y=%d" << "&x=%d";
		}
		else if (strcmp(_baseUrl.c_str(), "wmts") == 0)
		{
			buf << _baseUrl << sep
				<< "SERVICE=WMTS"
				<< "&REQUEST=GetTile"
				<< "&VERSION=1.0.0"
				<< "&LAYER=" << _layerName
				<< "&STYLE=" << _styleName
				<< "&FORMAT=" << _formatName
				<< "&TileMatrixSet=" << _tileMatrixSet
				<< "&TileMatrix=" << _tileMatirx << "%d"
				<< "&TileRow=%d"
				<< "&TileCol=%d";
		}


		std::string url = buf.str();

		char request_url[2048];

		//sprintf( request_url, buf.str().c_str(), zt, xt, yt );
		sprintf(request_url, buf.str().c_str(), level, y, x);

		std::string base_url(request_url);

		HttpRequest::ptr request = HttpService::getSingleton()->createSimpleGet(base_url.c_str());
		HttpResponse::ptr responce = std::make_shared<HttpResponse>();
		HttpService::getSingleton()->get(request, responce, NULL);

		if (!responce->isOk())
		{
			//
		}
		else
		{
			std::string* data = responce->getStringData();
			int len = data->length();
			//mem_len_ = len;
			if (len == 0)
			{
				return NULL;
			}
			img_data = new char[len];
			memcpy(img_data, data->data(), len * sizeof(char));
		}


		return img_data;

	}
}