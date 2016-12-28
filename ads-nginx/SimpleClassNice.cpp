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

namespace {

static const std::string NOT_FOUND = "NOT FOUND !!!";
static const std::string GEO = "geo";
static const std::string BT = "bt";
static const std::string DEMO = "demo";
static const std::string PLACEMENT = "placement";


bool matchField(const std::vector<int>& vec, int value)
{
	for (auto i : vec)
		if (i == value)
			return true;
	return false;
}

int getArg(const fastcgi::Request* request, const std::string& name)
{
	return atoi(request->getArg(name).c_str());
}

void write(fastcgi::Request* request, const std::string& data)
{
	request->write(data.c_str(), data.size());
}

};


class SimpleClass : virtual public fastcgi::Component, virtual public fastcgi::Handler {

public:
	SimpleClass(fastcgi::ComponentContext *context) :
		fastcgi::Component(context) {}
	virtual ~SimpleClass() {}

public:
	virtual void onLoad() {}
	virtual void onUnload() {}
	virtual void handleRequest(fastcgi::Request *request, fastcgi::HandlerContext *context)
	{
		int geo = getArg(request, GEO);
		int bt = getArg(request, BT);
		int demo = getArg(request, DEMO);
		int placement = getArg(request, PLACEMENT);

		auto isMatch = [geo, bt, demo, placement] (const Ad& ad) {
			return (
				matchField(ad.Geos, geo) &&
				matchField(ad.Behaves, bt) &&
				matchField(ad.Demos, demo) &&
				matchField(ad.Placements, placement)
			);
		};

		for (const auto& ad : Ads) {
			if (isMatch(ad)) {
				write(request, ad.Uri);
				return;
			}
		}

		request->sendError(404);
		write(request, NOT_FOUND);
	}
};

FCGIDAEMON_REGISTER_FACTORIES_BEGIN()
FCGIDAEMON_ADD_DEFAULT_FACTORY("simple_factory", SimpleClass)
FCGIDAEMON_REGISTER_FACTORIES_END()