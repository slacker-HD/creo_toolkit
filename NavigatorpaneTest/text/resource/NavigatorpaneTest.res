(Layout NavigatorpaneTest
    (Components
        (Label                          Info)
        (PushButton                     BtnName)
        (PushButton                     BtnDialog)
    )

    (Resources
        (Info.Label                 "获取当前打开模型信息")
        (BtnName.Label              "当前模型名称")
        (BtnDialog.Label            "显示自定义对话框")
        (.Layout
            (Grid (Rows 1 1) (Cols 1 1 1)
                (Pos 1 2)
                Info
                (Pos 2 1)
                BtnName
                (Pos 2 3)
                BtnDialog
            )
        )
    )
)
