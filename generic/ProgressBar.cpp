#include "ProgressBar.h"
#include <iostream>

void ProgressBar::Print() {
    unsigned int pos = mBarWidth * mProgress;
    std::wcout << L"[";
    for (unsigned int i = 0; i < mBarWidth; ++i) {
        if (i < pos) std::wcout << L"=";
        else if (i == pos) std::wcout << L">";
        else std::wcout << L" ";
    }
    std::wcout << L"] " << mPercentage << L" %\r";
    std::wcout.flush();
}

ProgressBar::ProgressBar(unsigned int count) {
    mStep = (count > 1) ? (1.0f / count) : 1.0f;
    Update(0.0f);
}

ProgressBar::~ProgressBar() {
    Update(1.0f);
    std::wcout << std::endl;
}

void ProgressBar::Update(float progress) {
    mProgress = progress;
    unsigned int percentage = mProgress * 100.0f;
    if (!mWasDrawn || percentage > mPercentage) {
        mWasDrawn = true;
        mPercentage = percentage;
        Print();
    }
}

void ProgressBar::Step() {
    Update(mProgress + mStep);
}
