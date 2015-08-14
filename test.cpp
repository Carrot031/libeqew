#include "EQEW.hpp"
#include <string>
#include <iostream>
using namespace std;
int main()
{
	string key = "PBQ03l26fX3eAsgmvPdL9KEhS";
	string sec = "mqs1xwzNU08gvszwdZ8qPg4DKZOLlIRxNJOU3UnNL3YQmiuxpX";
	EQEW e(key,sec);
	cout<<e.beginObtainingAccessTokenAndSecret()<<endl;
return 0;
	string pin;
	cin >> pin;
	e.completeObtainingAccessTokenAndSecret(pin);
	e.beginMonitoring();
	return 0;
}
