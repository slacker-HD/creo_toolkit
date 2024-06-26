﻿(Dialog BatConvertUnit
    (Components
        (SubLayout Content)
        (Separator CommitSeparator)
        (PushButton BatConvertUnitCommitApply)
        (Separator CommitSoftSeparator)
        (PushButton BatConvertUnitCommitOK)
        (PushButton BatConvertUnitCommitCancel)
    )
    (Resources
        (CommitSeparator.ResourceHints "Template:OKCancelApplyDialog.CommitSeparator")
        (BatConvertUnitCommitApply.Label "应用")
        (BatConvertUnitCommitApply.TopOffset 10)
        (BatConvertUnitCommitApply.BottomOffset 10)
        (BatConvertUnitCommitApply.LeftOffset 40)
        (BatConvertUnitCommitApply.RightOffset 0)
        (BatConvertUnitCommitApply.ResourceHints "Template:OKCancelApplyDialog.CommitApply" "UserOverrideAttrs:Label")
        (CommitSoftSeparator.Orientation True)
        (CommitSoftSeparator.TopOffset 10)
        (CommitSoftSeparator.BottomOffset 10)
        (CommitSoftSeparator.LeftOffset 8)
        (CommitSoftSeparator.RightOffset 8)
        (CommitSoftSeparator.ResourceHints "Template:OKCancelApplyDialog.CommitSoftSeparator")
        (BatConvertUnitCommitOK.Label "确定")
        (BatConvertUnitCommitOK.TopOffset 10)
        (BatConvertUnitCommitOK.BottomOffset 10)
        (BatConvertUnitCommitOK.LeftOffset 0)
        (BatConvertUnitCommitOK.RightOffset 0)
        (BatConvertUnitCommitOK.ResourceHints "Template:OKCancelApplyDialog.CommitOK" "UserOverrideAttrs:Label")
        (BatConvertUnitCommitCancel.Label "取消")
        (BatConvertUnitCommitCancel.TopOffset 10)
        (BatConvertUnitCommitCancel.BottomOffset 10)
        (BatConvertUnitCommitCancel.LeftOffset 8)
        (BatConvertUnitCommitCancel.RightOffset 10)
        (BatConvertUnitCommitCancel.ResourceHints "Template:OKCancelApplyDialog.CommitCancel" "UserOverrideAttrs:Label")
        (.TopOffset 0)
        (.BottomOffset 0)
        (.LeftOffset 0)
        (.RightOffset 0)
        (.DefaultButton "BatConvertUnitCommitOK")
        (.ResourceHints "Template:OKCancelApplyDialog" "Guidelines:Creo4-Dialog" "GuidelinesVersion:2" "Version:Creo4" "@Subgrid0x2.Template:OKCancelApplyDialog.CommitBar")
        (.Accelerator "Esc")
        (.Label "批量单位转换为mmNs")
        (.Rows 10)
        (.Columns 15)
        (.AttachLeft True)
        (.MinColumns 10)
        (.MinRows 10)
        (.Layout
            (Grid
                (Rows 1 0 0)
                (Cols 1)
                Content CommitSeparator
                (Grid
                    (Rows 1)
                    (Cols 1 0 0 0 0)
                    (Pos 1 2)
                    BatConvertUnitCommitApply
                    (Pos 1 3)
                    CommitSoftSeparator
                    (Pos 1 4)
                    BatConvertUnitCommitOK
                    (Pos 1 5)
                    BatConvertUnitCommitCancel
                )
            )
        )
    )
)
(Layout Content
    (Components
        (SubLayout Group)
    )
    (Resources
        (.AttachLeft True)
        (.AttachRight True)
        (.AttachTop True)
        (.TopOffset 16)
        (.BottomOffset 16)
        (.LeftOffset 10)
        (.RightOffset 10)
        (.ResourceHints "Template:OKCancelApplyDialog.Content")
        (.AttachBottom True)
        (.Layout
            (Grid
                (Rows 1)
                (Cols 1)
                Group
            )
        )
    )
)
(Layout Group
    (Components
        (Label GroupHeader)
        (Table TableUnit)
        (Label Label1)
        (ProgressBar ProgressBarUnit)
    )
    (Resources
        (GroupHeader.Label "当前工作目录下文件：")
        (GroupHeader.AttachLeft True)
        (GroupHeader.ResourceHints "Template:Group.GroupHeader")
        (GroupHeader.TopOffset 0)
        (GroupHeader.BottomOffset 10)
        (GroupHeader.LeftOffset 0)
        (GroupHeader.RightOffset 0)
        (GroupHeader.FontStyle 8)
        (Label1.Label "转换进度：")
        (Label1.AttachLeft True)
        (Label1.TopOffset 3)
        (Label1.BottomOffset 0)
        (Label1.LeftOffset 10)
        (Label1.RightOffset 4)
        (ProgressBarUnit.AttachLeft True)
        (ProgressBarUnit.AttachRight True)
        (ProgressBarUnit.TopOffset 3)
        (ProgressBarUnit.BottomOffset 0)
        (ProgressBarUnit.LeftOffset 3)
        (ProgressBarUnit.RightOffset 0)
        (.AttachLeft True)
        (.AttachRight True)
        (.AttachTop True)
        (.TopOffset 0)
        (.BottomOffset 0)
        (.LeftOffset 0)
        (.RightOffset 0)
        (.ResourceHints "Template:Group" "@Subgrid0x1.Template:Group.GroupContent")
        (.AttachBottom True)
        (.Layout
            (Grid
                (Rows 0 1)
                (Cols 1)
                GroupHeader
                (Grid
                    (Rows 1 0)
                    (Cols 1)
                    TableUnit
                    (Grid
                        (Rows 0)
                        (Cols 0 1)
                        Label1 ProgressBarUnit
                    )
                )
            )
        )
    )
)
(TableLayout TableUnit
    (Components
        (OptionMenu OptionMenuMethod)
    )
    (Resources
        (OptionMenuMethod.Names "1" "2" "3")
        (OptionMenuMethod.Labels "不转换" "解释尺寸" "转换尺寸")
        (OptionMenuMethod.TopOffset 0)
        (OptionMenuMethod.BottomOffset 0)
        (OptionMenuMethod.LeftOffset 0)
        (OptionMenuMethod.RightOffset 0)
        (.TopOffset 0)
        (.BottomOffset 3)
        (.LeftOffset 10)
        (.RightOffset 0)
        (.ColumnNames "name" "unit" "method")
        (.ColumnLabels "零件" "当前单位" "转换方式")
        (.ShowGrid True)
    )
)
