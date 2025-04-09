#pragma once

constexpr int WINDOW_SIZE = 600;
constexpr double LINE_WIDTH = 3.0;
constexpr double BLACK[3] = {0, 0, 0};

constexpr double STRESS_VIEWPORT[4] = {0.0, 0.0, 0.0, 1.0};
constexpr double STRAIN_VIEWPORT[4] = {0.0, 0.0, 0.5, 1.0};
constexpr double DEFORMED_SHAPE_VIEWPORT[4] = {0.5, 0.0, 1.0, 1.0};

constexpr const char* STRESS_NAME = "stress";
constexpr const char* STRAIN_NAME = "strain";