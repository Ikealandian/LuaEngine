#include "LProfiling.hpp"
#include "LAllocTest.hpp"

// 
// Profiler
//

Profiler::Profiler(LAlloc* _Alloc)
    : lAlloc(_Alloc)
{
}

Profiler::~Profiler()
{
}

void Profiler::StartProfiling(const char* _ProfileName) {}
void Profiler::StopProfiling(const char* _ProfileName) {}

ScopeProfiler Profiler::ProfileThisScope(const char* _ProfileName)
{
    return ScopeProfiler(_ProfileName, this, lAlloc);
}

Profile Profiler::GetProfileData(const char* _ProfileName)
{
    // TODO: Check if exists
    return Profiles[_ProfileName];
}

void Profiler::PrintProfileData(const char* _ProfileName)
{
    Profile P = GetProfileData(_ProfileName);
    printf("Profile: %s\n\tTime:\t\t%fms\n\tAllocated:\t%dB\n", _ProfileName, P.Ms, P.Allocated);
}

void Profiler::SetProfile(const char* _ProfileName, Profile _Profile)
{
    Profiles[_ProfileName] = _Profile;
}

// 
// Scope Profiler
//

ScopeProfiler::ScopeProfiler(const char* _Profile, Profiler* _Parent, struct LAlloc* _PAlloc)
{
    pProfiler = _Parent;
    pAlloc = _PAlloc;
    
    sProfile.Name = _Profile;
    aBefore = pAlloc->Allocated;
    
    pStart = std::chrono::steady_clock::now();
}

ScopeProfiler::~ScopeProfiler()
{
    pEnd = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> ScopeMS = pEnd - pStart;

    sProfile.Ms = ScopeMS.count();
    sProfile.Allocated = pAlloc->Allocated - aBefore;

    pProfiler->SetProfile(sProfile.Name, sProfile);
}