@echo off

mkdir ..\..\Binary\sdk\ivmp_sdk_module\SDK\Interfaces
xcopy Interfaces\* ..\..\Binary\sdk\ivmp_sdk_module\SDK\Interfaces /Y

mkdir ..\..\Binary\sdk\ivmp_sdk_module\SDK\ModuleNatives
xcopy ModuleNatives\* ..\..\Binary\sdk\ivmp_sdk_module\SDK\ModuleNatives /Y
mkdir ..\..\Binary\sdk\ivmp_sdk_module\SDK\ModuleNatives\Interface
xcopy ModuleNatives\Interface\* ..\..\Binary\sdk\ivmp_sdk_module\SDK\ModuleNatives\Interface /Y

mkdir ..\..\Binary\sdk\ivmp_sdk_module\SDK\Shared
xcopy ..\..\Shared\* ..\..\Binary\sdk\ivmp_sdk_module\SDK\Shared /Y /E