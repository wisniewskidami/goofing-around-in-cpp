g++ main.cpp callbacks.cpp Protocol.cpp WebSockets.cpp Runnable.hpp -lwebsockets -luuid -std=c++20

g++ wtf.cpp callbacks.cpp Protocol.cpp WebSockets.cpp Runnable.hpp -lwebsockets -luuid -pthread -lcrypto -DDEMO -std=c++20

```bash
>$ wscat wss://ws.bitstamp.net/
Connected (press CTRL+C to quit)
> { "event": "bts:subscribe","data": { "channel": "diff_order_book_btcusd"}}
```