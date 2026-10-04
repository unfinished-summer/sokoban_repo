#pragma once

#include <string>
#include <vector>

// ===== 关卡加载器（数据层）=====
// 读取 Tiled 导出的 JSON 关卡文件，解析成游戏地图

// 图块编号 → 推箱子符号
// 0=空地(空格)  1=墙(#)  2=玩家(@)  3=箱子($)
// 4=终点(.)     5=箱子在终点(*)  6=玩家在终点(+)
char tileToSymbol(int tileId);

// 从 JSON 文件加载一关，返回 map（行 vector，每行是列）
// 尺寸由文件里的 width/height 决定，不写死
std::vector<std::vector<char>> loadLevelFromJson(const std::string& path);

