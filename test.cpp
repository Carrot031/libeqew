#include "EQEW.hpp"
#include <string>

using namespace std;
int main()
{
	string key = "PBQ03l26fX3eAsgmvPdL9KEhS";
	string sec = "mqs1xwzNU08gvszwdZ8qPg4DKZOLlIRxNJOU3UnNL3YQmiuxpX";
	EQEW e(key,sec);
	e.obtainAccessTokenAndSecret();
	return 0;
}
