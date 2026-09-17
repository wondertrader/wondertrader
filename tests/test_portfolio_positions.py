#!/usr/bin/env python3
"""Compile actual position creation/profit aggregation bodies without broker dependencies.
Run: python3 tests/test_portfolio_positions.py [--tsan]
This is an isolated regression test, not a substitute for the Windows DLL build.
"""
from pathlib import Path
import os
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[1]
source = (ROOT / 'src/WtCore/WtEngine.cpp').read_text(encoding='utf-8-sig')

def function(signature):
    start = source.index(signature)
    opening = source.index('{', start)
    depth = 1
    end = opening + 1
    while depth:
        depth += (source[end] == '{') - (source[end] == '}')
        end += 1
    return source[start:end]

harness = r'''
#include <cassert>
#include <atomic>
#include <cfloat>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include "src/Includes/FasterDefs.h"
using namespace wtp;
using StdRecurMutex = std::recursive_mutex;
struct WTSLogger {
    static std::atomic<int> errors;
    template<class... T> static void error(T&&...) { ++errors; }
};
std::atomic<int> WTSLogger::errors{0};
namespace decimal { bool gt(double a,double b){return a>b;} bool lt(double a,double b){return a<b;} }
struct TimeUtils { static int64_t getLocalTimeNow(){return 10000;} };
struct Pair { uint32_t _date=0; double _dyn_balance=0; };
struct WTSFundStruct {
    uint32_t _last_date=0, _max_time=0, _min_time=0;
    int64_t _update_time=0;
    double _dynprofit=0, _balance=100, _max_dyn_bal=DBL_MAX, _min_dyn_bal=DBL_MAX;
    Pair _max_md_dyn_bal, _min_md_dyn_bal;
};
struct Fund { WTSFundStruct data; WTSFundStruct& fundInfo(){return data;} };
class WtEngine {
public:
    struct PosInfo { double _volume=0, _closeprofit=0, _dynprofit=0; };
    using PosInfoPtr=std::shared_ptr<PosInfo>;
    StdRecurMutex _mtx_positions;
    wt_hashmap<std::string,PosInfoPtr> _pos_map;
    Fund fund;
    Fund* _port_fund=&fund;
    uint32_t _cur_tdate=20260917, _fund_udt_span=0, _cur_raw_time=929, _cur_secs=2000;
    PosInfoPtr get_or_create_position(const char*);
    void update_fund_dynprofit();
};
'''
harness += '\n' + function('WtEngine::PosInfoPtr WtEngine::get_or_create_position(')
harness += '\n' + function('void WtEngine::update_fund_dynprofit()')
harness += r'''
int main() {
    WtEngine e;
    e.update_fund_dynprofit();
    assert(e.fund.data._dynprofit==0);
    auto a=e.get_or_create_position("A");
    a->_dynprofit=12;
    e.get_or_create_position("B")->_dynprofit=-2;
    e.update_fund_dynprofit();
    assert(e.fund.data._dynprofit==10);
    assert(e.fund.data._max_dyn_bal==110);
    assert(e.get_or_create_position("A")==a);
    e._pos_map.emplace("BROKEN",nullptr);
    auto before=e.fund.data;
    auto errors=WTSLogger::errors.load();
    e.update_fund_dynprofit();
    assert(WTSLogger::errors==errors+1);
    assert(e.fund.data._dynprofit==before._dynprofit);
    assert(e.fund.data._update_time==before._update_time);
    assert(e.fund.data._max_dyn_bal==before._max_dyn_bal);
    assert(!e.get_or_create_position("BROKEN"));
    e._pos_map.erase("BROKEN");
    std::atomic<bool> start{false};
    std::vector<std::thread> writers;
    for(int w=0;w<4;++w) writers.emplace_back([&,w]{
        while(!start.load()) std::this_thread::yield();
        for(int i=0;i<4000;++i) {
            std::unique_lock<StdRecurMutex> lock(e._mtx_positions);
            auto key=std::to_string(w)+":"+std::to_string(i);
            auto p=e.get_or_create_position(key.c_str());
            assert(p);
            p->_dynprofit=1;
        }
    });
    std::thread reader([&]{
        start=true;
        for(int i=0;i<4000;++i) e.update_fund_dynprofit();
    });
    for(auto& w:writers) w.join();
    reader.join();
    e.update_fund_dynprofit();
    assert(e._pos_map.size()==16002);
    assert(e.fund.data._dynprofit==16010);
    assert(e.get_or_create_position("A")==a);
}
'''
with tempfile.TemporaryDirectory(prefix='wt-positions-') as directory:
    cpp = Path(directory) / 'test.cpp'
    exe = Path(directory) / 'test'
    cpp.write_text(harness)
    command = [os.environ.get('CXX', 'clang++'), '-std=c++17', '-O1', '-g', '-pthread', '-I', str(ROOT)]
    if '--tsan' in sys.argv:
        command += ['-fsanitize=thread']
    subprocess.run(command + [str(cpp), '-o', str(exe)], check=True)
    subprocess.run([str(exe)], check=True, timeout=60)
print('PASS: empty/normal/null positions, unchanged failed aggregate, concurrent growth and aggregation')
