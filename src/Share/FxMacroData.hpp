#pragma once

#include <cctype>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace wt
{
class FxMacroDataUrlBuilder
{
public:
    using Params = std::vector<std::pair<std::string, std::string>>;

    explicit FxMacroDataUrlBuilder(
        const std::string& apiKey,
        const std::string& baseUrl = "https://api.fxmacrodata.com/v1/")
        : _apiKey(apiKey), _baseUrl(normalizeBaseUrl(baseUrl))
    {
    }

    // Headers to send with each request: {"X-API-Key", key}, or empty when no key is set.
    // The key is never added to the URL.
    Params headers() const
    {
        Params out;
        if (!_apiKey.empty())
        {
            out.push_back(std::make_pair("X-API-Key", _apiKey));
        }
        return out;
    }

    std::string requestUrl(const std::string& path, const Params& params = Params()) const
    {
        std::string url = _baseUrl + trimLeadingSlash(path);
        if (!params.empty())
        {
            url += "?";
            for (std::size_t i = 0; i < params.size(); ++i)
            {
                if (i != 0)
                {
                    url += "&";
                }
                url += urlEncode(params[i].first);
                url += "=";
                url += urlEncode(params[i].second);
            }
        }
        return url;
    }

    std::string dataCatalogue(const std::string& currency) const
    {
        return requestUrl("data_catalogue/" + normalizeCurrency(currency));
    }

    std::string announcements(
        const std::string& currency,
        const std::string& indicator,
        const Params& params = Params()) const
    {
        return requestUrl(
            "announcements/" + normalizeCurrency(currency) + "/" + urlEncode(indicator),
            params);
    }

    std::string latestAnnouncements(
        const std::string& currency,
        const Params& params = Params()) const
    {
        return requestUrl(
            "announcements/" + normalizeCurrency(currency) + "/latest",
            params);
    }

    std::string calendar(
        const std::string& currency,
        const Params& params = Params()) const
    {
        return requestUrl("calendar/" + normalizeCurrency(currency), params);
    }

    std::string predictions(
        const std::string& currency,
        const std::string& indicator,
        const Params& params = Params()) const
    {
        return requestUrl(
            "predictions/" + normalizeCurrency(currency) + "/" + urlEncode(indicator),
            params);
    }

    std::string forex(
        const std::string& base,
        const std::string& quote,
        const Params& params = Params()) const
    {
        return requestUrl(
            "forex/" + normalizeCurrency(base) + "/" + normalizeCurrency(quote),
            params);
    }

    std::string cot(const std::string& currency, const Params& params = Params()) const
    {
        return requestUrl("cot/" + normalizeCurrency(currency), params);
    }

    std::string commodity(const std::string& indicator, const Params& params = Params()) const
    {
        return requestUrl("commodities/" + urlEncode(indicator), params);
    }

    std::string commoditiesLatest(const Params& params = Params()) const
    {
        return requestUrl("commodities/latest", params);
    }

    std::string marketSessions(const Params& params = Params()) const
    {
        return requestUrl("market_sessions", params);
    }

    std::string riskSentiment(const Params& params = Params()) const
    {
        return requestUrl("risk_sentiment", params);
    }

    std::string pressReleases(
        const std::string& currency,
        const Params& params = Params()) const
    {
        return requestUrl("press-releases/" + normalizeCurrency(currency), params);
    }

private:
    static std::string normalizeBaseUrl(const std::string& baseUrl)
    {
        const auto invalid = []() {
            throw std::invalid_argument("FXMacroData base URL must be an HTTP(S) URL with a host and no credentials, query or fragment");
        };
        const std::size_t schemeEnd = baseUrl.find("://");
        if (schemeEnd == std::string::npos)
        {
            invalid();
        }
        std::string scheme = baseUrl.substr(0, schemeEnd);
        for (char& ch : scheme)
        {
            if (ch >= 'A' && ch <= 'Z') ch = static_cast<char>(ch + ('a' - 'A'));
        }
        if (scheme != "http" && scheme != "https") invalid();
        for (std::size_t i = 0; i < baseUrl.size(); ++i)
        {
            const unsigned char ch = static_cast<unsigned char>(baseUrl[i]);
            if (ch <= 0x20 || ch >= 0x7f || ch == '\\' || ch == '?' || ch == '#') invalid();
            if (ch == '%')
            {
                const auto hex = [](char value) {
                    return (value >= '0' && value <= '9') || (value >= 'a' && value <= 'f') || (value >= 'A' && value <= 'F');
                };
                if (i + 2 >= baseUrl.size() || !hex(baseUrl[i + 1]) || !hex(baseUrl[i + 2])) invalid();
            }
        }
        const std::size_t hostStart = schemeEnd + 3;
        const std::size_t pathStart = baseUrl.find('/', hostStart);
        const std::string authority = baseUrl.substr(hostStart, pathStart - hostStart);
        if (authority.empty() || authority.find('@') != std::string::npos) invalid();
        std::string host;
        std::string port;
        bool hasPort = false;
        if (authority[0] == '[')
        {
            const std::size_t close = authority.find(']');
            if (close == std::string::npos || close == 1) invalid();
            host = authority.substr(1, close - 1);
            if (host.find(':') == std::string::npos) invalid();
            for (char ch : host)
            {
                if (!((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F') || ch == ':' || ch == '.')) invalid();
            }
            if (close + 1 < authority.size())
            {
                if (authority[close + 1] != ':') invalid();
                hasPort = true;
                port = authority.substr(close + 2);
            }
        }
        else
        {
            const std::size_t colon = authority.find(':');
            host = authority.substr(0, colon);
            hasPort = colon != std::string::npos;
            if (hasPort) port = authority.substr(colon + 1);
            if (host.empty()) invalid();
            for (char ch : host)
            {
                if (!((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '.' || ch == '-' || ch == '_')) invalid();
            }
        }
        if (hasPort)
        {
            if (port.empty() || port.size() > 5) invalid();
            unsigned int number = 0;
            for (char ch : port)
            {
                if (ch < '0' || ch > '9') invalid();
                number = number * 10 + static_cast<unsigned int>(ch - '0');
            }
            if (number == 0 || number > 65535) invalid();
        }
        return baseUrl.back() == '/' ? baseUrl : baseUrl + "/";
    }

    static std::string trimLeadingSlash(const std::string& path)
    {
        std::size_t first = 0;
        while (first < path.size() && path[first] == '/')
        {
            ++first;
        }
        return path.substr(first);
    }

    static std::string normalizeCurrency(const std::string& currency)
    {
        std::string out;
        for (std::size_t i = 0; i < currency.size(); ++i)
        {
            const unsigned char ch = static_cast<unsigned char>(currency[i]);
            if (!std::isspace(ch))
            {
                out.push_back(static_cast<char>(std::tolower(ch)));
            }
        }
        return urlEncode(out);
    }

    static std::string urlEncode(const std::string& value)
    {
        std::ostringstream out;
        out.fill('0');
        out << std::hex << std::uppercase;
        for (std::size_t i = 0; i < value.size(); ++i)
        {
            const unsigned char ch = static_cast<unsigned char>(value[i]);
            if (std::isalnum(ch) || ch == '-' || ch == '_' || ch == '.' || ch == '~')
            {
                out << static_cast<char>(ch);
            }
            else
            {
                out << '%' << std::setw(2) << static_cast<int>(ch);
            }
        }
        return out.str();
    }

    std::string _apiKey;
    std::string _baseUrl;
};
} // namespace wt
