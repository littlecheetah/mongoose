#ifndef lc_Rest
	#define lc_Rest
	
	#include <stdio.h>
	#include <stdlib.h>
	#include <regex.h>
	#include <string.h>
	#include "mongoose.h"


	#define STRCMP 0
	#define REGEX 1


	struct RestApiRule{
		char *pattern;
		char *method;
		int (*callback)(struct mg_connection *);
		int cmpMethod;
	};

	struct RestApi{
		struct RestApiRule **Rules;
		int iRules;
		int (*defaultCallback)(struct mg_connection *);
	};

	int RestApiInit(struct RestApi *Api,int (*defaultCallback)(struct mg_connection *));
	int RestApiDeinit(struct RestApi *Api);
	int RestApiAddRule(struct RestApi *Api, char *sPath, char *sMethod, int (*callback)(struct mg_connection *),int cmpMethod);
	int RestApiMatch(const char *string, char *pattern);
	int RestApiHandle(struct RestApi *Api,struct mg_connection *);
	
#endif
