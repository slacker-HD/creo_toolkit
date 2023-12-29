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
+ (存在BUG，如果在输入后有弹出MessageBox等操作，会导致系统无法响应消息程序死掉)

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

+ 程序加载自定义Ribbon栏
+ 监听Ribbon栏切换事件
+ 利用宏取消Ribbon栏切换
+ 自定义ribbon界面文件在text/ribbon文件夹下

## AuxiliaryLine

+ 绘制圆中心线

## FrmReplacer

+ 设置图框，同时可以选择是否删除内建表格

## MaterialSet

+ 设置模型材料
+ 读取模型材料信息

## QRCodeGen

+ 生成并添加二维码
+ 二维码生成库调用自[https://github.com/elicec/MFCQRcode](https://github.com/elicec/MFCQRcode)。

## JpgSnapShot

+ 零件或装配体视图转到默认后截图

## MdlRename

+ 修改零件名称，如果有零件目录下存在同名绘图文件，也一并修改

## RotateView

+ 根据给定坐标轴旋转视图

## OutlineCalc

+ 计算指定坐标系下零件轮廓
+ 计算默认坐标系下零件轮廓

## CableTest

+ 根据坐标系布线

## FitTolList

+ 计算全局干涉量
+ 计算选定两个零件的干涉量

## zdpt

+ 自动排图，将工作目录下所有绘图文件加入到一个drw文件中去

## moyu

+ 利用Creo的状态栏~~看小说~~学习。
+ 包含两个工程，C#工程生成对应的Creo消息文件，Toolkit提供按行读取信息功能。
+ 将上面生成的novel_X.txt文件重命名为novel.txt再复制到插件text文件夹下即可。

## CoordTrf

+ 组件坐标转换为装配体坐标
+ 视图坐标变换为屏幕坐标
+ 组件坐标转换为装配体坐标再转换为屏幕坐标
+ 利用转换的坐标在绘图绘制预览直线以验证坐标转换的正确性
 
## DrawingAndView

+ 一键新建或打开同名绘图文件
+ 生成主视图、俯视图和左视图
+ 生成辅助视图
+ 生成详细视图
+ 生成剖视图

## MessageTest

+ 消息区显示各类消息
+ 读取输入消息
+ 清空消息区

## CascadingMenuTest

+ 显示级联式菜单
+ 通过菜单选择选项

## NavigatorpaneTest

+ 添加自定义网址到侧边栏面板
+ 添加自定义对话框到侧边栏面板

## AsyncProjectNOVS

+ 使用makefile而非visual studio异步开发

## SyncProjectNOVS

+ 使用makefile而非visual studio同步开发

## CurrentFolder

+ 打开当前目录

## TrailTest

+ 向Trail文件添加内容

## RenameMdlinAsm

+ 右键快速重命名装配体中组件（重命名组件的同时默认同时重命名组件所在目录下同名工程图（如果存在）,其余的情况不考虑）

## BatOpenFile

+ 批量打开工作目录下所有Prt
+ 批量打开工作目录下所有Drw

## FamInstExport
+ 导出族表文件到实例

## PartShow
+ 快速显示隐藏组件

## IMEHook

+ 使用钩子的方式让非模式对话框中Edit Control和Combo Box及更多的高级控件输入中文
+ 使用.NET主题
+ 存在BUG 1，Combo Box会产生自动全选及输入位置错误
+ 存在BUG 2，如果输入法选字采用的是鼠标点击一样会产生乱码

## LayerSet

+ 将在绘图中添加名为TABLE、NOTE、SYMBOL、SURFFIN、GTOL和DIMENSION六个层，每个层添加对应的元素。如绘图已存在同名层，系统将清空图层后再加入对应元素。

## UnitConvert

+ 一键转换单位系统为mmNs，解释尺寸。
+ 一键转换单位系统为mmNs，转换尺寸。

## TestTableDialog

+ 原生对话框表格控件功能测试

## FltkTest

+ 测试使用Fltk库

## FltkTestNoVS

+ 测试使用Fltk库，使用makefile而非visual studio

## TextFormat

+ 文本格式化

## DrawingScale

+ 设置绘图默认比例

## InsertDxf

+ 工程图插入dxf

## AsyncProjectWithPdcurses

+  使用makefile异步工程调用PDcurses库

## SurfaceTransparent

+ 设置面透明

## InsertNote

+ 工程图插入note

## DimModify

+ 修改尺寸前缀
+ 修改尺寸后缀
+ 修改尺寸文字

## ConfigOptSet

+ 读取内置浏览器主页
+ 设置内置浏览器主页
+ 读取自动加载的程序列表

## TableTextWidth

+ 表格文字自动换行
+ 表格字宽适应单元格宽度

## NoteTextAlign

+ 表格文字对齐（九个方位）

## BOMBalloon

+ 创建球标 - 主视图
+ 创建球标 | 按视图
+ 清理球标
