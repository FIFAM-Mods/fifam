#pragma once

class ProgressBar {
    const unsigned int mBarWidth = 70;
    bool mWasDrawn = false;
    unsigned int mPercentage = 0;
    float mProgress = 0.0f;
    float mStep = 0.0f;

    void Print();
public:
    ProgressBar(unsigned int count);
    ~ProgressBar();
    void Update(float progress);
    void Step();
};
