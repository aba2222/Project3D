
# Project3D

## 项目简介

Project3D 希望成为一个用于模拟多种客机的飞行模拟程序

## 项目组成
### 1. Win32部分

包含程序的入口点和对键盘鼠标还有窗口的处理

- `WinMain`：程序入口点。
- `Window`：负责创建窗口，处理消息。
- `App` : 启动后的主程序。
- `KeyBoard` 和 `Mouse`：鼠标键盘。

### 2. DirectX部分

采用DirectX11，主程序位于`Graphics.cpp`。

### 3. SimObject 文件夹（设想中）

包含用于管理和模拟飞行物体的类。

- `SimObjManager`：用于管理多个模拟对象。
- `SimObjectBase`：飞行物体的基类，包含对物理、控制和模型显示的综合管理。
- `ObjForces` 文件夹：包含物理模拟类，如升力、推力、重力和阻力的计
- `Model` 文件夹：包含模型渲染类，用于显示飞机模型。
- `Control` 文件夹：包含 `FlightControlComputer` 类的实现。

## 编译和运行

本项目使用Visual Studio 2022进行编写和编译。
