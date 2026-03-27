#ifndef GOOFING_AROUND_IN_CPP_CALLBACKS_H
#define GOOFING_AROUND_IN_CPP_CALLBACKS_H

#include <libwebsockets.h>

typedef int ( *callback )
	( struct lws *, enum lws_callback_reasons, void *, void *, size_t );

int callback_dumb_increment ( struct lws * wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len );



#endif //GOOFING_AROUND_IN_CPP_CALLBACKS_H
