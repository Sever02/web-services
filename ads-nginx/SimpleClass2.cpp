#include <fastcgi2/component.h>
#include <fastcgi2/component_factory.h>
#include <fastcgi2/handler.h>
#include <fastcgi2/request.h>
#include <fastcgi2/data_buffer.h>

#include <iostream>

#include <cstdlib>
#include <stdio.h>
#include <string>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "shared.h"

vector<Ad> Ads;

using namespace std;

class SimpleClass2 : virtual public fastcgi::Component, virtual public fastcgi::Handler {

public:
        SimpleClass2(fastcgi::ComponentContext *context) :
                fastcgi::Component(context) {
        }
        virtual ~SimpleClass2() {
        }

public:
        virtual void onLoad() {
        }
        virtual void onUnload() {
        }
        virtual void handleRequest(fastcgi::Request *request, fastcgi::HandlerContext *context) {

                //Request handler code. The start point of the request processing.

                //request->setHeader("Simple-Header", "Reply from csimple");
				
				//global = 12;
				//vector<S> v;
				//S s;
				//s.a = 1;
				//s.b = 3;
				//v.push_back(s);
				//global = v;
				
				// FILL Ads from request
				
				//DataBuffer bodyObj = request->requestBody();
				
				//string bodyJson;
				//bodyObj.toString(&bodyJson);
				
				srand (time(NULL));
				int count = 900000;
				vector<Ad> ads;
				for(int i = 0; i < count; ++i)
				{
					Ad ad;

					vector<int> bts;
					int count_bt = 1 + rand()%9;
					for (int j = 0; j < count_bt; ++j)
					{
						int bt = 1 + rand()%19;
						bool exists = false;
						for (int k = 0; k < bts.size(); ++k)
						{
							if (bt == bts[k])
							{
								exists = true;
								break;
							}
						}
						if (!exists)
						{
							bts.push_back(bt);
						}
					}
					sort(bts.begin(), bts.end());
					
					vector<int> demos;
					int count_demos = 1 + rand()%9;
					for (int j = 0; j < count_demos; ++j)
					{
						int demo = 1 + rand()%19;
						bool exists = false;
						for (int k = 0; k < demos.size(); ++k)
						{
							if (demo == demos[k])
							{
								exists = true;
								break;
							}
						}
						if (!exists)
						{
							demos.push_back(demo);
						}
					}
					sort(demos.begin(), demos.end());
					
					vector<int> geos;
					int count_geos = 1 + rand()%9;
					for (int j = 0; j < count_geos; ++j)
					{
						int geo = 1 + rand()%19;
						bool exists = false;
						for (int k = 0; k < geos.size(); ++k)
						{
							if (geo == geos[k])
							{
								exists = true;
								break;
							}
						}
						if (!exists)
						{
							geos.push_back(geo);
						}
					}
					sort(geos.begin(), geos.end());
					
					vector<int> placements;
					int count_placements = 1 + rand()%9;
					for (int j = 0; j < count_placements; ++j)
					{
						int placement = 1 + rand()%19;
						bool exists = false;
						for (int k = 0; k < placements.size(); ++k)
						{
							if (placement == placements[k])
							{
								exists = true;
								break;
							}
						}
						if (!exists)
						{
							placements.push_back(placement);
						}
					}
					sort(placements.begin(), placements.end());
					
					int units = 10 + rand() % 490;
					char buf[10];
					sprintf(buf, "%d", i);
					string uri(buf);
					
					ad.Geos = geos;
					ad.Behaves = bts;
					ad.Demos = demos;
					ad.Placements = placements;
					ad.Uri = uri;
					ad.Units = units;
					ads.push_back(ad);
				}
				Ads = ads;
				
				string str = "Hello, Lords.";
				request->write(str.c_str(), str.size());
        }

};

FCGIDAEMON_REGISTER_FACTORIES_BEGIN()
FCGIDAEMON_ADD_DEFAULT_FACTORY("simple2_factory", SimpleClass2)
FCGIDAEMON_REGISTER_FACTORIES_END()