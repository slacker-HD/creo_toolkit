﻿(Dialog TestTableDialog
    (Components
        (SubLayout Content)
        (Separator CommitSeparator)
        (PushButton CommitApply)
        (Separator CommitSoftSeparator)
        (PushButton CommitOK)
        (PushButton CommitCancel)
    )
    (Resources
        (CommitSeparator.ResourceHints "Template:OKCancelApplyDialog.CommitSeparator")
        (CommitApply.Label "应用")
        (CommitApply.TopOffset 10)
        (CommitApply.BottomOffset 10)
        (CommitApply.LeftOffset 40)
        (CommitApply.RightOffset 0)
        (CommitApply.ResourceHints "Template:OKCancelApplyDialog.CommitApply" "UserOverrideAttrs:Label")
        (CommitSoftSeparator.Orientation True)
        (CommitSoftSeparator.TopOffset 10)
        (CommitSoftSeparator.BottomOffset 10)
        (CommitSoftSeparator.LeftOffset 8)
        (CommitSoftSeparator.RightOffset 8)
        (CommitSoftSeparator.ResourceHints "Template:OKCancelApplyDialog.CommitSoftSeparator")
        (CommitOK.Label "确定")
        (CommitOK.TopOffset 10)
        (CommitOK.BottomOffset 10)
        (CommitOK.LeftOffset 0)
        (CommitOK.RightOffset 0)
        (CommitOK.ResourceHints "Template:OKCancelApplyDialog.CommitOK" "UserOverrideAttrs:Label")
        (CommitCancel.Label "取消")
        (CommitCancel.TopOffset 10)
        (CommitCancel.BottomOffset 10)
        (CommitCancel.LeftOffset 8)
        (CommitCancel.RightOffset 10)
        (CommitCancel.ResourceHints "Template:OKCancelApplyDialog.CommitCancel" "UserOverrideAttrs:Label")
        (.TopOffset 0)
        (.BottomOffset 0)
        (.LeftOffset 0)
        (.RightOffset 0)
        (.DefaultButton "CommitOK")
        (.ResourceHints "Template:OKCancelApplyDialog" "Guidelines:Creo4-Dialog" "GuidelinesVersion:2" "Version:Creo4" "@Subgrid0x2.Template:OKCancelApplyDialog.CommitBar")
        (.Accelerator "Esc")
        (.Label "批量参数设置")
        (.AttachLeft True)
        (.Layout
            (Grid
                (Rows 1 0 0)
                (Cols 1)
                Content CommitSeparator
                (Grid
                    (Rows 1)
                    (Cols 1 0 0 0 0)
                    (Pos 1 2)
                    CommitApply
                    (Pos 1 3)
                    CommitSoftSeparator
                    (Pos 1 4)
                    CommitOK
                    (Pos 1 5)
                    CommitCancel
                )
            )
        )
    )
)
(Layout Content
    (Components
        (Table TableParams)
    )
    (Resources
        (.AttachLeft True)
        (.AttachRight True)
        (.AttachTop True)
        (.ResourceHints "Template:OKCancelApplyDialog.Content" "UserOverrideAttrs:TopOffset,BottomOffset,LeftOffset,RightOffset")
        (.AttachBottom True)
        (.Layout
            (Grid
                (Rows 1)
                (Cols 1)
                TableParams
            )
        )
    )
)
(TableLayout TableParams
    (Components
        (CheckButton CheckButton1)
        (OptionMenu OptionMenu1)
        (InputPanel InputPanel1)
    )
    (Resources
        (CheckButton1.AttachLeft True)
        (CheckButton1.TopOffset 0)
        (CheckButton1.BottomOffset 0)
        (CheckButton1.LeftOffset 0)
        (CheckButton1.RightOffset 0)
        (OptionMenu1.TopOffset 0)
        (OptionMenu1.BottomOffset 0)
        (OptionMenu1.LeftOffset 0)
        (OptionMenu1.RightOffset 0)
        (OptionMenu1.Names "3" "2" "1")
        (OptionMenu1.Labels "测试1" "测试2" "测试3")
        (InputPanel1.TopOffset 0)
        (InputPanel1.BottomOffset 0)
        (InputPanel1.LeftOffset 0)
        (InputPanel1.RightOffset 0)
        (.TopOffset 4)
        (.BottomOffset 4)
        (.LeftOffset 4)
        (.RightOffset 4)
        (.MinRows 4)
        (.Columns 40)
        (.ResourceHints "UserOverrideAttrs:TopOffset,BottomOffset,LeftOffset,RightOffset")
        (.RowNames "3" "1")
        (.ColumnNames "1" "2")
        (.RowLabels "4" "2")
        (.ColumnLabels "2" "3")
        (.ColumnWidths 3 4)
        (.ShowGrid True)
        (.ColumnResizables 1 1)
    )
)
