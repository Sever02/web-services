#include <fastcgi2/component.h>
#include <fastcgi2/component_factory.h>
#include <fastcgi2/handler.h>
#include <fastcgi2/request.h>

#include <iostream>
#include <string>

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

                //Request handler code. The start point of the request processing.

                //request->setHeader("Simple-Header", "Reply from csimple");
				string str = "Hello, Lords.";
				request->write(str.c_str(), str.size());
        }

};

FCGIDAEMON_REGISTER_FACTORIES_BEGIN()
FCGIDAEMON_ADD_DEFAULT_FACTORY("simple_factory", SimpleClass)
FCGIDAEMON_REGISTER_FACTORIES_END()