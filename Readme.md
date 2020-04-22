# 简单说明

一些Creo Toolkit开发例子。开发环境为Creo 2.0 M060(X64) + VS2010。

文档可参见： [我的博客](http://www.hudi.site/)。

## AsyncProject

+ 最简单的Creo Toolkit异步开发配置，不使用MFC

## MfcAsync

+ 最简单的Creo Toolkit异步开发配置，使用MFC

## RelFunctions

+ 在关系里面添加字符串转数字函数，函数名为IMI_StrToDouble
+ 在关系里面添加数字转字符串函数，函数名为IMI_DoubleToStr

## LuaTest

+ 嵌入Lua并调用Lua函数返回结果

## SqliteTest

+ 嵌入Sqlite数据库
+ 打开一个数据库并返回select结果

## OneKeyalign

+ 一键对齐符号和尺寸

## TimerSaver

+ 定时保存文件

## ToolkitMacro

+ 宏的高级操作，运行完包含对话框操作、重生、刷新窗口的宏还可以再运行自定义代码

## Dashboard

+ Dashboard界面操作实例

## ImiProTKDLl

+ 调用自定义dll的例子

## Protktooltips

+ 非模式对话框显示TOOLTIPS

## unicodechineseinput

+ 非模式对话框中Edit Control和Combo Box输入中文
+ 使用.NET主题

## originview

+ 原生对话框嵌入MFC控件及交互

## ColorPaint

+ 一键对装配体所有零件随机着色

## ModelPurge

+ 一键清除工作目录下PRT、ASM以及DRW旧版本，文件移到回收站而不是直接删除

## CreoAccess

+ 二次开发连接Access数据库示例
+ 需要在[https://www.microsoft.com/zh-cn/download/confirmation.aspx?id=13255](https://www.microsoft.com/zh-cn/download/confirmation.aspx?id=13255)下载对应的数据库引擎。

## ToolkitCsharp

+ 二次开发调用C#类库实例，c#窗体可以和Toolkit交互，本例是调用保存模型功能。

## CreoMenuExample

+ 添加普通菜单项
+ 添加CheckBox菜单项
+ 添加RadioBox菜单项
+ 添加右键菜单项
+ 所有菜单项同时可添加到Ribbon中，自定义ribbon界面文件在text/ribbon文件夹下

## RibbonExample

+ 程序加载自定义Ribbon界面
+ 监听Ribbon界面切换事件
+ 利用宏取消Ribbon界面切换
+ 自定义ribbon界面文件在text/ribbon文件夹下
