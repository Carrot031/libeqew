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

string EQEW::beginObtainingAccessTokenAndSecret()
{
	OAuth10Credentials oc(getConsumerKey(),getConsumerSecret());
	oc.setCallback("oob");
	HTTPRequest req(Poco::Net::HTTPRequest::HTTP_POST,"/oauth/request_token");
	req.setContentLength(0);
	Poco::URI uri("https://api.twitter.com/oauth/request_token");
	oc.authenticate(req,uri);
	const Context::Ptr context = new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_NONE, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
	HTTPSClientSession cs(TWITTERAPI_HOST,TWITTERAPI_PORT,context);
	//cout<<"sssss"<<endl;
	cs.sendRequest(req);
	HTTPResponse resp;
	istream& rbody = cs.receiveResponse(resp);
	//resp.write(cout);

	char* rbodybuf = new char[8192];
	rbody.read(rbodybuf,8091);
	cout<<rbodybuf<<endl;

	map<string,string> q = parseQueryString(rbodybuf);
	for(auto unit:q)
	{
		cout<<unit.first<<endl;
	}

	delete[] rbodybuf;

	string request_token = q["oauth_token"];
	string request_token_secret = q["oauth_token_secret"];

	cout<<"request_token:"+request_token<<endl;

	requestToken = request_token;
	requestTokenSecret = request_token_secret;
	return "https://api.twitter.com/oauth/authorize?oauth_token="+request_token;
}

void EQEW::completeObtainingAccessTokenAndSecret(const string& pin)
{
	OAuth10Credentials oc(getConsumerKey(),getConsumerSecret());
	oc.setToken(requestToken);
	oc.setTokenSecret(requestTokenSecret);
	oc.setCallback("oob");
	oc.setRealm("oauth_verifier="+pin);
	HTTPRequest req(Poco::Net::HTTPRequest::HTTP_POST,"/oauth/access_token?oauth_verifier="+pin);
	req.setContentType("application/x-www-form-urlencoded");
	Poco::URI uri("https://api.twitter.com/oauth/access_token?oauth_verifier="+pin);
	oc.authenticate(req,uri);
	const Context::Ptr context = new Context(Context::CLIENT_USE,"","","",Context::VERIFY_NONE,9,false,"ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
	HTTPSClientSession cs(TWITTERAPI_HOST,TWITTERAPI_PORT,context);
	HTTPResponse resp;

	req.set("oauth_verifier",pin);
	req.setContentLength(0);

	ostream& outstream = cs.sendRequest(req);
	outstream << "\noauth_verifier="<<pin <<flush;
	istream& rbody = cs.receiveResponse(resp);
	
	char* rbodybuf = new char[8192];
	rbody.read(rbodybuf,8091);
	cout<<rbodybuf<<endl;

	delete[] rbodybuf;

	map<string,string> data = parseQueryString(rbodybuf);
	setAccessToken(data["oauth_token"]);
	setAccessTokenSecret(data["oauth_token_secret"]);

}

std::map<std::string, std::string> EQEW::parseQueryString(const std::string& query)
{
	std::map<std::string, std::string> data;

	vector<string> splitted = split(query,'&');
	string key;
	string value;
	vector<string>::iterator it = splitted.begin();
	for(auto unit:splitted)
	{
		vector<string> pair = split(unit,'=');
		if(pair.size() != 2)
		{
			return data;
		}
		key = pair[0];
		value= pair[1];
		data[key] = value;
	}
	return data;
}

vector<string> EQEW::split(const string &s, char delim) {
	vector<string> elems;
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {
		if (!item.empty()) {
			elems.push_back(item);
		}
	}
	return elems;
}
