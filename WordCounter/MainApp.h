// Copyright (C) 2023 Avnish Kirnalli.

#pragma once
#include "Gui/App/App.h"
#include "Gui/Platform/Windows/WindowsPlatform.h"

class MainApp : public App
{
public:
    MainApp();
    
    void Render() override;
    void Update() override;

    void OnCopy();
    void SetWindowExtended(bool bNewExtended);

private:
    WindowsPlatform* platform;
    double lastCopyTime;
    double copyInterval = 1.0;
    uint32_t wordCount = 0;
    uint64_t charCount = 0;
    bool bAuto = false;
    bool bExtended = false;
    bool bTogglePending = false;
};
