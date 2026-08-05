#include "MVVMUIFramework.h"

#define LOCTEXT_NAMESPACE "FMVVMUIFrameworkModule"

void FMVVMUIFrameworkModule::StartupModule()
{
    // 플러그인 초기화 로직
    UE_LOG(LogTemp, Log, TEXT("MVVMUIFramework module has started"));
}

void FMVVMUIFrameworkModule::ShutdownModule()
{
    // 플러그인 종료 로직
    UE_LOG(LogTemp, Log, TEXT("MVVMUIFramework module has shut down"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMVVMUIFrameworkModule, MVVMUIFramework)