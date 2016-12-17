namespace ads_httplistener.Server
{
    public static class ADServer
    {
        public static void Start(string host, int cthreads, int lqueue)
        {
            var server = new WebServer(host, cthreads, lqueue);
            RegistRoutes(server);
            server.Start();
        }

        private static void RegistRoutes(WebServer server)
        {
            server.RegistRoute("/setup/ads/", "PUT", ADAppService.ProcessPUTAds);
            server.RegistRoute("/aduri/", "GET", ADAppService.ProcessGETAdUri);
        }
    }
}
