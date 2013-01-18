@echo off

mkdir ..\..\Binary\sdk\ivmp_sdk_module\SDK\Interfaces
xcopy Interfaces\* ..\..\Binary\sdk\ivmp_sdk_module\SDK\Interfaces \Y

mkdir ..\..\Binary\sdk\ivmp_sdk_module\SDK\ModuleNatives
xcopy ModuleNatives\* ..\..\Binary\sdk\ivmp_sdk_module\SDK\ModuleNatives \Y
mkdir ..\..\Binary\sdk\ivmp_sdk_module\SDK\ModuleNatives\Interface
xcopy ModuleNatives\Interface\* ..\..\Binary\sdk\ivmp_sdk_module\SDK\ModuleNatives\Interface \Y

mkdir ..\..\Binary\sdk\ivmp_sdk_module\SDK\Shared
xcopy ..\..\Shared\Common.h ..\..\Binary\sdk\ivmp_sdk_module\SDK\Shared
xcopy ..\..\Shared\CString.h ..\..\Binary\sdk\ivmp_sdk_module\SDK\Shared
xcopy ..\..\Shared\CString.cpp ..\..\Binary\sdk\ivmp_sdk_module\SDK\Shared

mkdir ..\..\Binary\sdk\ivmp_sdk_module\SDK\Shared\Network
xcopy ..\..\Shared\Network\CBitStream.h ..\..\Binary\sdk\ivmp_sdk_module\SDK\Shared\Network
xcopy ..\..\Shared\Network\CBitStream.cpp ..\..\Binary\sdk\ivmp_sdk_module\SDK\Shared\Network

mkdir ..\..\Binary\sdk\ivmp_sdk_module\SDK\Shared\Game
xcopy ..\..\Shared\Game\CControlState.h ..\..\Binary\sdk\ivmp_sdk_module\SDK\Shared\Game
xcopy ..\..\Shared\Game\CControlState.cpp ..\..\Binary\sdk\ivmp_sdk_module\SDK\Shared\Game

mkdir ..\..\Binary\sdk\ivmp_sdk_module\SDK\Shared\Math
xcopy ..\..\Shared\Math\CMath.h ..\..\Binary\sdk\ivmp_sdk_module\SDK\Shared\Math
xcopy ..\..\Shared\Math\CVector3.h ..\..\Binary\sdk\ivmp_sdk_module\SDK\Shared\Math

mkdir ..\..\Binary\sdk\ivmp_sdk_module\SDK\Shared\Scripting
xcopy ..\..\Shared\Scripting\CSquirrelArguments.h ..\..\Binary\sdk\ivmp_sdk_module\SDK\Shared\Scripting
xcopy ..\..\Shared\Scripting\CSquirrelArgument.h ..\..\Binary\sdk\ivmp_sdk_module\SDK\Shared\Scripting