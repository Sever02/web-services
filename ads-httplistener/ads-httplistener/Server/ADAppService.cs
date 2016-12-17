using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace ads_httplistener.Server
{
    internal static class ADAppService
    {
        public static void ProcessPUTAds(HttpListenerContext context)
        {
            throw new NotImplementedException();
        }

        public static void ProcessGETAdUri(HttpListenerContext context)
        {
            context.Response.StatusCode = (int)HttpStatusCode.OK;
            context.Response.AddHeader("Content-Type", "text/plain; charset=utf-8");
            var msg = Encoding.UTF8.GetBytes("Hello, Lords.");
            context.Response.ContentLength64 = msg.Length;
            context.Response.OutputStream.Write(msg, 0, msg.Length);
            context.Response.OutputStream.Close();
        }
    }
}
