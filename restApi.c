#include "restApi.h"

void dumpRules(struct RestApi *Api){
	int i;
	printf("Rules: %i\n",Api->iRules);
	for(i=0;i<Api->iRules;i++){
		printf("rule:%i -- %i\n",i,Api->iRules);
		printf("Rule #%i: %s\t%s\t%i\n",i,Api->Rules[i]->method,Api->Rules[i]->pattern,Api->Rules[i]->cmpMethod);
	}
}

int RestApiInit(struct RestApi *Api,int (*defaultCallback)(struct mg_connection *)){
	Api->defaultCallback = defaultCallback;
	
	// one pointer
	Api->Rules = malloc(sizeof(struct RestApiRule *));
	if(Api->Rules == NULL){
		perror("malloc");
		return 0;
	}

	// 0 Rules
	Api->iRules = 0;
	
	return 0;
}

int RestApiDeinit(struct RestApi *Api){
	int i;
	for(i=0;i<Api->iRules;i++){
		free(Api->Rules[i]);
	}
	free(Api->Rules);
	
	Api->iRules = 0;	
	
	return 1;
}

int RestApiAddRule(struct RestApi *Api, char *sPath, char *sMethod, int (*callback)(struct mg_connection *),int cmpMethod){
	
	Api->Rules = realloc(Api->Rules,sizeof(struct RestApiRule *)*(Api->iRules+1));
	if(Api->Rules == NULL){
		perror("realloc");
		return 0;		
	}
	
	Api->Rules[Api->iRules] = malloc(sizeof(struct RestApiRule));
	if(Api->Rules[Api->iRules] == NULL){
		perror("malloc");
		return 0;
	}

	Api->Rules[Api->iRules]->pattern = sPath;
	Api->Rules[Api->iRules]->method = sMethod;
	Api->Rules[Api->iRules]->cmpMethod = cmpMethod ;
	Api->Rules[Api->iRules]->callback = callback;

	Api->iRules++;
	
	return 1;
}

int RestApiMatch(const char *string, char *pattern){
    int    status;
    regex_t    re;

    if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0) {
        return 0;
    }
    status = regexec(&re, string, (size_t) 0, NULL, 0);
    regfree(&re);
    if (status != 0) {
        return 0;
    }
    return 1;
}


int RestApiHandle(struct RestApi *Api,struct mg_connection *conn){
	int i;
	
	for(i=0;i<Api->iRules;i++){
		if(strcmp(Api->Rules[i]->method,conn->request_method) == 0){
		
			if(Api->Rules[i]->cmpMethod == REGEX ){
				if(RestApiMatch(conn->uri,Api->Rules[i]->pattern))
					return Api->Rules[i]->callback(conn);
			}else{
				if(strcmp(conn->uri,Api->Rules[i]->pattern) == 0)
					return Api->Rules[i]->callback(conn);			
			}
		}
	}
	return Api->defaultCallback(conn);
}
