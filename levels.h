#pragma once

// ===== 关卡数据（数据层）=====
// 只负责声明关卡数据结构，不含任何游戏逻辑
// 数组定义在 levels.cpp

// 地图符号说明
// # 墙壁（不能走）
// @ 玩家
// $ 箱子
// . 终点（目标）
// * 箱子在终点上（胜利标记）
// + 玩家站在终点上
// 空格 空地

const int ROW = 10;        // 关卡行数
const int COL = 11;        // 关卡列数
const int MAX_LEVEL = 3;   // 关卡总数

// 3 关的关卡数据（定义见 levels.cpp）
extern char levels[MAX_LEVEL][ROW][COL];
