#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../mongoose.h"
#include "../restApi.h"

struct RestApi Api;

static int ev_handler(struct mg_connection *conn, enum mg_event ev) {
  switch (ev) {
    case MG_AUTH: return MG_TRUE;
    case MG_REQUEST:
		// handle api-requst
		return RestApiHandle(&Api,conn);
    default: return MG_FALSE;
  }
}


/* Rest-Api callback-functions */
int DefaultFunction(struct mg_connection *conn){
	printf("defaultcalltback: %s %s\n",conn->request_method,conn->uri);
	mg_printf_data(conn,"defaultcalltback");
	return MG_TRUE;
}

int Api1(struct mg_connection *conn){
	printf("Api1: %s %s\n",conn->request_method,conn->uri);
	mg_printf_data(conn,"Api1");
	return MG_TRUE;
}

int Api2(struct mg_connection *conn){
	printf("Api2: %s %s\n",conn->request_method,conn->uri);
	mg_printf_data(conn,"Api2");
	return MG_TRUE;
}

int main(void) {
	struct mg_server *server;

	// Create and configure the server
	server = mg_create_server(NULL, ev_handler);
	mg_set_option(server, "listening_port", "8000");

	// create restApi
	RestApiInit(&Api,&DefaultFunction);
	
	// add Rules
	RestApiAddRule(&Api,"/api/gpio/","GET",&Api1,STRCMP);
	RestApiAddRule(&Api,"^/api/gpio/[0-8]{1}$","GET",&Api2,REGEX);
	RestApiAddRule(&Api,"^/api/gpio/set/[0-8]{1}$","POST",&Api2,REGEX);
	RestApiAddRule(&Api,"/api/gpio/","PUT",&Api1,STRCMP);
	

	// Serve request. Hit Ctrl-C to terminate the program
	printf("Starting on port %s\n", mg_get_option(server, "listening_port"));
	for (;;) {
		mg_poll_server(server, 1000);
	}

	// cleanup/free Api
	RestApiDeinit(&Api);

	// Cleanup, and free server instance
	mg_destroy_server(&server);

	return 0;
}
