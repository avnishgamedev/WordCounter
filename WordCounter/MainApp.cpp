// Copyright (C) 2023 Avnish Kirnalli.

#include "MainApp.h"

#include "Gui/Font.h"
#include "Gui/Gui.h"

#include "Gui/GuiHelper.h"
#include "Gui/Themes.h"

#include "Gui/Platform/Platform.h"

#include <format>
#include <sstream>

int CountWords(const std::string& text)
{
    std::istringstream stream(text);
    int count = 0;
    std::string word;
    while (stream >> word)
    {
        ++count;
    }
    return count;
}

MainApp::MainApp() : App("Word Count", 120, 85) // <-- Put your App Name here.
{
    // Check All the available themes in Themes.h and apply any as shown below.
    Theme::Comfy();

    // Loading Font from Assets folder
    //Gui::PushFont(GuiFont("C:\\Windows\\Fonts\\Arial.ttf", 18.0f));
    ImGuiIO& io = ImGui::GetIO();
    ImVector<ImWchar> ranges;
    ImFontGlyphRangesBuilder builder;
    builder.AddChar(0x2191);                               // Add a specific character
    builder.AddChar(0x2193);                               // Add a specific character
    builder.AddRanges(io.Fonts->GetGlyphRangesDefault()); // Add one of the default ranges
    builder.BuildRanges(&ranges);                          // Build the final result (ordered ranges with all the unique characters submitted)
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 16.0f, nullptr, ranges.Data);

    platform = dynamic_cast<WindowsPlatform*>(Platform::Get());
    lastCopyTime = ImGui::GetTime();

    platform->onCopy = std::bind(&MainApp::OnCopy, this);

    SetWindowExtended(false);
}

void MainApp::Render()
{
    // Add all the UI Components here.
    // Use ImGui namespace for default ImGui UI components.
    // Use GH namespace for advanced UI components.
    
    GH::TextCenterColored(std::format("Words: {}", wordCount), ImVec4(0.0f, 0.66f, 1.0f, 1.0f));
    GH::TextCenterColored(std::format("Letters: {}", charCount), ImVec4(0.0f, 0.66f, 1.0f, 1.0f));

    if (bExtended)
    {
        {
            // Get window width for centering
            float windowWidth = ImGui::GetWindowSize().x;
        
            // ----- X -----
            {
                const char* label = "X: ";
                const char* status = (wordCount > 280) ? "Invalid" : "Valid";
                ImVec4 color = (wordCount > 280) ? ImVec4(1, 0, 0, 1) : ImVec4(0, 0, 1, 1);

                ImVec2 labelSize = ImGui::CalcTextSize(label);
                ImVec2 statusSize = ImGui::CalcTextSize(status);
                float totalWidth = labelSize.x + ImGui::GetStyle().ItemSpacing.x + statusSize.x;

                ImGui::SetCursorPosX((windowWidth - totalWidth + 15.0f) * 0.5f);
                ImGui::TextUnformatted(label);
                ImGui::SameLine();
                ImGui::TextColored(color, "%s", status);
            }

            // ----- Snap -----
            {
                const char* label = "Snap: ";
                const char* status = (wordCount > 150) ? "Invalid" : "Valid";
                ImVec4 color = (wordCount > 150) ? ImVec4(1, 0, 0, 1) : ImVec4(0, 0, 1, 1);

                ImVec2 labelSize = ImGui::CalcTextSize(label);
                ImVec2 statusSize = ImGui::CalcTextSize(status);
                float totalWidth = labelSize.x + ImGui::GetStyle().ItemSpacing.x + statusSize.x;

                ImGui::SetCursorPosX((windowWidth - totalWidth - 11.0f) * 0.5f);
                ImGui::TextUnformatted(label);
                ImGui::SameLine();
                ImGui::TextColored(color, "%s", status);
            }
        }

        {
            float windowWidth = ImGui::GetWindowSize().x;
            ImVec2 textSize = ImGui::CalcTextSize("Auto?");
            float checkboxWidth = textSize.x + ImGui::GetFrameHeight(); // include box width
            ImGui::SetCursorPosX((windowWidth - checkboxWidth) * 0.5f);
            ImGui::Checkbox("Auto?", &bAuto);
        }
    }

    {
        // Add small arrow button at bottom
        float windowWidth = ImGui::GetWindowSize().x;
        float buttonWidth = 20.0f;  // Small button width
        float buttonHeight = 20.0f; // Small button height
    
        // Position the button at the bottom center
        ImGui::SetCursorPosX(0.0f);
        ImGui::SetCursorPosY(ImGui::GetWindowSize().y - buttonHeight - 14);
    
        // Remove button padding and background
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.2f, 0.2f, 0.5f));

        if (ImGui::Button(bExtended ? "↑" : "↓", ImVec2(buttonWidth, buttonHeight)))
        {
            bTogglePending = true;
        }
    
        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar();
    }
}

void MainApp::Update()
{
    App::Update();

    if (bAuto)
    {
        if (ImGui::GetTime() - lastCopyTime > copyInterval)
        {
            platform->SimulateCtrlC();
            wordCount = CountWords(platform->GetClipboardText());
            charCount = platform->GetClipboardText().length();
            lastCopyTime = ImGui::GetTime();
        }
    }

    if (bTogglePending)
    {
        SetWindowExtended(!bExtended);
        bTogglePending = false;
    }
}

void MainApp::OnCopy()
{
    wordCount = CountWords(platform->GetClipboardText());
    charCount = platform->GetClipboardText().length();
}

void MainApp::SetWindowExtended(bool bNewExtended)
{
    bExtended = bNewExtended;
    if (bExtended)
    {
        Gui::WIDTH = 120;
        Gui::HEIGHT = 145;
    }
    else
    {
        Gui::WIDTH = 120;
        Gui::HEIGHT = 80;
    }

    // Use the new resize function
    if (platform)
    {
        platform->ResizeWindow(Gui::WIDTH, Gui::HEIGHT);
    }
}
