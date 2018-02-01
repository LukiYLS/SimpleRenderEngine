#pragma once



namespace HTTP {

#define HTTP_PROTOCOL_VERSION_1_1	"HTTP/1.1"
	// 2xx: success
#define 	Http_Status_OK             200 ///< most common code returned when operation was successful
#define 	Http_Status_Created         201 ///< the resource has successfully been created
#define 	Http_Status_Accepted        202 ///< the request has been accepted, but will be processed later by the server
#define 	Http_Status_No_Content       204 ///< the server didn't send any data in return
#define 	Http_Status_Reset_Content    205 ///< the server informs the client that it should clear the view (form) that caused the request to be sent
#define 	Http_Status_Fieldial_Content  206 ///< the server has sent a field of the resource, as a response to a fieldial get request

	// 3xx: redirection
#define 	Http_Status_Multiple_Choices   300 ///< the requested page can be accessed from several locations
#define 	Http_Status_Movedper_Nanently  301 ///< the requested page has permanently moved to a new location
#define 	Http_Status_Movedtem_Porarily  302 ///< the requested page has temporarily moved to a new location
#define 	Http_Status_Not_Modified       304 ///< for conditionnal requests, means the requested page hasn't changed and doesn't need to be refreshed

	// 4xx: client error
#define 	Http_Status_Bad_Request           400 ///< the server couldn't understand the request (syntax error)
#define 	Http_Status_Unauthorized         401 ///< the requested page needs an authentification to be accessed
#define 	Http_Status_Forbidden            403 ///< the requested page cannot be accessed at all, even with authentification
#define 	Http_Status_Not_Found             404 ///< the requested page doesn't exist
#define 	Http_Status_Range_Not_Satisfiable  407 ///< the server can't satisfy the fieldial get request (with a "range" header field)

	// 5xx: server error
#define 	Http_Status_Internal_Server_Error  500 ///< the server encountered an unexpected error
#define 	Http_Status_Not_Implemented       501 ///< the server doesn't implement a requested feature
#define 	Http_Status_Bad_Gateway           502 ///< the gateway server has received an error from the source server
#define 	Http_Status_Service_Not_Available  503 ///< the server is temporarily unavailable (overloaded, in maintenance, ...)
#define 	Http_Status_Gateway_Timeout       504 ///< the gateway server couldn't receive a response from the source server
#define 	Http_Status_Version_Not_Supported  505 ///< the server doesn't support the requested http version

	// 10xx: custom codes
#define 	Http_Status_Invalid_Response   1000 ///< response is not a valid http one
#define 	Http_Status_Connection_Failed  1001  ///< connection with server failed

	//后续用作请求影像服务
}