#pragma once

#include <cctype>
#include <iomanip>
#include <sstream>
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

    std::string requestUrl(const std::string& path, const Params& params = Params()) const
    {
        Params merged = params;
        if (!_apiKey.empty())
        {
            merged.push_back(std::make_pair("api_key", _apiKey));
        }

        std::string url = _baseUrl + trimLeadingSlash(path);
        if (!merged.empty())
        {
            url += "?";
            for (std::size_t i = 0; i < merged.size(); ++i)
            {
                if (i != 0)
                {
                    url += "&";
                }
                url += urlEncode(merged[i].first);
                url += "=";
                url += urlEncode(merged[i].second);
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
            "announcements/" + normalizeCurrency(currency) + "/" + indicator,
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
            "predictions/" + normalizeCurrency(currency) + "/" + indicator,
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
        return requestUrl("commodities/" + indicator, params);
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
        if (baseUrl.empty() || baseUrl[baseUrl.size() - 1] == '/')
        {
            return baseUrl;
        }
        return baseUrl + "/";
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
        return out;
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
