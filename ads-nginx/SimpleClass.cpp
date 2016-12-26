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

				string query = request->getQueryString();
				int geo_pos = query.find("geo=") + 4;
				int geo_len = query.find("&", geo_pos) - geo_pos;
				int bt_pos = query.find("bt=") + 3;
				int bt_len = query.find("&", bt_pos) - bt_pos;
				int demo_pos = query.find("demo=") + 5;
				int demo_len = query.find("&", demo_pos) - demo_pos;
				int placement_pos = query.find("placement=") + 10;
				
				int geo = atoi( query.substr(geo_pos, geo_len).c_str() );
				int bt = atoi( query.substr(bt_pos, bt_len).c_str() );
				int demo = atoi( query.substr(demo_pos, demo_len).c_str() );
				int placement = atoi( query.substr(placement_pos).c_str() );
				
				//geo=1&bt=2&demo=3&placement=4
		
				vector<Ad>::iterator resIt = find_if(Ads.begin(), Ads.end(), 
					[geo, bt, demo, placement] (const Ad& ad) { 
						bool match = true;
						match &= binary_search(ad.Geos.begin(), ad.Geos.end(), geo);
						match &= binary_search(ad.Behaves.begin(), ad.Behaves.end(), bt);
						match &= binary_search(ad.Demos.begin(), ad.Demos.end(), demo);
						match &= binary_search(ad.Placements.begin(), ad.Placements.end(), placement);
						return match; 
				} );
				
				if (resIt != Ads.end())
				{
					Ad res = *resIt;
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