#pragma once
#include <iostream>
#include "Poco/Net/OAuth10Credentials.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "EQEW.hpp"

using namespace std;
using namespace Poco::Net;

const string EQEW::URI_TWITTERAPI_REQUEST_TOKEN = "https://api.twitter.com/oauth/request_token";
const string EQEW::TWITTERAPI_HOST = "api.twitter.com";
const Poco::UInt16 EQEW::TWITTERAPI_PORT = 443;

EQEW::EQEW()
{
}

EQEW::EQEW(const string ck,const string cs)
{
	setConsumerKey(ck);
	setConsumerSecret(cs);
}

string EQEW::getConsumerKey() const
{
	return consumerKey;
}
void EQEW::setConsumerKey(const string value)
{
	consumerKey = value;
}
string EQEW::getConsumerSecret() const
{
	return consumerSecret;
}
void EQEW::setConsumerSecret(const string value)
{
	consumerSecret = value;
}

string EQEW::getAccessToken() const
{
	return accessToken;
}
void EQEW::setAccessToken(const string value)
{
	accessToken = value;
}
string EQEW::getAccessTokenSecret() const
{
	return accessTokenSecret;
}
void EQEW::setAccessTokenSecret(const string value)
{
	accessTokenSecret = value;
}

void EQEW::obtainAccessTokenAndSecret()
{
	OAuth10Credentials oc(getConsumerKey(),getConsumerSecret());
	oc.setCallback("oob");
	HTTPRequest req("POST",URI_TWITTERAPI_REQUEST_TOKEN,"1.1");
	Poco::URI uri(URI_TWITTERAPI_REQUEST_TOKEN);
	oc.authenticate(req,uri);
	HTTPSClientSession cs(TWITTERAPI_HOST,TWITTERAPI_PORT);
	cout<<"sssss"<<endl;
	cs.sendRequest(req);
	HTTPResponse resp;
	cout<<cs.receiveResponse(resp);
	cout<<""<<endl;
}
