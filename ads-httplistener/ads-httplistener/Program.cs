using System;
using System.Net;
using System.Text;
using System.Threading;
using ads_httplistener.Server;

namespace ads_httplistener
{
    class Program
    {
        // args[0] - count of threads
        // args[1] - length of request queue
        static void Main(string[] args)
        {
#if DEBUG
            args = new[] { "2", "20" };
#endif

            if (args.Length < 2)
            {
                Console.WriteLine("You should specify THREADS and RQUEUE");
                return;
            }

            int cthreads = Int32.Parse(args[0]);
            int lqueue = Int32.Parse(args[1]);

#if DEBUG
            ADServer.Start("http://localhost:8888/", cthreads, lqueue);
#endif
            ADServer.Start("http://*:80/", cthreads, lqueue);
        }
    }
}
