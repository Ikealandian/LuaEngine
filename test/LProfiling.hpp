//
// Test Profiling Tools
//

#ifndef _LPROFILER_
#define _LPROFILER_

#include <stdlib.h>
#include <stdio.h>
#include <chrono>
#include <map>

// Profile Data
struct Profile
{
    const char* Name;
    double Ms;

    // TODO:
    int Allocations;
    int Allocated;
};

// Profiling
// Creates ScopeProfilers
class Profiler
{
private:
    std::map<const char*, Profile> Profiles;

public:
    Profiler();
    ~Profiler();

    void StartProfiling(const char* _ProfileName);
    void StopProfiling(const char* _ProfileName);

    class ScopeProfiler ProfileThisScope(const char* _ProfileName);

    Profile GetProfileData(const char* _ProfileName);
    void PrintProfileData(const char* _ProfileName);

    void SetProfile(const char* _ProfileName, Profile _Profile);
};

// Start and Stop Times
// TODO: Memory usage change
// Created by Scope
class ScopeProfiler
{
private:
    Profiler* pProfiler;
    
    Profile sProfile;
    std::chrono::steady_clock::time_point pStart, pEnd;

public:

    ScopeProfiler(const char* _ProfileName, Profiler* _Parent);
    ~ScopeProfiler();

};

#endif