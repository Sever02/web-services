#include <fastcgi2/component.h>
#include <fastcgi2/component_factory.h>
#include <fastcgi2/handler.h>
#include <fastcgi2/request.h>

#include <iostream>
#include <string>

#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <functional>
#include "shared.h"
vector<Ad> Ads;

using namespace std;

class SimpleClass : virtual public fastcgi::Component, virtual public fastcgi::Handler {

public:
        SimpleClass(fastcgi::ComponentContext *context) :
                fastcgi::Component(context) {
        }
        virtual ~SimpleClass() {
        }

public:
        virtual void onLoad() {
        }
        virtual void onUnload() {
        }
        virtual void handleRequest(fastcgi::Request *request, fastcgi::HandlerContext *context) {

				int geo = atoi(request->getArg("geo").c_str());
				int bt = atoi(request->getArg("bt").c_str());
				int demo = atoi(request->getArg("demo").c_str());
				int placement = atoi(request->getArg("placement").c_str());
		
				vector<Ad>::iterator resIt = find_if(Ads.begin(), Ads.end(), 
					[geo, bt, demo, placement] (const Ad& ad) { 
						bool match = true;
						match &= find(ad.Geos.begin(), ad.Geos.end(), geo) != ad.Geos.end();
						match &= find(ad.Behaves.begin(), ad.Behaves.end(), bt) != ad.Behaves.end();
						match &= find(ad.Demos.begin(), ad.Demos.end(), demo) != ad.Demos.end();
						match &= find(ad.Placements.begin(), ad.Placements.end(), placement) != ad.Placements.end();
						return match; 
				} );
				
				if (resIt != Ads.end())
				{
					const Ad& res = *resIt;
					request->write(res.Uri.c_str(), res.Uri.size());
				}
				else {
					string str = "NOT FOUND !!!";			
					request->sendError(404);
					request->write(str.c_str(), str.size());
				}
				
				
				
        }

};

FCGIDAEMON_REGISTER_FACTORIES_BEGIN()
FCGIDAEMON_ADD_DEFAULT_FACTORY("simple_factory", SimpleClass)
FCGIDAEMON_REGISTER_FACTORIES_END()