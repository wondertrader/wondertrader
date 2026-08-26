# FXMacroData integration helper

`src/Share/FxMacroData.hpp` provides a small C++ URL builder for FXMacroData
REST endpoints. It is header-only so it can be used with the HTTP transport
already selected by a WonderTrader deployment.

```cpp
#include "Share/FxMacroData.hpp"

wt::FxMacroDataUrlBuilder fxmd("YOUR_API_KEY");
std::string url = fxmd.forex("eur", "usd", {{"limit", "100"}});
```

The helper covers macro catalogues, announcements, release calendars,
predictions, FX history, COT positioning, commodities, market sessions, risk
sentiment, and press releases.
