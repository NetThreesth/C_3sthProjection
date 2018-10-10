#pragma once

#include "constParameter.h"

#include "ofxHttpUtils.h"
#include "json.hpp"
using json = nlohmann::json;
class serverReq
{
public:
	void init();
	void update();
	void reqChatCount();
	void reqPattern();
	void newResponse(ofxHttpResponse & response);

public:
	bool _isChatCountUpdate, _isPatternUpdate;
	int _chatCount;
	string _symbol64;
	ofxHttpUtils _server;

public:
	ofEvent<int> newChatCount;
	ofEvent<string> newPattern;


	//-------------------
	//Singleton
	//-------------------
private:
	serverReq()
		:_isChatCountUpdate(false)
		, _isPatternUpdate(false)
	{
	};
	~serverReq()
	{
		serverReq::destroy();
	}
	serverReq(serverReq const&);
	void operator=(serverReq const&);

public:
	static serverReq* getInstance();
	static void destroy();

private:
	static serverReq *pInstance;
};