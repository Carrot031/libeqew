#pragma once
#include <iostream>
#include <regex>
#include "Poco/Net/OAuth10Credentials.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "EQEW.hpp"

using namespace std;
using namespace Poco::Net;

const string EQEW::URI_TWITTERAPI_REQUEST_TOKEN = "/oauth/request_token";
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
	HTTPRequest req(Poco::Net::HTTPRequest::HTTP_POST,"/oauth/request_token");
	req.setContentLength(0);
	Poco::URI uri("https://api.twitter.com/oauth/request_token");
	oc.authenticate(req,uri);
	const Context::Ptr context = new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_NONE, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
	HTTPSClientSession cs(TWITTERAPI_HOST,TWITTERAPI_PORT,context);
	cout<<"sssss"<<endl;
	cs.sendRequest(req);
	HTTPResponse resp;
	istream& rbody = cs.receiveResponse(resp);
	//cout<<cs.receiveResponse(resp);
//	cout<<resp.read()<<endl;
	resp.write(cout);

	char* rbodybuf = new char[8192];
	rbody.read(rbodybuf,8091);
	cout<<rbodybuf<<endl;
}

/*std::map<std::string, std::string> EQEW::parseQueryString(const std::string& query)
{
	std::map<std::string, std::string> data;
	std::regex pattern("([\\w+%]+)=([^&]*)");
	auto words_begin = std::sregex_iterator(query.begin(), query.end(), pattern);
	auto words_end = std::sregex_iterator();

	for (std::sregex_iterator i = words_begin; i != words_end; i++)
	{
		std::string key = (*i)[1].str();
		std::string value = (*i)[2].str();
		data[key] = value;
	}

	return data;
}*/
