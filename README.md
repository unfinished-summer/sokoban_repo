# Sokoban 推箱子小游戏

基于 C++ 控制台的经典推箱子游戏，仅依赖 Windows 系统 API 与标准 C++ 库，无需图形引擎，原生控制台运行。内置 4 个递进难度关卡，支持 WASD 移动、关卡重置、通关自动切关、全局通关判定，采用光标定位刷新画面，解决全屏清屏闪烁问题。关卡数据使用 Tiled 导出的 JSON 文件独立存放，数据层与游戏逻辑层分离。

## 符号规则

| 符号 | 含义 |
| --- | --- |
| `#` | 墙壁，不可通行 |
| `@` | 玩家（站在空地） |
| `+` | 玩家站在终点格子 |
| `$` | 箱子（空地之上） |
| `*` | 箱子推至终点（完成目标） |
| `.` | 终点目标格子 |
| 空格 | 空白可走空地 |

关卡数据使用 Tiled 图块编号存储（JSON 内 `data` 数组）：

| 编号 | 含义 |
| --- | --- |
| 0 | 空地 |
| 1 | 墙 |
| 2 | 玩家 |
| 3 | 箱子 |
| 4 | 终点 |
| 5 | 箱子在终点 |
| 6 | 玩家在终点 |

## 操作说明

- `W` / `w`：向上移动
- `S` / `s`：向下移动
- `A` / `a`：向左移动
- `D` / `d`：向右移动
- `R` / `r`：重置当前关卡
- `Q` / `q`：退出游戏

## 游戏规则

1. 将所有箱子全部推到终点 `.` 上（全部变为 `*`）即通关本关；
2. 箱子无法穿过墙壁、其他箱子，仅前方为空地/终点时才能推动；
3. 单关通关后自动加载下一关卡，全部 4 关完成则游戏胜利结束；
4. 操作失误可按 R 一键重置当前关卡初始布局。

## 项目结构

```
sokoban_repo/
├── sokoban.cpp          # 游戏逻辑（动态地图、移动、推箱、通关判定）
├── level_loader.h/.cpp  # 关卡加载器：解析 Tiled JSON → 游戏地图
├── levels/
│   ├── level_1.json     # Tiled 导出的关卡数据（第 1 关）
│   ├── level_2.json     # 第 2 关
│   ├── level_3.json     # 第 3 关
│   └── level_4.json     # 第 4 关（Tiled 图形化）
├── CMakeLists.txt       # 构建配置（FetchContent 自动拉取 nlohmann-json）
└── README.md            # 项目说明
```

## 依赖与构建

### 依赖

- 标准库：`<iostream>` 输入输出、`<thread>` `<chrono>` 按键休眠防高占用、`<vector>` `<string>` 动态地图
- Windows 专属：`<windows.h>` 控制台光标控制、`<conio.h>` `_kbhit/_getch` 无阻塞按键检测
- JSON 解析：nlohmann-json（header-only 单头文件库，由 CMake FetchContent 自动下载，无需手动安装）

### 构建要求

1. 仅支持 **Windows 系统**（使用 Windows Console API，Linux/macOS 无法直接运行）；
2. 需要 **CMake 3.10+** 与支持 C++20 的编译器（Visual Studio 2019+ / MinGW 均可）；
3. 首次构建需联网下载 nlohmann-json（约 1 分钟）。

### 构建运行（Visual Studio）

1. 用 VS 打开 `sokoban_repo` 文件夹；
2. 顶部选择 x64-Debug，点击「生成」（首次自动下载依赖）；
3. 按 `F5` 运行。

### 构建运行（命令行）

```
cmake -B build
cmake --build build
build\sokoban.exe
```

## 核心功能亮点

1. **无闪烁画面刷新**：使用 `gotoxy` 光标定位至窗口左上角重绘地图，摒弃繁琐清屏，消除画面闪烁；
2. **无阻塞按键检测**：`_kbhit` 轮询按键，搭配线程休眠降低 CPU 占用；
3. **数据层与逻辑层分离**：关卡数据独立存放在 Tiled JSON 文件中，游戏代码只负责读取与逻辑，新增关卡不需要改代码逻辑；
4. **动态地图尺寸**：地图宽高由 JSON 文件决定，用 `vector<vector<char>>` 动态存储，不再写死常量；
5. **健壮的错误处理**：关卡文件缺失/损坏时提示错误并安全退出，不再直接崩溃；
6. **完整边界校验**：墙壁阻挡、箱子推动逻辑、玩家/箱子与终点叠加状态完整处理；
7. **通关自动跳转**：单关完成自动加载下一关，全部关卡通关给出胜利提示；
8. 隐藏控制台光标，界面干净整洁。

## 拓展修改指南

### 1. 新增关卡（推荐用 Tiled 编辑）

1. 用 [Tiled](https://www.mapeditor.org/) 绘制新地图（图块编号见上文符号规则表）；
2. 「文件 → 导出为 JSON」保存到 `levels/level_5.json`；
3. 在 `sokoban.cpp` 的 `levelFiles` 列表末尾加一行 `"levels/level_5.json"`；
4. 重新生成并运行。

### 2. 修改地图尺寸

直接改 JSON 文件里的 `width` / `height` 与 `data` 数组即可，**无需改任何代码**。

### 3. 调整按键响应速度

修改 `sleep_for(chrono::milliseconds(20))` 内数字，数值越大响应越慢。

### 4. 自定义胜利提示

修改全部关卡通关后的输出文本。

## 技术要点

- **Tiled JSON 格式**：`layers[0].data` 是按行优先排列的图块编号数组，元素下标 = `row * width + col`；
- **nlohmann-json**：`json::parse(文件流)` 一步完成解析，`j["layers"][0]["data"][i]` 下标访问，header-only 无需编译；
- **工作目录问题**：构建时通过 `add_custom_command(POST_BUILD)` 把 `levels/` 拷贝到 exe 旁边，确保 exe 与数据文件一起发布即可运行；
- **编码**：源码与 JSON 均使用 UTF-8，程序内调用 `SetConsoleOutputCP(CP_UTF8)` 保证控制台中文正常显示。