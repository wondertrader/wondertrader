#pragma once

#include <functional>

// 作用域防护
class ScopeGuard
{
public:
  explicit ScopeGuard( std::function < void() > p_onExitScope )
    : m_onExitScope( p_onExitScope )
    , m_bDismissed( false )
  {
  }

  ~ScopeGuard()
  {
    if ( !m_bDismissed )
    {
      m_onExitScope();
    }
  }

  void Dismiss() // 解除防护
  {
    m_bDismissed = true;
  }

private:
  std::function < void() > m_onExitScope;
  bool m_bDismissed;

private: // noncopyable
  ScopeGuard(ScopeGuard const&) = delete;
  ScopeGuard& operator=(ScopeGuard const&) = delete;
};

