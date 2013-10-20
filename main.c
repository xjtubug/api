#include "api.h"

static void shutdown_server(int _)
{
	if (o)
		onion_listen_stop(o);
}

int main(int argc, char *argv[])
{
	signal(SIGINT, shutdown_server);
	signal(SIGTERM, shutdown_server);

	o=onion_new(O_POOL);
	onion_set_max_threads(o, 32);

	onion_set_timeout(o, 5000);
	onion_set_hostname(o, "0.0.0.0");
	onion_set_port(o, "8080");

	onion_url *urls=onion_root_url(o);
	onion_url_add(urls, "", api_error);

	onion_url_add(urls, "^user/query$", api_user_query);
	onion_url_add(urls, "^user/login$", api_user_login);
	onion_url_add(urls, "^user/logout$", api_user_logout);
	onion_url_add(urls, "^user/checksession$", api_user_check_session);

	onion_listen(o);

	onion_free(o);
	return 0;
}