(Dialog QRCode
    (Components
        (SubLayout Content)
        (Separator CommitSeparator)
        (PushButton CommitOK)
        (PushButton CommitCancel)
    )
    (Resources
        (CommitSeparator.ResourceHints "Template:OKCancelDialog.CommitSeparator")
        (CommitOK.Label "确定")
        (CommitOK.TopOffset 10)
        (CommitOK.BottomOffset 10)
        (CommitOK.LeftOffset 40)
        (CommitOK.RightOffset 0)
        (CommitOK.ResourceHints "Template:OKCancelDialog.CommitOK" "UserOverrideAttrs:Label")
        (CommitCancel.Label "取消")
        (CommitCancel.TopOffset 10)
        (CommitCancel.BottomOffset 10)
        (CommitCancel.LeftOffset 8)
        (CommitCancel.RightOffset 10)
        (CommitCancel.ResourceHints "Template:OKCancelDialog.CommitCancel" "UserOverrideAttrs:Label")
        (.TopOffset 0)
        (.BottomOffset 0)
        (.LeftOffset 0)
        (.RightOffset 0)
        (.DefaultButton "CommitOK")
        (.ResourceHints "Template:OKCancelDialog" "Guidelines:Creo4-Dialog" "GuidelinesVersion:2" "Version:Creo4" "@Subgrid0x2.UserOverrideCols:111" "@Subgrid0x2.Template:OKCancelDialog.CommitBar")
        (.Accelerator "Esc")
        (.AttachLeft True)
        (.Layout
            (Grid
                (Rows 1 0 0)
                (Cols 1)
                Content CommitSeparator
                (Grid
                    (Rows 1)
                    (Cols 1 0 0)
                    (Pos 1 2)
                    CommitOK
                    (Pos 1 3)
                    CommitCancel
                )
            )
        )
    )
)

(Layout Content
    (Components
        (Label Label1)
        (TextArea TextAreaInput)
    )
    (Resources
        (Label1.Label "请输入文字：")
        (Label1.AttachLeft True)
        (Label1.TopOffset 0)
        (Label1.BottomOffset 2)
        (Label1.LeftOffset 0)
        (Label1.RightOffset 0)
        (TextAreaInput.MaxLen 256)
        (TextAreaInput.TopOffset 1)
        (TextAreaInput.BottomOffset 0)
        (TextAreaInput.LeftOffset 0)
        (TextAreaInput.RightOffset 0)
        (.AttachLeft True)
        (.AttachRight True)
        (.AttachTop True)
        (.TopOffset 16)
        (.BottomOffset 16)
        (.LeftOffset 10)
        (.RightOffset 10)
        (.ResourceHints "Template:OKCancelDialog.Content")
        (.AttachBottom True)
        (.Layout
            (Grid
                (Rows 1)
                (Cols 1)
                (Grid
                    (Rows 0 1)
                    (Cols 1)
                    (Size 2 1)
                    (Grid
                        (Rows 0 1)
                        (Cols 1)
                        Label1 TextAreaInput
                    )
                )
            )
        )
    )
)
