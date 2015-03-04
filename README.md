# Restful Api for Mongoose web server

# Useage

create an api-struct like
```c
struct RestApi Api;
```
Init the api with an default fallbackfunction
```c	
RestApiInit(&Api,&DefaultFunction);
```
Add callbacks to the api
Possible methods: GET,POST,PUT,DELETE
Possible methods for finding matching urls: STRCMP (uses strcmp()) and REGEX (ueses normal posix regex)
```c
	RestApiAddRule(&Api,"/api/gpio/","GET",&ApiCallback1,STRCMP);
	RestApiAddRule(&Api,"^/api/gpio/[0-8]{1}$","GET",&ApiCallback2,REGEX);
	RestApiAddRule(&Api,"^/api/gpio/set/[0-8]{1}$","POST",&ApiCallback3,REGEX);
	RestApiAddRule(&Api,"/api/gpio/","PUT",&ApiCallback4,STRCMP);
``` 
And just add the Apihandler to mongoose eventhandler
```c
static int ev_handler(struct mg_connection *conn, enum mg_event ev) {
  switch (ev) {
    case MG_AUTH: return MG_TRUE;
    case MG_REQUEST:
		// handle api-requst
		return RestApiHandle(&Api,conn);
    default: return MG_FALSE;
  }
}
```
