#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FMVVMUIFrameworkModule : public IModuleInterface
{
public:
    // 플러그인이 메모리에 로드될 때 실행되는 함수
    virtual void StartupModule() override;
    
    // 플러그인이 언로드될 때 실행되는 함수 (정리 작업)
    virtual void ShutdownModule() override;
};