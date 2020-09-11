#include "LProfiling.hpp"

// 
// Profiler
//

Profiler::Profiler()
{
}

Profiler::~Profiler()
{
}

void Profiler::StartProfiling(const char* _ProfileName) {}
void Profiler::StopProfiling(const char* _ProfileName) {}

ScopeProfiler Profiler::ProfileThisScope(const char* _ProfileName)
{
    return ScopeProfiler(_ProfileName, this);
}

Profile Profiler::GetProfileData(const char* _ProfileName)
{
    // TODO: Check if exists
    return Profiles[_ProfileName];
}

void Profiler::PrintProfileData(const char* _ProfileName)
{
    Profile P = GetProfileData(_ProfileName);
    printf("Profile: %s\n\tTime:\t%fms\n", _ProfileName, P.Ms);
}

void Profiler::SetProfile(const char* _ProfileName, Profile _Profile)
{
    Profiles[_ProfileName] = _Profile;
}

// 
// Scope Profiler
//

ScopeProfiler::ScopeProfiler(const char* _Profile, Profiler* _Parent)
{
    pProfiler = _Parent;
    sProfile.Name = _Profile;
    pStart = std::chrono::steady_clock::now();
}

ScopeProfiler::~ScopeProfiler()
{
    pEnd = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> ScopeMS = pEnd - pStart;
    sProfile.Ms = ScopeMS.count();
    pProfiler->SetProfile(sProfile.Name, sProfile);
}