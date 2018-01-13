#include "HttpService.h"
#include "curl/curl.h"


namespace HTTP {

	//////////////////////////////////////////////////////////////////////////


	HttpRequest::ptr HttpService::createSimpleGet(const char* url)
	{
		if (NULL == url)
			return NULL;

		HttpRequest* request = new HttpRequest(url);
		return HttpRequest::ptr(request);
	}

	HttpRequest::ptr HttpService::createStringPost(const char* url, char* stringBuffer, size_t dataSize)
	{

		if (NULL == url)
			return NULL;

		return HttpRequest::ptr(new HttpRequest(url, stringBuffer, dataSize));

	
	}

	HttpRequest::ptr HttpService::createFilePost(const char* url, const char* fileName)
	{
		if (NULL == url)
			return NULL;

		return HttpRequest::ptr(new HttpRequest(url, fileName));
	}

	HttpRequest::ptr HttpService::createMultiFilesPost(const char* url, const FileNameList& files)
	{
		if (NULL == url)
			return NULL;

		HttpPostField::PostFields file_fields;
		FileNameList::const_iterator it = files.begin();
		for (; it != files.end(); it++)
		{
			file_fields.push_back(new HttpPostField(get_file_base_name(it->c_str()).c_str(), it->c_str()));
		}

		return HttpRequest::ptr(new HttpRequest(url, file_fields));
	}
}