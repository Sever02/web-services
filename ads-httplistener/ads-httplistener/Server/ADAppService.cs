using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Newtonsoft.Json;

namespace ads_httplistener.Server
{
    internal static class ADAppService
    {
        private class Ad
        {
            public List<int> Geos { get; set; }

            public List<int> Behaves { get; set; }

            public List<int> Demos { get; set; }

            public List<int> Placements { get; set; }
            
            public string Uri { get; set; }

            public int Units;
        }

        private static List<Ad> Ads;

        public static void ProcessPUTAds(HttpListenerContext context)
        {
            string body;
            using (var bodyStream = context.Request.InputStream)
            {
                using (var reader = new StreamReader(bodyStream, context.Request.ContentEncoding))
                {
                    body = reader.ReadToEnd();
                }
            }

            var ads = (List<Ad>)JsonConvert.DeserializeObject(body, typeof(List<Ad>));
            Ads = ads;

            context.Response.StatusCode = (int) HttpStatusCode.OK;
            context.Response.OutputStream.Close();

            GC.Collect();
        }

        public static void ProcessGETAdUri(HttpListenerContext context)
        {
            var geo = Int32.Parse(context.Request.QueryString["geo"]);
            var bt = Int32.Parse(context.Request.QueryString["bt"]);
            var demo = Int32.Parse(context.Request.QueryString["demo"]);
            var placement = Int32.Parse(context.Request.QueryString["placement"]);

            var ad = Ads.FirstOrDefault(x => x.Behaves.Contains(bt) && x.Demos.Contains(demo) && x.Geos.Contains(geo) &&
                    x.Placements.Contains(placement) && x.Units > 0);

            if (ad != null)
            {
                Interlocked.Decrement(ref ad.Units);

                context.Response.StatusCode = (int)HttpStatusCode.OK;
                context.Response.AddHeader("Content-Type", "text/plain; charset=utf-8");
                var msg = Encoding.UTF8.GetBytes(ad.Uri);
                context.Response.ContentLength64 = msg.Length;
                context.Response.OutputStream.Write(msg, 0, msg.Length);
                context.Response.OutputStream.Close();
            }
            else
            {
                context.Response.StatusCode = (int) HttpStatusCode.NotFound;
                context.Response.AddHeader("Content-Type", "text/plain; charset=utf-8");
                var msg = Encoding.UTF8.GetBytes("NOT FOUND");
                context.Response.ContentLength64 = msg.Length;
                context.Response.OutputStream.Write(msg, 0, msg.Length);
                context.Response.OutputStream.Close();
            }
        }

        private static void GenerateSample()
        {
            var rnd = new Random();

            int count = 900000;
            var ads = new List<Ad>(count);
            for (int i = 0; i < count; i++)
            {
                ads.Add(new Ad
                {
                    Behaves = (new byte[rnd.Next(1,10)]).Select(x => rnd.Next(1, 20)).Distinct().ToList(),
                    Demos = (new byte[rnd.Next(1, 10)]).Select(x => rnd.Next(1, 20)).Distinct().ToList(),
                    Geos = (new byte[rnd.Next(1, 10)]).Select(x => rnd.Next(1, 20)).Distinct().ToList(),
                    Placements = (new byte[rnd.Next(1, 10)]).Select(x => rnd.Next(1, 20)).Distinct().ToList(),
                    Units = rnd.Next(10, 500),
                    Uri = i.ToString()
                });
            }

            var json = JsonConvert.SerializeObject(ads);
            File.WriteAllText("body.txt", json);
        }

        private static void GenerateUriSample()
        {
            var rnd = new Random();

            int count = 2000*10*60;
            var sb = new StringBuilder();
            for (int i = 0; i < count; i++)
            {
                sb.Append(String.Format("/aduri?geo={0}&bt={1}&demo={2}&placement={3}\n", rnd.Next(1, 20),
                    rnd.Next(1, 20), rnd.Next(1, 20), rnd.Next(1, 20)));
            }
            File.WriteAllText("uri", sb.ToString());
        }
    }
}
