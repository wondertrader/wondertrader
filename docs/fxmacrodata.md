# FXMacroData integration helper

`src/Share/FxMacroData.hpp` provides a small C++ URL builder for FXMacroData
REST endpoints. It is header-only so it can be used with the HTTP transport
already selected by a WonderTrader deployment.

```cpp
#include "Share/FxMacroData.hpp"

wt::FxMacroDataUrlBuilder fxmd("YOUR_API_KEY");
std::string url = fxmd.forex("eur", "usd", {{"limit", "100"}, {"offset", "0"}});

// pass the auth header to your HTTP client; the key is never put in the URL
for (const auto& header : fxmd.headers())
{
    request.set(header.first, header.second);  // e.g. your client's header setter
}
```

The API key is sent in the `X-API-Key` request header. USD announcements, the
USD release calendar and the USD data catalogue also work without a key, in
which case `headers()` is empty.

List endpoints (announcements, predictions, forex, COT, commodities) return
20 rows by default and at most 100 per request, newest first. To read a longer
history, request `limit=100` and repeat with `offset` set to
`pagination.next_offset` while `pagination.has_more` is true.

The helper covers macro catalogues, announcements, release calendars,
predictions, FX history, COT positioning, commodities, market sessions, risk
sentiment, and press releases.
