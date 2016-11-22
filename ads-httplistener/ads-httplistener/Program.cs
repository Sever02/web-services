using System.Net;
using System.Text;
using System.Threading; 

namespace ads_httplistener
{
    class Program
    {
        private class WebServer
        {
            HttpListener _listener;

            public WebServer(string address)
            {
                _listener = new HttpListener();
                _listener.Prefixes.Add(address);
            }

            public void Start()
            {
                _listener.Start();
                while (true)
                {
                    HttpListenerContext request = _listener.GetContext();
                    ThreadPool.QueueUserWorkItem(ProcessRequest, request);
                }
            }

            void ProcessRequest(object listenerContext)
            {
                var context = (HttpListenerContext)listenerContext;
                context.Response.StatusCode = (int)HttpStatusCode.OK;
                context.Response.AddHeader("Content-Type", "text/plain; charset=utf-8");
                var msg = Encoding.UTF8.GetBytes("Hello, Lords.");
                context.Response.ContentLength64 = msg.Length;
                context.Response.OutputStream.Write(msg, 0, msg.Length);
                context.Response.OutputStream.Close();
            }
        } 

        static void Main(string[] args)
        {
            (new WebServer("http://localhost:9998/")).Start();
        }
    }
}
