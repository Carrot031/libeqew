#include <iostream>
#include <sstream>
#include <regex>
#include <thread>
#include "liboauthcpp/liboauthcpp.h"
#include "json11/json11.hpp"
#include "EQEW.hpp"
#include "EarthquakeData.hpp"
#include "curl/curl.h"

using namespace std;

namespace libeqew
{

	const string EQEW::URI_TWITTERAPI_REQUEST_TOKEN = "https://api.twitter.com/oauth/request_token";
	const string EQEW::URI_TWITTERAPI_ACCESS_TOKEN = "https://api.twitter.com/oauth/access_token";
	const string EQEW::URI_TWITTERAPI_USERS_SHOW = "https://api.twitter.com/1.1/users/show.json";
	const string EQEW::URI_TWITTERUSERSTREAM_USER = "https://userstream.twitter.com/1.1/user.json";
	const string EQEW::URI_TWITTERSTREAM_FILTER = "https://stream.twitter.com/1.1/statuses/filter.json";
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

	void EQEW::monitoringWorker()
	{
		OAuth::Consumer consumer(getConsumerKey(),getConsumerSecret());
		OAuth::Token oauth_access_token(getAccessToken(),getAccessTokenSecret());
		OAuth::Client client(&consumer,&oauth_access_token);

		const string twitter_uid = "214358709";
		string url = URI_TWITTERSTREAM_FILTER+"?follow="+twitter_uid;

		curl_global_init(CURL_GLOBAL_ALL);
		CURLM* curlm = curl_multi_init();
		CURL* curl = curl_easy_init();
		struct curl_slist* list = nullptr;
		list = curl_slist_append(list,client.getFormattedHttpHeader(OAuth::Http::Get,url.c_str(),"",true).c_str());

		curl_easy_setopt(curl,CURLOPT_URL,url.c_str());
		curl_easy_setopt(curl,CURLOPT_HTTPHEADER,list);
		string downloaded;
		curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,curl_writefunction);
		curl_easy_setopt(curl,CURLOPT_WRITEDATA,(void*)&downloaded);


		curl_multi_add_handle(curlm,curl);
		int handle_count;
		int numfds;

		string newline = {0x0d,0x0a};
		size_t newlinepos;
		string unit; // a json of a tweet.
		while(1) //Download data from twitter server.
		{
			if(stopMonitoringFlag)
			{
				break;
			}
			curl_multi_perform(curlm,&handle_count);
			if(handle_count==0)
			{
				break;
			}

			while(1) //Process the data.
			{
				newlinepos = downloaded.find(newline);
				if(newlinepos != string::npos)
				{
					unit = downloaded.substr(0,newlinepos);
					downloaded = downloaded.substr(newlinepos+1);
					string err;
					auto json = json11::Json::parse(unit,err);
					cout<<json["text"].string_value()<<endl;

					EarthquakeData ed;

					auto splitted = split(json["text"].string_value(),',');
					if(splitted.size()<15)
					{
						continue;
					}

					//Parses string.
					const auto& isdrill_str = splitted[1];
					ed.isDrill = isdrill_str == "01";
					auto timestr = splitted[2];
					struct tm tm;
					if(strptime(timestr.c_str(),"%Y/%m/%d %H:%M:%S",&tm)!=nullptr)
					{
						ed.announcedTime = mktime(&tm);
					}
					ed.id = splitted[5];
					const auto& otimestr = splitted[6];
					if(strptime(otimestr.c_str(),"%Y/%m/%d $H:%M:%S",&tm))
					{
						ed.occuredTime = mktime(&tm);
					}
					ed.nLatitude = stod(splitted[7]);
					ed.eLongitude = stod(splitted[8]);
					ed.epicenterName = splitted[9];
					ed.depth = stod(splitted[10]);
					ed.magnitude = stoi(splitted[11]);
					ed.occuredAtSea = splitted[13] == "1";
					ed.isEmergency = splitted[14] == "1";
					for(auto fp : onEarthquakeOccuredFp)
					{
						fp.first(ed,fp.second);
					}
				}else{
					break;
				}
			}


			curl_multi_wait(curlm,NULL,0,75,&numfds);
		}

		curl_global_cleanup();
	}

	void EQEW::beginMonitoring()
	{
		if(monitoringThread)
		{
			cerr<<"libeqew: beginMonitoring() : Monitoring is already started."<<endl;
			return;
		}
		monitoringThread.reset(new thread(&EQEW::monitoringWorker,this));
	}

	void EQEW::stopMonitoring()
	{
		if(!monitoringThread)
		{
			return;
		}
		stopMonitoringFlag = true;
		monitoringThread->join();
	}

	void EQEW::addOnEarthquakeOccured(void (*fp)(EarthquakeData& e,void* u),void* userdata)
	{
		onEarthquakeOccuredFp.insert(make_pair(fp,userdata));
	}

	void EQEW::clearOnEarthquakeOccured()
	{
		onEarthquakeOccuredFp.clear();
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

}
