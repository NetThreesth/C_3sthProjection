#include "serverReq.h"

//---------------------------------------
void serverReq::init()
{
	ofAddListener(_server.newResponseEvent, this, &serverReq::newResponse);
	_server.start();
}

//---------------------------------------
void serverReq::update()
{
	if (_isChatCountUpdate)
	{
		ofNotifyEvent(newChatCount, _chatCount, this);
		_isChatCountUpdate = false;
		
	}

	if (_isPatternUpdate)
	{
		ofNotifyEvent(newPattern, _symbol64, this);
		_isPatternUpdate = false;
	}
}

//---------------------------------------
void serverReq::reqChatCount()
{
	if (_isChatCountUpdate)
	{
		ofLog(OF_LOG_ERROR, "[serverReq::reqChatCount]Dont request twice");
		return;
	}

	ofxHttpForm req;
	req.action = cBackendGetChatCount;
	req.method = OFX_HTTP_GET;
	_server.addForm(req);
}

//---------------------------------------
void serverReq::reqPattern()
{
	if (_isPatternUpdate)
	{
		ofLog(OF_LOG_ERROR, "[serverReq::reqPattern]Dont request twice");
		return;
	}
	ofxHttpForm req;
	req.action = cBackendGetPattern;
	req.method = OFX_HTTP_GET;
	_server.addForm(req);
}

//---------------------------------------
void serverReq::newResponse(ofxHttpResponse & response)
{
	if (response.status == 200)
	{
		string msg = response.responseBody.getText();
		json obj = json::parse(msg);
		if (obj.find("active") == obj.end())
		{
			ofLog(OF_LOG_ERROR, "[serverReq::newResponse]Wrong format");
			return;
		}
		string active = obj["active"];

		if (active == "getChatroomCount")
		{
			_chatCount = obj["count"];
			_isChatCountUpdate = true;
		}
		else if (active == "getPattern")
		{
			string data = obj["pattern"];
			_symbol64 = data;
			_isPatternUpdate = true;
		}
		else
		{
			ofLog(OF_LOG_ERROR, "[serverReq::newResponse]Unknow active");
			return;
		}
	}
	else
	{
		ofLog(OF_LOG_ERROR, "[serverReq::newResponse]get response failed");
	}
}

#pragma region Singleton
//--------------------------------------------------------------
serverReq* serverReq::pInstance = 0;

serverReq* serverReq::getInstance()
{
	if (pInstance == 0)
	{
		pInstance = new serverReq();
	}
	return pInstance;
}

//--------------------------------------------------------------
void serverReq::destroy()
{
	if (pInstance != 0)
	{
		delete pInstance;
	}
}
#pragma endregion


