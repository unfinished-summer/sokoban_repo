#include "level_loader.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>

using json = nlohmann::json;

char tileToSymbol(int tileId)
{
    switch (tileId)
    {
    case 0:  return ' ';   // 空地
    case 1:  return '#';   // 墙
    case 2:  return '@';   // 玩家
    case 3:  return '$';   // 箱子
    case 4:  return '.';   // 终点
    case 5:  return '*';   // 箱子在终点
    case 6:  return '+';   // 玩家在终点
    default:
        throw std::runtime_error("未知图块编号: " + std::to_string(tileId));
    }
}

std::vector<std::vector<char>> loadLevelFromJson(const std::string& path)
{
    // 1. 打开文件
    std::ifstream ifs(path);
    if (!ifs)
        throw std::runtime_error("无法打开关卡文件: " + path);

    // 2. 解析 JSON（nlohmann 直接把文件流喂给 parse）
    json j = json::parse(ifs);

    // 3. 取第一个 tile layer（推箱子只需一层）
    const auto& layer = j["layers"].at(0);
    int w = layer["width"].get<int>();
    int h = layer["height"].get<int>();
    const auto& data = layer["data"];   // 数字数组，按行排列

    // 4. 数字 → 符号，填成二维 map
    std::vector<std::vector<char>> map(h, std::vector<char>(w));
    for (int row = 0; row < h; row++)
        for (int col = 0; col < w; col++)
            map[row][col] = tileToSymbol(data[row * w + col].get<int>());

    return map;
}
