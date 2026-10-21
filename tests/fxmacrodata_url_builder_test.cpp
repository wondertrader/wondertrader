// Standalone regression checks: g++ -std=c++11 -Wall -Wextra -Werror
// -Isrc/Share tests/fxmacrodata_url_builder_test.cpp -o fxmacrodata-test
#include "FxMacroData.hpp"

#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {
void expect(const std::string& actual, const std::string& expected) {
    if (actual != expected) {
        throw std::runtime_error("unexpected URL or header");
    }
}
}

int main() {
    const std::string base = "https://example.invalid/v1/";
    const wt::FxMacroDataUrlBuilder client("sample-test-key", base);
    const std::string indicator = "inflation/other?limit=1#detail";
    const std::string encoded = "inflation%2Fother%3Flimit%3D1%23detail";
    const auto invalidBase = [](const std::string& url) {
        try {
            const wt::FxMacroDataUrlBuilder invalid("sample-test-key", url);
            (void)invalid;
        } catch (const std::invalid_argument& error) {
            if (std::string(error.what()).find("sample-test-key") != std::string::npos)
                throw std::runtime_error("base URL diagnostic contains a credential");
            return;
        }
        throw std::runtime_error("invalid base URL accepted");
    };
    const std::vector<std::pair<std::string, std::function<void()>>> tests = {
        {"canonical announcement", [&]() {
            expect(client.announcements("USD", "inflation"), base + "announcements/usd/inflation");
        }},
        {"query encoding", [&]() {
            expect(client.calendar("USD", {{"note", "a & b"}}), base + "calendar/usd?note=a%20%26%20b");
        }},
        {"header key stays out of URL", [&]() {
            const auto headers = client.headers();
            if (headers.size() != 1) throw std::runtime_error("missing header");
            expect(headers[0].first, "X-API-Key");
            expect(headers[0].second, "sample-test-key");
            expect(client.forex("EUR", "USD"), base + "forex/eur/usd");
        }},
        {"currency normalization", [&]() {
            expect(client.calendar(" U S D "), base + "calendar/usd");
        }},
        {"announcement indicator is one segment", [&]() {
            expect(client.announcements("USD", indicator), base + "announcements/usd/" + encoded);
        }},
        {"prediction indicator is one segment", [&]() {
            expect(client.predictions("USD", indicator), base + "predictions/usd/" + encoded);
        }},
        {"commodity indicator is one segment", [&]() {
            expect(client.commodity(indicator), base + "commodities/" + encoded);
        }},
        {"currency is one segment", [&]() {
            expect(client.calendar("US/D?x=1#frag"), base + "calendar/us%2Fd%3Fx%3D1%23frag");
        }},
        {"empty base", [&]() { invalidBase(""); }},
        {"non-HTTP base", [&]() { invalidBase("ftp://example.invalid/v1/"); }},
        {"missing host", [&]() { invalidBase("https:///v1/"); }},
        {"credential-bearing base", [&]() { invalidBase("https://user:sample-test-key@example.invalid/v1/"); }},
        {"query-bearing base", [&]() { invalidBase("https://example.invalid/v1/?token=sample-test-key"); }},
        {"fragment-bearing base", [&]() { invalidBase("https://example.invalid/v1/#sample-test-key"); }},
        {"control character in base", [&]() { invalidBase("https://example.invalid/v1/\n"); }},
        {"backslash in base", [&]() { invalidBase("https://example.invalid\\other/v1/"); }},
        {"invalid port", [&]() { invalidBase("https://example.invalid:abc/v1/"); }},
        {"unclosed IPv6 base", [&]() { invalidBase("https://[::1/v1/"); }},
        {"invalid percent escape", [&]() { invalidBase("https://example.invalid/%xx/"); }},
        {"HTTP port", [&]() { expect(wt::FxMacroDataUrlBuilder("", "http://127.0.0.1:8080/v1").calendar("USD"), "http://127.0.0.1:8080/v1/calendar/usd"); }},
        {"IPv6 port", [&]() { expect(wt::FxMacroDataUrlBuilder("", "https://[::1]:8443/v1").calendar("USD"), "https://[::1]:8443/v1/calendar/usd"); }},
        {"upper-case scheme", [&]() { expect(wt::FxMacroDataUrlBuilder("", "HTTPS://example.invalid/v1").calendar("USD"), "HTTPS://example.invalid/v1/calendar/usd"); }},
        {"escaped base path", [&]() { expect(wt::FxMacroDataUrlBuilder("", "https://example.invalid/a%20b/v1").calendar("USD"), "https://example.invalid/a%20b/v1/calendar/usd"); }},
    };
    int failures = 0;
    for (const auto& test : tests) {
        try {
            test.second();
            std::cout << "PASS " << test.first << '\n';
        } catch (const std::exception&) {
            ++failures;
            std::cout << "FAIL " << test.first << '\n';
        }
    }
    std::cout << "SUMMARY passed=" << tests.size() - failures << " failed=" << failures << '\n';
    return failures == 0 ? 0 : 1;
}
