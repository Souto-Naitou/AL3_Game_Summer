#pragma once

const unsigned int kScreenWidth     = 1280u;
const unsigned int kScreenHeight    = 720u;

struct HitCount
{
    unsigned int perfect;
    unsigned int great;
    unsigned int bad;
};

struct BeatCount
{
    unsigned int quarterCount;      // 4分拍カウント
    unsigned int eighthCount;       // 8分拍カウント
    unsigned int sixteenthCount;    // 16分拍カウント
};

struct BeatDuration
{
    double quarter;     // 4分
    double eighth;      // 8分
    double sixteenth;   // 16分
};