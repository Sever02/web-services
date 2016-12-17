using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace ads_httplistener.Server
{
    internal class RouteInfo
    {
        public string Route { get; set; }

        public string Method { get; set; }

        public Action<HttpListenerContext> Resolver { get; set; }
    }

    internal partial class WebServer
    {
        private readonly HttpListener listener;
        private readonly int lqueue;

        private readonly List<RouteInfo> routes;

        public WebServer(string host, int cthreads, int lqueue)
        {
            listener = new HttpListener();
            listener.Prefixes.Add(host);

            this.lqueue = lqueue;

            int m1, m2;
            ThreadPool.GetMaxThreads(out m1, out m2);
            ThreadPool.SetMaxThreads(cthreads, m2);

            routes = new List<RouteInfo>();
        }

        public void RegistRoute(string route, string method, Action<HttpListenerContext> resolver)
        {
            routes.Add(new RouteInfo
            {
                Route = route,
                Method = method,
                Resolver = resolver
            });
        }

        public void Start()
        {
            listener.Start();
            Hack.SetRequestQueueLength(listener, lqueue);
            
            while (true)
            {
                var context = listener.GetContext();

                var worker = GetResolver(GetRoute(context.Request.RawUrl), context.Request.HttpMethod);
                if (worker != null)
                {
                    ThreadPool.QueueUserWorkItem(worker, context);
                }
                else
                {
                    NotFound(context);
                }
            }
        }

        private string GetRoute(string rawUrl)
        {
            var qindex = rawUrl.IndexOf('?');
            var route = rawUrl.Substring(0, qindex > 0 ? qindex : rawUrl.Length);
            return route[route.Length - 1] == '/' ? route : route + '/';
        }

        private WaitCallback GetResolver(string route, string method)
        {
            var resolver = routes.SingleOrDefault(x => x.Route == route && x.Method == method);

            if (resolver == null) return null;

            return obj => resolver.Resolver((HttpListenerContext)obj);
        }

        private void NotFound(HttpListenerContext context)
        {
            context.Response.StatusCode = (int)HttpStatusCode.NotFound;
            context.Response.OutputStream.Close();
        }
    }
}
