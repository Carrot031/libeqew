#include <iostream>
#include <sstream>
#include <regex>
#include "Poco/Net/OAuth10Credentials.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "liboauthcpp/liboauthcpp.h"
#include "EQEW.hpp"
#include "curl/curl.h"

using namespace std;
using namespace Poco::Net;

const string EQEW::URI_TWITTERAPI_REQUEST_TOKEN = "https://api.twitter.com/oauth/request_token";
const string EQEW::URI_TWITTERAPI_ACCESS_TOKEN = "https://api.twitter.com/oauth/access_token";
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

size_t EQEW::curl_writefunction(char* ptr, size_t size, size_t nmemb, void* userdata)
{
	size_t ret = size * nmemb;
	string* str = (string*) userdata;
	for(size_t i=0; i<ret;i++)
	{
		*str += *ptr;
		ptr++;
	}
	return ret;
}

string EQEW::beginObtainingAccessTokenAndSecret()
{
/*
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
	*/

	OAuth::Consumer consumer(getConsumerKey(),getConsumerSecret());
	OAuth::Client oauth(&consumer);
	curl_global_init(CURL_GLOBAL_ALL);

	string oauth_header = oauth.getFormattedHttpHeader(OAuth::Http::Post,URI_TWITTERAPI_REQUEST_TOKEN,"",true);
	cout<<oauth_header<< endl;
	CURL* curl = curl_easy_init();
	struct curl_slist* list = nullptr;

	if(!curl)
	{
		cout<<"unable to initialize curl."<<endl;
		return "";
	}
	curl_easy_setopt(curl,CURLOPT_URL,URI_TWITTERAPI_REQUEST_TOKEN.c_str());
	list = curl_slist_append(list,oauth_header.c_str());
	curl_easy_setopt(curl,CURLOPT_HTTPHEADER,list);
	string downloaded;
	curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,curl_writefunction);
	curl_easy_setopt(curl,CURLOPT_WRITEDATA,(void*)&downloaded);

	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, 0);
	curl_easy_perform(curl);
	curl_slist_free_all(list);
	curl_global_cleanup();
	map<string,string> q = parseQueryString(downloaded);
	requestToken = q["oauth_token"];
	requestTokenSecret = q["oauth_token_secret"];
	
	return "https://api.twitter.com/oauth/authorize?oauth_token="+requestToken;
}

void EQEW::completeObtainingAccessTokenAndSecret(const string& pin)
{
/*
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
	cout<<"received:"<<rbodybuf<<endl;


	map<string,string> data = parseQueryString(rbodybuf);
	setAccessToken(data["oauth_token"]);
	setAccessTokenSecret(data["oauth_token_secret"]);

	delete[] rbodybuf;
*/

	OAuth::Consumer consumer(getConsumerKey(),getConsumerSecret());
	OAuth::Token oauth_request_token(requestToken,requestTokenSecret);
	oauth_request_token.setPin(pin);

	OAuth::Client client(&consumer,&oauth_request_token);

	string oauth_header = client.getFormattedHttpHeader(OAuth::Http::Post,URI_TWITTERAPI_ACCESS_TOKEN,"",true);

	curl_global_init(CURL_GLOBAL_ALL);
	CURL* curl = curl_easy_init();
	struct curl_slist* list = nullptr;
	list = curl_slist_append(list, oauth_header.c_str());
	curl_easy_setopt(curl,CURLOPT_URL,URI_TWITTERAPI_ACCESS_TOKEN.c_str());
	curl_easy_setopt(curl,CURLOPT_HTTPHEADER,list);
	string downloaded;
	curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,curl_writefunction);
	curl_easy_setopt(curl,CURLOPT_WRITEDATA,(void*)&downloaded);
	
	curl_easy_perform(curl);
	//cout<<downloaded<<endl;
	
	map<string,string> data = parseQueryString(downloaded);
	setAccessToken(data["oauth_token"]);
	setAccessTokenSecret(data["oauth_token_secret"]);
	curl_global_cleanup();
}

void EQEW::beginMonitoring()
{
	OAuth10Credentials oc(getConsumerKey(),getConsumerSecret());
	cout<<"token:"<<getAccessToken()<<endl;
	cout<<"tokensecret:"<<getAccessTokenSecret()<<endl;
	oc.setToken(getAccessToken());
	oc.setTokenSecret(getAccessTokenSecret());
	cout<<oc.getTokenSecret()<<endl;
	HTTPRequest req(Poco::Net::HTTPRequest::HTTP_GET,"/1.1/users/show.json");
	//req.setVersion("1.1");
	req.set("User-Agent","libeqew");
	Poco::URI uri("https://api.twitter.com/1.1/users/show.json?screen_name=pumpkin031");
	oc.authenticate(req,uri);

	cout<<req.get("Authorization")<<endl;
	const Context::Ptr context = new Context(Context::CLIENT_USE,"","","",Context::VERIFY_NONE,9,false,"ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
	HTTPSClientSession cs("api.twitter.com",TWITTERAPI_PORT,context);
	cs.setKeepAlive(true);
	HTTPResponse resp;


	ostream& outstream = cs.sendRequest(req);
	istream& rbody = cs.receiveResponse(resp);

	cout<<resp.getReason()<<endl;

	char* rbodybuf = new char[8192];
	rbody.read(rbodybuf,8191);
	cout<<rbodybuf<<endl;
	delete[] rbodybuf;


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
